-- ########################################################################
-- $Software: busiac
-- $section : hardware component
--       $Id: plus12.vhd 325 2015-06-03 12:47:32Z ia $ 
--  $HeadURL: svn://lunix120.ensiie.fr/ia/cours/archi/projet/busiac/vhdl/plus12.vhd $
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
-- Ce module additionne 2 nombres de 12 bits signés.
-- Ses E/S sont les bus busin et busout.
--
-- Input:
--   busin_ctl (43 DOWNTO 40) : not used
--   busin_asrc(39 DOWNTO 32) : adresse emetteur (E_ADR)
--   busin_ades(31 DOWNTO 24) : adresse destination (MYADR)
--   busin_data(23 DOWNTO 12) : operande B en complement a 2
--   busin_data(11 DOWNTO  0) : operande A en complement a 2
--
-- Output:
--   busout_ctl (43 DOWNTO 40) : "0000"
--   busout_asrc(39 DOWNTO 32) : MYADR
--   busout_ades(31 DOWNTO 24) : E_ADR
--   busout_data(23)           : V (overflow)
--   busout_data(22)           : C (retenue sortante)
--   busout_data(21)           : N (résultat négatif)
--   busout_data(20)           : Z (résultat nul)
--   busout_data(19 DOWNTO 12) : "00000000"
--   busout_data(11 DOWNTO  0) : résultat en complément a 2 (A+B)
-------------------------------------------------------------------------------

LIBRARY IEEE;
USE IEEE.std_logic_1164.ALL;
use IEEE.numeric_std.all;

ENTITY plus12 IS
	GENERIC(
	    MYADR : STD_LOGIC_VECTOR(7 downto 0) :=  x"C0" );  -- 192
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
        -- debug
       debug         : OUT STD_LOGIC_VECTOR(7 DOWNTO 0)
   );
END plus12;


ARCHITECTURE Montage OF plus12 IS

    TYPE T_CMD_LoadNoop IS (LOAD, NOOP);

    -- partie operative
    -- le registre de transfert de busin vers busout
    SIGNAL CMD_tft : T_CMD_LoadNoop;
    SIGNAL R_tft   : STD_LOGIC_VECTOR(43 DOWNTO 0);

    -- le registre resultat de A+B, ov
    -- on etend R sur 13 bits pour avoir la retenue
    SIGNAL CMD_res : T_CMD_LoadNoop;
    SIGNAL R_res   : STD_LOGIC_VECTOR(12 DOWNTO 0);

    -- les operandes A et B (1 bit de plus pour la retenue)
    SIGNAL A,B     : STD_LOGIC_VECTOR (12 DOWNTO 0);  -- V1
    -- bits de retenue et de somme de A+B             -- V1
    SIGNAL r,s     : STD_LOGIC_VECTOR (12 DOWNTO 0);  -- V1
    -- SIGNAL A,B   : SIGNED (12 DOWNTO 0);              -- V2

    -- l' adresse destination
    SIGNAL busin_ades    :  STD_LOGIC_VECTOR ( 7 DOWNTO 0);

    -- message résulat
    SIGNAL mess_resultat :  STD_LOGIC_VECTOR (43 DOWNTO 0);

    -- partie controle
    TYPE STATE_TYPE IS (ST_READ, ST_WRITE_TFT, ST_COMPUTE, ST_WRITE_SUM);
    SIGNAL state : STATE_TYPE;
    
BEGIN

