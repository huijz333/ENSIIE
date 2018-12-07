-------------------------------------------------------------------------------
--
-- Ce bloc est le wrapper dans le bus ia
-- 
-- Ce module transfert tous les messages (????,addrsrc,addrdest,data) venant de
-- busin.
--
-- data est stocké dans un registre
--
-- Si addrdest==MYADDR, data est transmis sur busv
-- Sinon, tout le message est transféré sur busout
--
-- Du coté busin, il suit le protocole "poignée de main" (signaux: busin, 
--   busin_valid, busin_eated).
--
-- Du coté busout, il suit le protocole "poignée de main" (signaux: busout, 
--   busout_valid, busout_eated).
--
-- Du coté busmsg, la valeur du message est transmise
-------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use ieee.numeric_std.all;

ENTITY wrapper IS
	GENERIC(
		       MYADDR : STD_LOGIC_VECTOR(7 downto 0) :=  "00001010" -- 10
	       );
	PORT(
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

		-- les 32 valeurs du 7 segments R_SSurées (7 * 32 = 224) + N sur 6 bits
		busSS : out STD_LOGIC_VECTOR(229 downto 0);

		-- N_clock : nombre de clock à attendre pour générer un tick
		busNClock : out STD_LOGIC_VECTOR(21 downto 0)
);
END wrapper;

ARCHITECTURE montage OF wrapper IS

	-------------------------------------------------------------------------------
	--  Partie Opérative
	-------------------------------------------------------------------------------

	-- Registre de transfert entre busin et busout
	type T_CMD_tft is (INIT, NOOP);
	signal CMD_tft :  T_CMD_tft ; 
	signal R_tft   :  STD_LOGIC_VECTOR (43 downto 0);
	
	type T_CMD_msg is (LOAD, NOOP);
	signal CMD_msg :  T_CMD_msg ; 
	
		-- registre stockant les 32 valeurs du 7 segments R_SSurées (7 * 32 = 224)
	signal R_SS : STD_LOGIC_VECTOR(229 downto 0);

	-- registre stockant V (nombre de master clock à attendre avant de générer un tick)
	signal R_N_CLOCK : STD_LOGIC_VECTOR(21 downto 0);
	

	-------------------------------------------------------------------------------
	-- Partie Contrôle
	-------------------------------------------------------------------------------
	-- adresse destination
	alias busin_addrdest : STD_LOGIC_VECTOR(7 downto 0) is busin(31 downto 24);

	type STATE_TYPE is (
        ST_READ_BUSIN, ST_WRITE_OUT, ST_LOAD_MSG
	);
	signal state : STATE_TYPE;

