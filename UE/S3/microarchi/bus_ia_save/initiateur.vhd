-- ########################################################################
-- $Software: busiac
-- $section : hardware component
--       $Id: initiateur.vhd 327 2015-06-03 19:18:19Z ia $ 
--  $HeadURL: svn://lunix120.ensiie.fr/ia/cours/archi/projet/busiac/vhdl/initiateur.vhd $
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

-------------------------------------------------------------------------------
-- Ce module transfert tous les messages venant de busin sur busout.
-- De plus, ce module récupère 4 octets B3,B2,B1,B0 1 par 1 sur RS232in et
-- les regroupe pour réaliser un message (0000,addrsrc,addrdest,data)
-- (voir bus.txt) qu'il transfert sur busout.
-- B0 est le premier octet reçu, B3 est le dernier octet reçu.   
--
--   addrsrc  = 11111111 (255)
--   addrdest = B3
--   data     = B2<<16 | B1<<8 | B0
--
-- Du coté busin, il suit le protocole "poignée de main" (signaux: busin, 
-- busin_valid, busin_eated).
-- Du coté busout, il suit le protocole "poignée de main" (signaux: busout, 
-- busout_valid, busout_eated).
-- Du coté RS232in, il suit le protocole du module RS232in (signaux: Data, Ndata).
-- 
-- Attention:
--   - il n'y a pas de contrôle de flux du cote RS232in, donc si des données
--     arrive sur Data et que le bus est bloqué, elles seront perdues.
--   - ce module assume que Data reste stable au moins 3 cycles après la pulse Ndata
-------------------------------------------------------------------------------

LIBRARY IEEE;
USE IEEE.std_logic_1164.ALL;

ENTITY initiateur IS
    PORT(
        clk          : IN  STD_LOGIC;
        reset        : IN  STD_LOGIC;
        -- interface busin
        busin        : in  STD_LOGIC_VECTOR(43 DOWNTO 0);
        busin_valid  : in  STD_LOGIC;
        busin_eated  : out STD_LOGIC; 
        -- interface busout
        busout       : OUT STD_LOGIC_VECTOR(43 DOWNTO 0);
        busout_valid : OUT STD_LOGIC;
        busout_eated : IN  STD_LOGIC; 
        -- interface vers rs232out
        Data         : IN  STD_LOGIC_VECTOR(7 DOWNTO 0);
        Ndata        : IN  STD_LOGIC);
END initiateur;


ARCHITECTURE Montage OF initiateur IS
    -- compteur donnant le nombre d'octets a mettre dans R_32
    TYPE T_CMD_i IS (NOOP, COUNT, INIT);
    SIGNAL CMD_i :  T_CMD_i ; 
    SIGNAL R_i   :  INTEGER RANGE 0 TO 4;
    SIGNAL VT_endLoop:  STD_LOGIC;

    -- accumule les octets venant de Data.
    TYPE T_CMD_32 IS (NOOP, SHIFT);
    SIGNAL CMD_32 :  T_CMD_32 ; 
    SIGNAL R_32   :  STD_LOGIC_VECTOR (31 DOWNTO 0);

    -- Registre de transfert entre busin et busout
    TYPE T_CMD_tft IS (INIT, NOOP);
    SIGNAL CMD_tft :  T_CMD_tft ; 
    SIGNAL R_tft   :  STD_LOGIC_VECTOR (43 DOWNTO 0);

    -- Sauve une pulse de Ndata qd on est dans les etats de la FSM
    -- qui ne teste pas Ndata
    TYPE T_CMD_pulse IS (CLEAR, LOAD);
    SIGNAL CMD_pulse :  T_CMD_pulse ; 
    SIGNAL R_pulse   :  STD_LOGIC;
    SIGNAL R_data    :  STD_LOGIC_VECTOR(7 DOWNTO 0);

    -- les sources d'ecriture sur busout
    SIGNAL busout_rs232 : STD_LOGIC_VECTOR(43 DOWNTO 0);
    SIGNAL busout_tft   : STD_LOGIC_VECTOR(43 DOWNTO 0);

    --Description des �tats
    TYPE STATE_TYPE IS (
        ST_INIT,ST_WAIT_BUSIN_OR_NDATA,
        ST_BUSIN_AND_NDATA_LOADED,
        ST_NDATA_LOADED, ST_BUSIN_LOADED,
        ST_EndLoop, ST_NDATA_WRITE);
    SIGNAL state : STATE_TYPE;
    
    
BEGIN

