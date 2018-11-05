-- ########################################################################
-- $Software: busiac
-- $section : hardware component
--       $Id: rs232out.vhd 322 2015-05-29 06:43:59Z ia $ 
--  $HeadURL: svn://lunix120.ensiie.fr/ia/cours/archi/projet/busiac/vhdl/rs232out.vhd $
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
-- Ce module sérialise l'entrée DATA de 8 bits sur la sortie TX.
--
-- le format écrit est:
--    - 1 start bit
--    - 8 bit de données
--    - 1 stop bits
--
-- La sortie BUSY indique que le module est en train de sérialiser.
-- 
-- Pour sérialiser une nouvelle valeur, il faut:
--    * attendre que BUSY soit nul.
--    * la positionner sur DATA et mettre NDATA à 1 au moins 1 cycle.
--
-- Pour fixer le BAUD du composant utilisez les paramètres génériques
-- BAUD et FREQ ci dessous.
-------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;

entity rs232out is
	generic(
		FREQ : integer :=  50000000;  -- Frequence de clk
	   BAUD : integer :=  9600);     -- Baud de Rx
    port(
        clk     : in     STD_LOGIC;
        reset   : in     STD_LOGIC;
        Tx      : out    STD_LOGIC;
        Data    : in     STD_LOGIC_VECTOR(7 downto 0);
        Ndata   : in     STD_LOGIC;
        Busy    : out    STD_LOGIC);
end rs232out;

architecture montage of rs232out is

-------------------------------------------------------------------------------
--  Partie Opérative
-------------------------------------------------------------------------------

    type T_CMD_i is (NOOP, COUNT, INIT);
    signal CMD_i :  T_CMD_i ; 
    signal R_i   :  integer RANGE 0 TO 15;
    signal VT_endLoop:  STD_LOGIC;

    type T_CMD_baud is (NOOP, COUNT, INIT);
    signal CMD_baud :  T_CMD_baud ; 
    signal R_baud:  integer RANGE 0 TO (FREQ)/BAUD;
    signal VT_endbaud:  STD_LOGIC;

    type T_CMD_data is (NOOP, SHIFT, INIT);
    signal CMD_data : T_CMD_data ;
    signal R_data   : STD_LOGIC_VECTOR(8 downto 0);  --    0 : 1 start bit
                                                     --  8:1 : 8 data bits
-------------------------------------------------------------------------------
-- Partie Contrôle
-------------------------------------------------------------------------------
    --Description des états
    type STATE_TYPE is (ST_BEGIN, ST_FOR, ST_ATT, ST_ADV);
    signal state : STATE_TYPE;
	 
begin

-------------------------------------------------------------------------------
--  Partie Opérative
-------------------------------------------------------------------------------

    process (clk)
    begin if clk'event and clk = '1' then
        -- R_i
        if	  ( CMD_i = INIT ) then
            R_i <= 11 ;
        elsif ( CMD_i = COUNT ) then
            R_i <= R_i - 1;
        else
            R_i <= R_i;
        end if;
        -- R_baud
        if	( CMD_baud = INIT ) then
            R_baud <= FREQ/BAUD ;
        elsif ( CMD_baud = COUNT ) then
            R_baud <= R_baud - 1;
        else
            R_baud <= R_baud;
        end if;
        -- R_data
		  if	  ( CMD_data = INIT ) then
				-- V = E + '0'
				R_data(8 downto 1) <= Data;
				R_data(0) <= '0';
		  elsif ( CMD_data = SHIFT ) then
				-- v = '1' + (v >> 1)
				R_data(7 downto 0) <= R_data(8 downto 1);
				R_data(8) <= '1';
		  else 
				R_data <= R_data;
		  end if ;
    end if; end process;
    
    VT_endbaud <= '1' WHEN R_Baud = 0 ELSE '0' ;
    VT_endLoop <= '1' WHEN R_i = 0 ELSE '0' ;

-------------------------------------------------------------------------------
-- Partie Contrôle
-------------------------------------------------------------------------------
-- Inputs:  Ndata VT_endLoop VT_endBaud
-- Outputs: Tx Busy  CMD_i CMD_baud CMD_data
-------------------------------------------------------------------------------

    -- fonction de transitition    
    process (reset,clk)
    begin
        if reset = '1' then
            state <= ST_BEGIN;
        elsif clk'event and clk = '1' then
            case state is
                when ST_BEGIN =>
							-- si go, alors on commence à serialiser
							if Ndata = '1' then
								state <= ST_FOR;
							end if;
						  
					 when ST_FOR => 
							if VT_endLoop = '1' then
								state <= ST_BEGIN;
							else
								state <= ST_ATT;
							end if;
							
					 when ST_ATT =>
							if VT_endbaud = '1' then
								state <= ST_ADV;
							end if;
							
					 when ST_ADV =>
							state <= ST_FOR;
							
            end case;
        end if;
    end process;

    -- fonction de sortie    
   with state select tx   <=
			'1'			when	ST_BEGIN,
			R_Data(0)	when	others
	;
   
	with state select busy   <=
			'0'		when 		ST_BEGIN,
			'1'		when 		others
	;

   with state select CMD_i   <=
			INIT		when 		ST_BEGIN,
			COUNT		when 		ST_ADV,
			NOOP		when 		others
	;

    with state select CMD_baud <=
			COUNT		when 		ST_ATT,
			INIT		when 		others
	;

    with state select CMD_data <=
			INIT		when 		ST_BEGIN,
			SHIFT		when 		ST_ADV,
			NOOP		when 		others
		;
		
end montage;
