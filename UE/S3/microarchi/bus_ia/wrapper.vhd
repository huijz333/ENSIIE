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
-- Du coté busout, il suit le protocole "poignée de main" (signaux: busout, 
--   busout_valid, busout_eated).
-- Du coté busv, la valeur est transmise sans check
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

		    -- les 32 valeurs du 7 segments configurées (7 * 32 = 224)
		    busSS : out STD_LOGIC_VECTOR(223 downto 0);

		    -- N : nombre de valeurs configurées à interpréter
		    busN : out STD_LOGIC_VECTOR(5 downto 0);

		    -- N_clock : nombre de clock à attendre pour générer un tick
		    busNClock : out STD_LOGIC_VECTOR(20 downto 0)
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

	-- Registre de stockage des données du dernier message reçu à destination de ce processeur
	TYPE T_CMD_Msg IS (LOAD, NOOP);
	signal CMD_Msg : T_CMD_Msg ;

	-- check (ON ou OFF) pour que le processeur envoit un message au PC
	-- tous les 1000 tickets de H100
	signal R_TICK1000 : STD_LOGIC;

	-- N : nombre de valeurs configurées à interpréter (2^5 = 32 ; 2^6 = 64)
	signal R_N : STD_LOGIC_VECTOR(5 downto 0);

	-- registre stockant les 32 valeurs du 7 segments configurées (7 * 32 = 224)
	signal R_SS : STD_LOGIC_VECTOR(223 downto 0);

	-- registre stockant V (nombre de master clock à attendre avant de générer un tick)
	signal R_N_CLOCK : STD_LOGIC_VECTOR(20 downto 0);

	-------------------------------------------------------------------------------
	-- Partie Contrôle
	-------------------------------------------------------------------------------
	-- adresse destination
	alias busin_addrdest : STD_LOGIC_VECTOR(7 downto 0) is busin(31 downto 24);

	-- valeur destination
	alias busin_addrdest : STD_LOGIC_VECTOR(7 downto 0) is busin(31 downto 24);

	type STATE_TYPE is (
		ST_READ_BUSIN, ST_WRITE_OUT, ST_WRITE_MSG
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
			-- 100 ticks par seconde (500 est en kilo-clock)
			R_N_CLOCK <= STD_LOGIC_VECTOR(to_unsigned(5000, 20));

			-- configure le serpentin pour qu'il soit vide
			R_N  <= STD_LOGIC_VECTOR(to_unsigned(32, 6));
			R_SS <= (others => '0');

			-- desactive le check TICK1000
			R_TICK1000 <= '0';

		ELSIF clk'event AND clk = '1' THEN

			-- commandes de transfert bus ia
			-- si l'on doit lire le message, on le stocke
			-- dans le registre 'R_tft'
			IF CMD_tft = INIT THEN 
				R_tft <= busin ;
			END IF;

			-- si le message doit être chargé (<=> qu'il nous est destiné)
			-- on met à jour les registre de stockages
			IF CMD_Msg = LOAD THEN

				-- switch l'id du message
				CASE R_tft(23 downto 21) IS

					-- hinit(n_clock)
					WHEN "000" =>
						R_N_CLOCK <= R_tft(20 downto 0);

					-- h-check-on()
					WHEN "001" =>
						R_TICK1000 <= '1';

					-- h-check-off()
					WHEN "010" =>
						R_TICK1000 <= '0';

					-- clr(s)
					WHEN "011" =>
						R_N  <= STD_LOGIC_VECTOR(to_unsigned(32, 6));
						R_SS <= (others => '0');

					-- si commande set-N(n)
					WHEN "100" =>
						R_N <= R_tft(5 downto 0);

					-- si commande set-val(i, v)
					WHEN "101" =>
						-- TODO
						-- R_SS((i + 1) * 7 - 1, i * 7) <= R_tft(6 downto 0);

			END IF ;
		END IF;
	END PROCESS;

	busout      <= R_tft;
	busSS       <= R_SS;
	busN        <= R_N;
	busNClock   <= R_N_CLOCK;
	busTICK1000 <= R_TICK1000;

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
							state <= ST_WRITE_MSG ;
						ELSE
							state <= ST_WRITE_OUT ;
						END IF ;
					END IF ;

				WHEN ST_WRITE_OUT =>
					IF busout_eated = '1' THEN
						state <= ST_READ_BUSIN;
					END IF ;

				WHEN ST_WRITE_MSG =>
					state <= ST_READ_BUSIN;

			END CASE;
		END IF;
	END PROCESS;

		-- fonction de sortie    
	WITH state SELECT busin_eated <=
		'1'    WHEN   ST_READ_BUSIN,
		'0'    WHEN   others
	; 

	WITH state WHEN busout_valid <=
		'1'    WHEN    ST_WRITE_OUT,
		'0'    WHEN   others
	; 

	WITH state SELECT CMD_tft <=
		INIT   WHEN   ST_READ_BUSIN,
		NOOP   WHEN   others
	; 

	WITH state SELECT CMD_Msg <=
		LOAD   WHEN   ST_WRITE_MSG,
		NOOP   WHEN   others
	;

end montage;