-------------------------------------------------------------------------------
--  Partie Op�rative
-------------------------------------------------------------------------------

    PROCESS (clk)
    BEGIN IF clk'EVENT AND clk = '1' THEN
        -- R_i
        if    ( CMD_i = INIT ) then
            R_i <= 4 ;
        elsif ( CMD_i = COUNT ) then
            R_i <= R_i - 1;
        else
            R_i <= R_i;
        end if;
        -- R_32
        if    ( CMD_32 = SHIFT ) then
            R_32(31 DOWNTO 24) <= R_data;
            R_32(23 DOWNTO 16) <= R_32(31 DOWNTO 24);
            R_32(15 DOWNTO  8) <= R_32(23 DOWNTO 16);
            R_32( 7 DOWNTO  0) <= R_32(15 DOWNTO  8);
        else
            R_32 <= R_32 ;
        end if;
        -- R_tft
        if    ( CMD_tft = INIT ) then
            R_tft <= busin ;
        else
            R_tft <= R_tft ;
        end if;
        -- R_pulse
        if    ( CMD_pulse = LOAD ) then
            R_pulse <= R_pulse OR Ndata ;
        else -- CLEAR
            R_pulse <= '0' ;
        end if;
        -- R_data
        if (Ndata = '1') then
            R_data <= data;
        else
            R_data <= R_data;
        end if;
    END IF; END PROCESS;
    
    VT_endLoop <= '1' when R_i=0 else '0' ; 
    
    busout_rs232(43 DOWNTO 40) <= "0000";
    busout_rs232(39 DOWNTO 32) <= "11111111";
    busout_rs232(31 DOWNTO 0)  <= R_32;
    busout_tft                 <= R_tft;

-------------------------------------------------------------------------------
-- Partie Contr�le
-------------------------------------------------------------------------------
-- Inputs:  busout_eated Ndata R_pulse VT_endLoop busin_valid
-- Outputs: busout_valid CMD_i CMD_32 busin_eated CMD_tft CMD_pulse busout 
-------------------------------------------------------------------------------

    -- fonction de transitition    
    PROCESS (reset,clk)
    BEGIN
        if reset = '1' then
            state <= ST_INIT;
        ELSIF clk'EVENT AND clk = '1' THEN
            CASE state IS
			    WHEN ST_INIT =>
					state <= ST_WAIT_BUSIN_OR_NDATA;

                WHEN ST_WAIT_BUSIN_OR_NDATA =>
                    IF busin_valid = '1' AND  R_pulse = '1' THEN 
                        state <= ST_BUSIN_AND_NDATA_LOADED;
                    ELSIF R_pulse = '1' THEN
                        state <= ST_NDATA_LOADED;
                    ELSIF busin_valid = '1'  THEN 
                        state <= ST_BUSIN_LOADED;
                    END IF; 

                WHEN ST_BUSIN_LOADED =>
                    if busout_eated = '1' then
                        state <= ST_WAIT_BUSIN_OR_NDATA;
                    END IF; 

                WHEN ST_BUSIN_AND_NDATA_LOADED =>
                    if busout_eated = '1' then
                        state <= ST_NDATA_LOADED;
                    END IF; 

                WHEN ST_NDATA_LOADED =>
                    state  <= ST_EndLoop;

                WHEN ST_EndLoop =>
                    IF VT_EndLoop = '1' THEN
                        state  <= ST_NDATA_WRITE;
                    else
                        state <= ST_WAIT_BUSIN_OR_NDATA;
                    END IF; 

                WHEN ST_NDATA_WRITE =>
                    if busout_eated = '1' then
                        state <= ST_WAIT_BUSIN_OR_NDATA;
                    END IF; 

            END CASE;
        END IF;
    END PROCESS;

    -- fonction de sortie    
    WITH state  SELECT busout_valid <=
        '1'     WHEN   ST_BUSIN_LOADED,
        '1'     WHEN   ST_NDATA_WRITE,
        '0'     WHEN   OTHERS; 

    WITH state  SELECT CMD_i <=
         INIT   WHEN   ST_INIT,
         INIT   WHEN   ST_NDATA_WRITE,
         COUNT  WHEN   ST_NDATA_LOADED,
         NOOP   WHEN   OTHERS; 

    WITH state  SELECT CMD_32 <=
         SHIFT  WHEN   ST_NDATA_LOADED,
         NOOP   WHEN   OTHERS; 

    WITH state  SELECT busin_eated <=
         '1'    WHEN   ST_WAIT_BUSIN_OR_NDATA,
         '0'    WHEN   OTHERS; 

    WITH state  SELECT CMD_tft <=
         INIT   WHEN   ST_WAIT_BUSIN_OR_NDATA,
         NOOP   WHEN   OTHERS; 

    WITH state  SELECT CMD_pulse <=
         CLEAR  WHEN   ST_NDATA_LOADED,
         LOAD   WHEN   OTHERS;

    WITH state  SELECT busout <=
         busout_rs232 WHEN   ST_NDATA_WRITE,
         busout_tft   WHEN OTHERS;

END Montage;

