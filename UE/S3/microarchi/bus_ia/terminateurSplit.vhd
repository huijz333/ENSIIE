-- ########################################################################
-- $Software: busiac
-- $section : hardware component
--       $Id: terminateurSplit.vhd 322 2015-05-29 06:43:59Z ia $ 
--  $HeadURL: svn://lunix120.ensiie.fr/ia/cours/archi/projet/busiac/vhdl/terminateurSplit.vhd $
-- $Author  : Ivan Auge (Email: auge@ensiie.fr)
-- ########################################################################
-- 
-- This file is part of the BUSIAC software: Copyright (C) 2010 by I. Auge.
-- 
-- This program is free software; you can redistribute it  and/or modify it
-- under the  terms of the GNU  General Public License  as published by the
-- Free Software Foundation;  either version 2 of the License,  or (at your
-- option) any later version.
-- 
-- BUSIAC software is distributed in the hope that it will be  useful, but
-- WITHOUT  ANY  WARRANTY ;  without  even  the  implied  warranty  of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
-- Public License for more details.
-- 
-- You should have received a copy  of the GNU General Public License along
-- with the GNU C Library; see the  file COPYING. If not, write to the Free
-- Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
-- 
-- ######################################################################*/

LIBRARY IEEE;
USE IEEE.std_logic_1164.ALL;

-------------------------------------------------------------------------------
-- Ce module récupère un message (????,addrsrc,addrdest,data) sur busin et le
-- découpe en 6 octets B5, B4, B3,B2,B1,B0 puis les envoie 1 par 1 sur RS232out.
-- B0 est le premier octet envoyé, B5 est le dernier octet envoyé.   
--   busin[43:40] = control = B5  (0000CCCC)
--   busin[39:32] = adddest = B4
--   busin[31:24] = addrsrc = B3
--   busin[23: 0] = data    = B2<<16 | B1<<8 | B0
--
-- Du coté busin, il suit le protocole "poignee de main" (signaux: busin, 
-- busin_valid, busin_eated).
-- Du coté RS232out, il suit le protocole du module RS232out (signaux:
-- Data, Ndata, Busy).
-------------------------------------------------------------------------------

ENTITY terminateurSplit IS
    PORT(
        clk          : IN  STD_LOGIC;
        reset        : IN  STD_LOGIC;
        -- interface busin
        busin        : in  STD_LOGIC_VECTOR(43 DOWNTO 0);
        busin_valid  : in  STD_LOGIC;
        busin_eated  : out STD_LOGIC; 
        -- interface vers rs232in
        Data         : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
        Ndata        : OUT STD_LOGIC;
        Busy         : IN  STD_LOGIC);
END terminateurSplit;


ARCHITECTURE Montage OF terminateurSplit IS
    -- compteur donnant le nombre d'octets de R deja envoye sur Data
    TYPE T_CMD_i IS (NOOP, COUNT, INIT);
    SIGNAL CMD_i :  T_CMD_i ; 
    SIGNAL R_i   :  INTEGER RANGE 0 TO 6;
    SIGNAL VT_endLoop:  STD_LOGIC;

    -- accumule les octets venant de Data.
    TYPE T_CMD IS (NOOP, LOAD, SHIFT);
    SIGNAL CMD :  T_CMD ; 
    SIGNAL R   :  STD_LOGIC_VECTOR (47 DOWNTO 0);

    -- FSM
    TYPE STATE_TYPE IS (
        ST_READ_BUSIN, ST_WAIT_NOBUSY, ST_WRITE, ST_LOOP);
    SIGNAL state : STATE_TYPE;

BEGIN

-------------------------------------------------------------------------------
--  Partie Opérative
-------------------------------------------------------------------------------

    PROCESS (clk)
    BEGIN IF clk'EVENT AND clk = '1' THEN
        -- R_i
        if    ( CMD_i = INIT ) then
            R_i <= 6 ;
        elsif ( CMD_i = COUNT ) then
            R_i <= R_i - 1;
        else
            R_i <= R_i;
        end if;
        -- R
        if       ( CMD = LOAD ) then
            R(43 DOWNTO 0)  <= busin ;
            R(47 DOWNTO 44) <= "0000" ;
        elsif    ( CMD = SHIFT ) then
            R(39 DOWNTO 32) <= R(47 DOWNTO 40);
            R(31 DOWNTO 24) <= R(39 DOWNTO 32);
            R(23 DOWNTO 16) <= R(31 DOWNTO 24);
            R(15 DOWNTO  8) <= R(23 DOWNTO 16);
            R( 7 DOWNTO  0) <= R(15 DOWNTO  8);
        else
            R <= R ;
        end if;
    END IF; END PROCESS;
    
    VT_endLoop <= '1' when R_i=0 else '0' ; 
    data       <= R(7 DOWNTO 0);

-------------------------------------------------------------------------------
-- Partie Contrôle
-------------------------------------------------------------------------------
-- Inputs:  busin_valid Busy  
-- Outputs: busin_eated Ndata CMD_i CMD 
-------------------------------------------------------------------------------

    -- fonction de transitition    
    PROCESS (reset,clk)
    BEGIN
        IF reset = '1' THEN
            state <= ST_READ_BUSIN;
        ELSIF clk'EVENT AND clk = '1' THEN
            CASE state IS
                WHEN ST_READ_BUSIN =>
                    IF busin_valid  = '1' THEN state <= ST_WAIT_NOBUSY; END IF; 
                WHEN ST_WAIT_NOBUSY =>
                    IF Busy  /= '1' THEN state <= ST_WRITE; END IF; 
                WHEN ST_WRITE =>
                    state <= ST_LOOP;
                WHEN ST_LOOP =>
                    IF VT_endLoop = '1' THEN
                        state <= ST_READ_BUSIN;
                    ELSE
                        state <= ST_WAIT_NOBUSY;
                    END IF; 
            END CASE;
        END IF;
    END PROCESS;

    -- fonction de sortie    
    WITH state  SELECT busin_eated <=
        '1'     WHEN   ST_READ_BUSIN,
        '0'     WHEN   OTHERS; 
    WITH state  SELECT Ndata <=
        '1'     WHEN   ST_WRITE,
        '0'     WHEN   OTHERS; 
    WITH state  SELECT CMD_i <=
        INIT    WHEN   ST_READ_BUSIN,
        COUNT   WHEN   ST_WRITE,
        NOOP    WHEN   OTHERS; 
    WITH state  SELECT CMD <=
        LOAD    WHEN   ST_READ_BUSIN,
        SHIFT   WHEN   ST_WRITE,
        NOOP    WHEN   OTHERS; 

END Montage;