-------------------------------------------------------------------------------
--  Partie Opérative
-------------------------------------------------------------------------------
    busin_ades          <= busin(31 DOWNTO 24) ;
    a                   <= "0" & R_tft(23 DOWNTO 12) ; -- V1
    b                   <= "0" & R_tft(11 DOWNTO  0) ; -- V1
    -- a                   <= SIGNED (R_tft(23 DOWNTO 12)) ; -- V2
    -- b                   <= SIGNED (R_tft(11 DOWNTO  0)) ; -- V2

    mess_resultat(43 DOWNTO 40) <= "0000";
    mess_resultat(39 DOWNTO 32) <= MYADR;
    mess_resultat(31 DOWNTO 24) <= R_tft(39 DOWNTO 32);
    mess_resultat(23)           <= -- overflow
        '1' WHEN a(11)='1' AND b(11)='1' AND R_res(11)='0' ELSE -- N+N=P
        '1' WHEN a(11)='0' AND b(11)='0' AND R_res(11)='1' ELSE -- P+P=N
        '0' ;
    mess_resultat(22)           <= R_res(12);  -- cout
    mess_resultat(21)           <= R_res(11);  -- signe
    mess_resultat(20)           <=   -- null
        '1' WHEN R_res(11 downto 0) = x"000" ELSE '0';
    mess_resultat(19 DOWNTO 12) <=   "00000000" ;
    mess_resultat(11 DOWNTO 0)  <=   R_res(11 DOWNTO 0);

    -- s,r <-- a + b;                                -- V1
    s <=  a XOR b XOR r;                             -- V1
    r(0) <= '0';                                     -- V1
    r(12 DOWNTO 1) <=                                -- V1
        ( a(11 DOWNTO 0) AND b(11 DOWNTO 0) ) OR     -- V1
        ( a(11 DOWNTO 0) AND r(11 DOWNTO 0) ) OR     -- V1
        ( r(11 DOWNTO 0) AND b(11 DOWNTO 0) );       -- V1

    PROCESS (clk)
    BEGIN IF clk'EVENT AND clk = '1' THEN
        -- R_tft
        if    ( CMD_tft = LOAD ) then
            R_tft <= busin;
        end if;
        -- R_res
        if    ( CMD_res = LOAD ) then
            R_res(12 DOWNTO 0) <= s ;  -- V1
            -- R_res(12 DOWNTO 0) <= STD_LOGIC_VECTOR(a + b) ;  -- V2
        end if;
    END IF; END PROCESS;
    
-------------------------------------------------------------------------------
-- Partie Controle
-------------------------------------------------------------------------------
-- Inputs:  busin_valid busout_eated
-- Outputs: busin_eated busout_valid, CMD_res, CMD_tft, busout
-------------------------------------------------------------------------------

    -- fonction de transitition    
    PROCESS (reset,clk)
    BEGIN
      IF reset = '1' THEN
          state <= ST_READ;
      ELSIF clk'EVENT AND clk = '1' THEN
          CASE state IS
              WHEN ST_READ =>
                  IF busin_valid  = '1' and busin_ades = MYADR THEN
                      state <= ST_COMPUTE;
                  ELSIF busin_valid  = '1' and busin_ades /= MYADR THEN
                      state <= ST_WRITE_TFT;
                  END IF; 

              WHEN ST_COMPUTE =>
                  state  <= ST_WRITE_SUM;

              WHEN ST_WRITE_SUM =>
                  IF busout_eated = '1' THEN
                      state  <= ST_READ;
                  END IF; 

              WHEN ST_WRITE_TFT =>
                  IF busout_eated = '1' THEN
                      state  <= ST_READ;
                  END IF; 
          END CASE;
      END IF;
    END PROCESS;

    -- fonction de sortie    
    WITH state  SELECT busin_eated <=
         '1'    WHEN   ST_READ,
         '0'    WHEN   OTHERS; 

    WITH state  SELECT busout_valid <=
        '1'     WHEN   ST_WRITE_TFT,
        '1'     WHEN   ST_WRITE_SUM,
        '0'     WHEN   OTHERS; 

    WITH state  SELECT CMD_res <=
         LOAD   WHEN   ST_Compute,
         NOOP   WHEN   OTHERS; 

    WITH state  SELECT CMD_tft <=
         LOAD   WHEN   ST_READ,
         NOOP   WHEN   OTHERS; 

    WITH state  SELECT busout <=
        mess_resultat  WHEN   ST_WRITE_SUM,
        R_tft          WHEN   OTHERS;

END Montage;

