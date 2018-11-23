-------------------------------------------------------------------------------
--
-- Ce bloc interprete des messages correspond aux commandes processeurs
--
--	Il prends un message en paramètre au format
--		(3 bits ; 21 bits)
--		(  cmd  ;   msg  )
--
--		- 'cmd' la commande a executé
--		- 'msg' le message de la commande
--
--	TODO comment
--
--
-------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use ieee.numeric_std.all;

ENTITY dispacher IS
	PORT(
			-- la master clock et le reset
		    clk   : in  STD_LOGIC;
		    reset : in  STD_LOGIC;
			
			-- message en entrée
			busmsg : in STD_LOGIC_VECTOR(23 downto 0);

		   -- les 32 valeurs du 7 segments configurées (7 * 32 = 224)
		   busSS : out STD_LOGIC_VECTOR(223 downto 0);

		   -- N : nombre de valeurs configurées à interpréter
		   busN : out STD_LOGIC_VECTOR(5 downto 0);

		   -- N_clock : nombre de clock à attendre pour générer un tick
		   busNClock : out STD_LOGIC_VECTOR(20 downto 0);
			
			-- est à '1' si l'on doit envoyer un message au PC tous les 1000 ticks
			busTICK1000 : out STD_LOGIC
	    );
END dispacher;

ARCHITECTURE montage OF dispacher IS

	-------------------------------------------------------------------------------
	--  Partie Opérative
	-------------------------------------------------------------------------------

	-- Commande pour 'busmsg'
	-- 	SEND => envoit du message
	-- 	NOOP => envoit d'un buffer 0
	TYPE T_CMD IS (READ_MSG, DISPATCH);
	signal cmd : T_CMD;

	-- le registre de stockage des données du dernier message reçu
	signal R_Msg : STD_LOGIC_VECTOR(23 downto 0);

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

	type STATE_TYPE is (
		ST_READ, ST_DISPATCH
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
			R_N_CLOCK <= STD_LOGIC_VECTOR(to_unsigned(5000, 21));

			-- configure le serpentin pour qu'il soit vide
			R_N  <= STD_LOGIC_VECTOR(to_unsigned(32, 6));
			R_SS <= (others => '0');

			-- desactive le check TICK1000
			R_TICK1000 <= '0';

		ELSIF clk'event AND clk = '1' THEN

			-- stocke le message
			IF cmd = READ_MSG THEN
				R_Msg <= busmsg;

			-- dispatch le message
			ELSIF cmd = DISPATCH THEN

				-- switch l'id du message
				CASE R_Msg(23 downto 21) IS

					-- si 0 == NOOP
					WHEN "000" =>
						-- ne rien faire

					-- hinit(n_clock)
					WHEN "001" =>
						R_N_CLOCK <= R_Msg(20 downto 0);

					-- h-check-on()
					WHEN "010" =>
						R_TICK1000 <= '1';

					-- h-check-off()
					WHEN "011" =>
						R_TICK1000 <= '0';

					-- clr(s)
					WHEN "100" =>
						R_N  <= STD_LOGIC_VECTOR(to_unsigned(32, 6));
						R_SS <= (others => '0');

					-- si commande set-N(n)
					WHEN "101" =>
						R_N <= R_Msg(5 downto 0);

					-- si commande set-val(i, v)
					WHEN "110" =>
						-- TODO
						-- R_SS((i + 1) * 7 - 1, i * 7) <= R_Msg(6 downto 0);
						
					WHEN others =>
						-- message invalide
				END CASE;
			END IF;
		END IF;
	END PROCESS;

	busSS       <= R_SS;
	busN        <= R_N;
	busNClock   <= R_N_CLOCK;
	busTICK1000 <= R_TICK1000;

	-------------------------------------------------------------------------------
	-- Partie Contrôle
	-------------------------------------------------------------------------------

	-- fonction de transitition    
	PROCESS (reset,clk)
	BEGIN
		IF clk'event AND clk = '1' THEN
			IF state = ST_READ THEN
				state <= ST_DISPATCH;
			ELSIF state = ST_DISPATCH THEN
				state <= ST_READ;
			END IF;
		END IF;
	END PROCESS;

	-- fonction de sortie    
	WITH state SELECT cmd <=
		READ_MSG   WHEN   ST_READ,
		DISPATCH   WHEN   ST_DISPATCH
	; 

end montage;

