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

			-- les 32 valeurs du 7 segments configurées (7 * 32 = 224) + N sur 6 bits
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
	
		-- registre stockant les 32 valeurs du 7 segments configurées (7 * 32 = 224)
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
			
			-- configure le serpentin pour qu'il soit vide
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
						-- TODO
						-- R_SS((i + 1) * 7 - 1, i * 7) <= R_Msg(6 downto 0);
						
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
         '0'    when   others; 

    with state  select CMD_tft <=
         INIT   when   ST_READ_BUSIN,
         NOOP   when   others; 
			
	 with state  select CMD_msg <=
         LOAD   when   ST_LOAD_MSG,
         NOOP   when   others;
			
end montage;

