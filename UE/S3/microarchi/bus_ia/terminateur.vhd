-- ########################################################################
-- $Software: busiac
-- $section : hardware component
--       $Id: terminateur.vhd 322 2015-05-29 06:43:59Z ia $ 
--  $HeadURL: svn://lunix120.ensiie.fr/ia/cours/archi/projet/busiac/vhdl/terminateur.vhd $
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
-- ATTENTION:
-- Ceci un template, les trous marqués "..." doivent être comblés pour
-- pouvoir être compilé, puis fonctionné.
-------------------------------------------------------------------------------

-------------------------------------------------------------------------------
-- Ce module transfert tous les messages (????,addrsrc,addrdest,data) venant de
-- busin.
-- Si ????=???1, il transmet le messages sur busdump.
-- Si ????=???0 et addrdest!=MYADDR, il transmet sur busout le message
--   (???1,addrsrc,addrdest,data).
-- Si addrdest==MYADDR, il transmet le message sur busdump.
--
-- Du coté busin, il suit le protocole "poignée de main" (signaux: busin, 
--   busin_valid, busin_eated).
-- Du coté busout, il suit le protocole "poignée de main" (signaux: busout, 
--   busout_valid, busout_eated).
-- Du coté busdump, il suit le protocole "poignée de main" (signaux: busdump, 
--   busdump_valid, busdump_eated).
-------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;

entity terminateur is
	generic(
		MYADDR : STD_LOGIC_VECTOR(7 downto 0) :=  "11111111" -- 255
    );
    port(
        clk          : in  STD_LOGIC;
        reset        : in  STD_LOGIC;
        -- interface busin
        busin        : in  STD_LOGIC_VECTOR(43 downto 0);
        busin_valid  : in  STD_LOGIC;
        busin_eated  : out STD_LOGIC; 
        -- interface busout
        busout       : out STD_LOGIC_VECTOR(43 downto 0);
        busout_valid : out STD_LOGIC;
        busout_eated : in  STD_LOGIC; 
        -- interface busdump
        busdump      : out STD_LOGIC_VECTOR(43 downto 0);
        busdump_valid: out STD_LOGIC;
        busdump_eated: in  STD_LOGIC);
end terminateur;

architecture montage of terminateur is

-------------------------------------------------------------------------------
--  Partie Opérative
-------------------------------------------------------------------------------
    -- Registre de transfert entre busin et busout
    type T_CMD_tft is (INIT, NOOP);
    signal CMD_tft :  T_CMD_tft ; 
    signal R_tft   :  STD_LOGIC_VECTOR (43 downto 0);

-------------------------------------------------------------------------------
-- Partie Contrôle
-------------------------------------------------------------------------------
    -- adresse destination & bit de boucle du message lu
    alias  busin_addrdest : STD_LOGIC_VECTOR(7 downto 0) is busin(31 downto 24);
    alias  busin_loop     : STD_LOGIC is busin(40);

    type STATE_TYPE is (
        ST_READ_BUSIN, ST_WRITE_OUT, ST_WRITE_DUMP
	 );
    signal state : STATE_TYPE;
    
begin

-------------------------------------------------------------------------------
--  Partie Opérative
-------------------------------------------------------------------------------

    process (clk)
    begin if clk'event and clk = '1' then
	     IF CMD_tft = INIT THEN 
		      R_tft <= busin ;
		  END IF;
    end if; end process;

    busout(43 downto 41)  <= R_tft(43 downto 41);
    busout(40)            <= '1';
    busout(39 downto  0)  <= R_tft(39 downto  0);

    busdump <= R_tft;

-------------------------------------------------------------------------------
-- Partie Contrôle
-------------------------------------------------------------------------------
-- Inputs:  busin_valid, busout_eated, busdump_eated, busin_addrdest, busin_loop
-- Outputs: busin_eated, busout_valid, busdump_valid, CMD_tft
-------------------------------------------------------------------------------

    -- fonction de transitition    
    process (reset,clk)
    begin
        if reset = '1' then
            state <= ST_READ_BUSIN;
        elsif clk'event and clk = '1' then
            case state is
                when ST_READ_BUSIN =>
                    IF busin_valid='1' THEN
						      IF busin_loop = '1' OR busin_addrdest = MYADDR THEN
								    state <= ST_WRITE_DUMP ;
							   ELSE
								    state <= ST_WRITE_OUT ;
								END IF ;
						  END IF ;

				    when ST_WRITE_OUT =>
					     IF busout_eated = '1' THEN
						      state <= ST_READ_BUSIN;
						  END IF ;
						  
					 when ST_WRITE_DUMP =>
					     IF busdump_eated = '1' THEN
						      state <= ST_READ_BUSIN;
						  END IF ;
						  
            end case;
        end if;
    end process;

    -- fonction de sortie    
    with state  select busin_eated <=
         '1'    when   ST_READ_BUSIN,
         '0'    when   others; 

    with state  select busout_valid <=
         '1'    when    ST_WRITE_OUT,
         '0'    when   others; 

    with state  select busdump_valid <=
         '1'    when    ST_WRITE_DUMP,
         '0'    when   others; 

    with state  select CMD_tft <=
         INIT   when   ST_READ_BUSIN,
         NOOP   when   others; 

end montage;

