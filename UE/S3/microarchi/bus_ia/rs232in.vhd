-- ########################################################################
-- $Software: busiac
-- $section : hardware component
--       $Id: rs232in.vhd 325 2015-06-03 12:47:32Z ia $ 
--  $HeadURL: svn://lunix120.ensiie.fr/ia/cours/archi/projet/busiac/vhdl/rs232in.vhd $
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
-- Ce module désérialise l'entrée RX dans la sortie DATA de 8 bits.
--
-- le format lu est:
--    - 1 start bit
--    - 8 bit de données
--    - 1 ou plusieurs stop bits
-- 
-- Ce module met NDATA à 1 pendant un cycle quand une nouvelle
-- valeur est présente sur DATA d'où le chronogramme suivant
-- avec A, B et C des valeurs stables de NDATA.
-- La valeur reste stable pendant un BAUD.
--
-- NDATA    0000100000000000000000001000000000000000100000000000
-- DATA     ????AAAAA???????????????BBBBB???????????CCCCC???????
-- 
-- Pour fixer le BAUD du composant utilisez les paramètres génériques
-- BAUD et FREQ ci dessous.
-------------------------------------------------------------------------------

library IEEE ;
use IEEE.std_logic_1164.all ;

entity rs232in is
	generic(
		       FREQ : integer :=  50000000;  -- Frequence de clk
		       BAUD : integer :=  9600);     -- Baud de Rx
	port(
		    clk     : IN    STD_LOGIC;
		    reset   : IN    STD_LOGIC;
		    rx      : IN    STD_LOGIC;
		    Ndata   : OUT   STD_LOGIC;
		    Data    : OUT   STD_LOGIC_VECTOR(7 DOWNTO 0);
	-- debug
		    debug   : OUT   STD_LOGIC_VECTOR(7 DOWNTO 0)
	    );
end rs232in;


architecture montage of rs232in is

	signal CMD_sh:  STD_LOGIC;                      -- 0:noop, 1:shift
	signal R_sh:    STD_LOGIC_VECTOR(7 DOWNTO 0);

	type T_CMD_baud is (COUNT, INIT1P5B, INIT1B) ;
	signal CMD_baud: T_CMD_baud;
	signal VT_baud:  STD_LOGIC;
	signal R_baud:   INTEGER RANGE 0 TO (2*FREQ)/BAUD;

	type T_CMD_i is (NOOP, COUNT, INIT);
	signal CMD_i: T_CMD_i;
	signal R_i:   INTEGER RANGE 0 TO 10;
	signal VT_i:  STD_LOGIC;

    -- rx input avoid glitch
	signal rx_R : std_logic;
	signal rx_fifo_R : std_logic_vector(2 downto 0);

    --Description des états
	type STATE_TYPE is ( WAIT_StartBit, WAIT_1P5B, MainLoop, ECRIRE,
	WAIT_1B, GEN_PULSE, WAIT_FIN);
	signal state : STATE_TYPE;


begin

    -------------------------------------------------------------------------------
    --  Partie Opérative
    -------------------------------------------------------------------------------

	VT_baud <= '1' when R_baud=0 else '0';
	VT_i <= '1' when R_i=0 else '0';
	Data <= R_sh;

	process (clk)
	begin if clk'event and clk='1' then
		if CMD_baud=INIT1P5B then
			R_baud <= (FREQ+FREQ/2)/BAUD;  -- init à 1.5 * F/B
		elsif CMD_baud=INIT1B then
			R_baud <= FREQ/BAUD;          -- init à F/B
		else
			R_baud <= R_baud - 1;
		end if;     
	    -- R_i
		if CMD_i=NOOP then
			R_i <= R_i ;            -- on ne fait rien (noop)
		elsif CMD_i=INIT then
			R_i <= 8;             -- init à 8
		else
			R_i <= R_i - 1;     -- on compte
		end if;
	    -- R_sh
		if CMD_sh='1' then                  
			R_sh(6 downto 0) <= R_sh(7 downto 1);                 
			R_sh(7) <= rx_R;
		end if;
	    -- rx input avoid glitch
		rx_fifo_R(2 downto 1) <= rx_fifo_R(1 downto 0);
		rx_fifo_R(0)          <= rx;
		if rx_fifo_R = "000" or  rx_fifo_R = "111" then
			rx_R <= rx_fifo_R(2);
		end if;
	end if; end process; 

    -------------------------------------------------------------------------------
    -- Partie Contrôle
    -------------------------------------------------------------------------------

    -- fonction de transitition    
	process (reset,clk)
	begin
		if reset = '1' then
			state <= WAIT_StartBit;
		elsif clk'event and clk = '1' then case state is
			when WAIT_StartBit =>
				if rx_R = '0' then
					state <= WAIT_1P5B;
				else
					state <= WAIT_StartBit;
				end if; 

			when WAIT_1P5B =>
				if VT_baud='1' then
					state <= MainLoop;                            
				else
					state <= WAIT_1P5B;
				end if;

			when MainLoop=>
				if VT_i='1' then
					state <= GEN_PULSE;
				else
					state <= ECRIRE;
				end if;

			when ECRIRE=>
				state <= WAIT_1B;

			when WAIT_1B=>
				if VT_baud='1' then
					state <= MainLoop;
				else
					state <= WAIT_1B;
				end if;

			when GEN_PULSE =>
				state <= WAIT_FIN;

			when WAIT_FIN=>
				if VT_baud='1' then
					state <= WAIT_StartBit;
				else
					state <= WAIT_FIN;
				end if;

			when others =>
				state <= WAIT_StartBit;

		end case; end if; end process;

    -- fonction de sortie    
		with state select nData <=
		'1' when    GEN_PULSE,
		'0' when    others; 

		with state select CMD_sh <=
		'1' when    ECRIRE,
		'0' when    others;     

		with state select CMD_baud <=
		INIT1B    when    MainLoop,
		INIT1B    when    ECRIRE,
		INIT1P5B  when    WAIT_StartBit,
		COUNT     when    others; 

		with state select CMD_i <=
		INIT    when    WAIT_StartBit,
		INIT    when    WAIT_1P5B,
		COUNT   when    MainLoop,
		NOOP    when    others; 

		debug(7) <= rx_R;
		with state select debug(6 downto 0) <=
		"1000000"    when    WAIT_StartBit,
		"0100000"    when    WAIT_1P5B,
		"0010000"    when    MainLoop,
		"0001000"    when    ECRIRE,
		"0000100"    when    WAIT_1B,
		"0000010"    when    GEN_PULSE,
		"0000001"    when    WAIT_FIN,
		"1111111"    when    others; 
	end montage;