BEGIN

	-------------------------------------------------------------------------------
	--  Partie Opérative
	-------------------------------------------------------------------------------
	 
	PROCESS (reset, clk)
	BEGIN
		-- si on reset
		IF reset = '1' THEN
			-- 5 000 000 <=> 100 ticks par secondes
			-- 5 000 000 / 2 = 2 500 000 = 0b1001100010010110100000
			R_N_CLOCK <= "1001100010010110100000";
			
			-- R_SSure le serpentin pour qu'il soit vide
			R_SS <= (5 => '1', others => '0');
			
		ELSIF clk'event AND clk = '1' THEN
	 
			-- commandes de transfert bus ia
			-- si l'on doit lire le message, on le stocke
			-- dans le registre 'R_tft'
			IF CMD_tft = INIT THEN 
				R_tft <= busin ;
			END IF;
				
			IF CMD_msg = LOAD THEN
				-- switch l'id du message
				CASE R_tft(23 downto 22) IS

					-- hinit(n_clock)
					WHEN "00" =>
						R_N_CLOCK <= R_tft(21 downto 0);

					-- clr(s)
					WHEN "01" =>
						R_SS <= (5 => '1', others => '0');

					-- si commande set-N(n)
					WHEN "10" =>
						R_SS(5 downto 0) <= R_tft(5 downto 0);

					-- si commande set-val(i, v)
					WHEN "11" =>
						-- switch l'id de la frame
						CASE unsigned(R_tft(5 downto 0)) IS

							WHEN to_unsigned(1, 6) =>
								R_SS(  6 downto   0) <= R_tft(12 downto 6);

							WHEN to_unsigned(2, 6) =>
								R_SS(  13 downto   7) <= R_tft(12 downto 6);

							WHEN to_unsigned(3, 6) =>
								R_SS(  20 downto   14) <= R_tft(12 downto 6);

							WHEN to_unsigned(4, 6) =>
								R_SS(  27 downto   21) <= R_tft(12 downto 6);

							WHEN to_unsigned(5, 6) =>
								R_SS(  34 downto   28) <= R_tft(12 downto 6);

							WHEN to_unsigned(6, 6) =>
								R_SS(  41 downto   35) <= R_tft(12 downto 6);

							WHEN to_unsigned(7, 6) =>
								R_SS(  48 downto   42) <= R_tft(12 downto 6);

							WHEN to_unsigned(8, 6) =>
								R_SS(  55 downto   49) <= R_tft(12 downto 6);

							WHEN to_unsigned(9, 6) =>
								R_SS(  62 downto   56) <= R_tft(12 downto 6);

							WHEN to_unsigned(10, 6) =>
								R_SS(  69 downto   63) <= R_tft(12 downto 6);

							WHEN to_unsigned(11, 6) =>
								R_SS(  76 downto   70) <= R_tft(12 downto 6);

							WHEN to_unsigned(12, 6) =>
								R_SS(  83 downto   77) <= R_tft(12 downto 6);

							WHEN to_unsigned(13, 6) =>
								R_SS(  90 downto   84) <= R_tft(12 downto 6);

							WHEN to_unsigned(14, 6) =>
								R_SS(  97 downto   91) <= R_tft(12 downto 6);

							WHEN to_unsigned(15, 6) =>
								R_SS( 104 downto   98) <= R_tft(12 downto 6);

							WHEN to_unsigned(16, 6) =>
								R_SS( 111 downto   105) <= R_tft(12 downto 6);

							WHEN to_unsigned(17, 6) =>
								R_SS( 118 downto   112) <= R_tft(12 downto 6);

							WHEN to_unsigned(18, 6) =>
								R_SS( 125 downto   119) <= R_tft(12 downto 6);

							WHEN to_unsigned(19, 6) =>
								R_SS( 132 downto   126) <= R_tft(12 downto 6);

							WHEN to_unsigned(20, 6) =>
								R_SS( 139 downto   133) <= R_tft(12 downto 6);

							WHEN to_unsigned(21, 6) =>
								R_SS( 146 downto   140) <= R_tft(12 downto 6);

							WHEN to_unsigned(22, 6) =>
								R_SS( 153 downto   147) <= R_tft(12 downto 6);

							WHEN to_unsigned(23, 6) =>
								R_SS( 160 downto   154) <= R_tft(12 downto 6);

							WHEN to_unsigned(24, 6) =>
								R_SS( 167 downto   161) <= R_tft(12 downto 6);

							WHEN to_unsigned(25, 6) =>
								R_SS( 174 downto   168) <= R_tft(12 downto 6);

							WHEN to_unsigned(26, 6) =>
								R_SS( 181 downto   175) <= R_tft(12 downto 6);

							WHEN to_unsigned(27, 6) =>
								R_SS( 188 downto   182) <= R_tft(12 downto 6);

							WHEN to_unsigned(28, 6) =>
								R_SS( 195 downto   189) <= R_tft(12 downto 6);

							WHEN to_unsigned(29, 6) =>
								R_SS( 202 downto   196) <= R_tft(12 downto 6);

							WHEN to_unsigned(30, 6) =>
								R_SS( 209 downto   203) <= R_tft(12 downto 6);

							WHEN to_unsigned(31, 6) =>
								R_SS( 216 downto   210) <= R_tft(12 downto 6);

							WHEN to_unsigned(32, 6) =>
								R_SS( 223 downto  217) <= R_tft(12 downto 6);

							WHEN others =>

						-- TODO
						-- R_SS((i + 1) * 7 - 1, i * 7) <= R_Msg(6 downto 0);
						END CASE;
				END CASE;
			END IF ;
		END IF;
	END PROCESS;

	-- sortie
	busNClock <= R_N_CLOCK;
	busSS     <= R_SS;
	busout    <= R_tft;

	-------------------------------------------------------------------------------
	-- Partie Contrôle
	-------------------------------------------------------------------------------
	-- Inputs:  busin_valid, busout_eated, busin_addrdest
	-- Outputs: busin_eated, busout_valid, CMD_tft
	-------------------------------------------------------------------------------

	-- fonction de transitition    
	PROCESS (reset,clk)
	BEGIN
		IF reset = '1' THEN
			state <= ST_READ_BUSIN;
		ELSIF clk'event AND clk = '1' THEN
			CASE state IS
				WHEN ST_READ_BUSIN =>
					IF busin_valid='1' THEN
						IF busin_addrdest = MYADDR THEN
							state <= ST_LOAD_MSG ;
						ELSE
							state <= ST_WRITE_OUT ;
						END IF ;
					END IF ;

				WHEN ST_WRITE_OUT =>
					IF busout_eated = '1' THEN
						state <= ST_READ_BUSIN;
					END IF ;
					
				WHEN ST_LOAD_MSG =>
					state <= ST_READ_BUSIN;

			END CASE;
		END IF;
	END PROCESS;

    -- fonction de sortie    
   with state  select busin_eated <=
		'1'    when   ST_READ_BUSIN,
      '0'    when   others; 

   with state  select busout_valid <=
      '1'    when   ST_WRITE_OUT,
      '0'    when   others
	; 

	with state  select CMD_tft <=
      INIT   when   ST_READ_BUSIN,
      NOOP   when   others
	;
			
	with state  select CMD_msg <=
      LOAD   when   ST_LOAD_MSG,
      NOOP   when   others
	;
			
end montage;

