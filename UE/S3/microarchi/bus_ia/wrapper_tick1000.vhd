-------------------------------------------------------------------------------
--
-- Ce bloc est le wrapper dans le bus ia
-- 
-- Ce module transfert tous les messages (????,addrsrc,addrdest,data) venant de
-- busin.
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
-- Tous les 1000 ticks, ce bloc envoit un message au PC
-------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use ieee.numeric_std.all;

ENTITY wrapper_tick1000 IS
	GENERIC(
		       MYADDR : STD_LOGIC_VECTOR(7 downto 0) :=  "00001010" -- 10
	       );
	PORT(
		clk          : in  STD_LOGIC;
		reset        : in  STD_LOGIC;
		
		-- la valeur du tick
		T : in STD_LOGIC;

		-- interface busin
		busin        : in  STD_LOGIC_VECTOR(43 downto 0);
		busin_valid  : in  STD_LOGIC;
		busin_eated  : out STD_LOGIC; 
		
		-- interface busout
		busout       : out STD_LOGIC_VECTOR(43 downto 0);
		busout_valid : out STD_LOGIC;
		busout_eated : in  STD_LOGIC
);
END wrapper_tick1000;

ARCHITECTURE montage OF wrapper_tick1000 IS

	-------------------------------------------------------------------------------
	--  Partie Opérative
	-------------------------------------------------------------------------------

	-- Registre de transfert entre busin et busout
	type T_CMD_tft is (INIT, NOOP, TICK);
	signal CMD_tft :  T_CMD_tft ; 
	signal R_tft   :  STD_LOGIC_VECTOR (43 downto 0);
	
	type T_CMD_msg is (LOAD, NOOP);
	signal CMD_msg :  T_CMD_msg ; 

	-- registre indiquant qu'un message doit être envoyé tous les 1000 ticks ou non
	signal R_CHCK : STD_LOGIC;

	-- registre comptant le nombre de tick
	signal R_CNT : unsigned(9 downto 0);
	
	-- message pingant le PC tous les 1000 ticks
	SIGNAL mess_resultat :  STD_LOGIC_VECTOR (43 DOWNTO 0);

	-------------------------------------------------------------------------------
	-- Partie Contrôle
	-------------------------------------------------------------------------------
	-- adresse destination
	alias busin_addrdest : STD_LOGIC_VECTOR(7 downto 0) is busin(31 downto 24);

	type STATE_TYPE is (
        ST_READ_BUSIN, ST_WRITE_OUT, ST_LOAD_CHCK, ST_TICK
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
			-- on met le compteur à 0
			R_CNT <= to_unsigned(0, 10);

		ELSIF clk'event AND clk = '1' THEN
	 
			-- commandes de transfert bus ia
			-- si l'on doit lire le message, on le stocke
			-- dans le registre 'R_tft'
			IF CMD_tft = INIT THEN 
				R_tft <= busin ;
			END IF;

			-- on charge la valeur de 'N'
			IF CMD_msg = LOAD THEN
				R_CHCK <= R_tft(0);
			END IF ;

			-- on compte le nombre de tick
			IF T = '1' THEN
				IF R_CNT = to_unsigned(1000, 10) THEN
					R_CNT <= to_unsigned(0, 10);
				ELSE
					R_CNT <= R_CNT + 1;
				END IF;
			END IF;
			
		END IF;
	END PROCESS;

	-- crée le message à envoyer au PC
	mess_resultat(43 DOWNTO 40) <= "0000";
	mess_resultat(39 DOWNTO 32) <= MYADDR;
	mess_resultat(31 DOWNTO 24) <= R_tft(39 DOWNTO 32);
	mess_resultat(23 DOWNTO 0)  <= "000000000000000000000001";

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
							state <= ST_LOAD_CHCK;
						ELSIF R_CHCK = '1' AND R_CNT = to_unsigned(1000, 10) THEN
							state <= ST_TICK;
						ELSE
							state <= ST_WRITE_OUT ;
						END IF ;
					END IF ;

				WHEN ST_WRITE_OUT =>
					IF busout_eated = '1' THEN
						state <= ST_READ_BUSIN;
					END IF ;
					
				WHEN ST_LOAD_CHCK =>
					state <= ST_READ_BUSIN;
					
				WHEN ST_TICK =>
					state <= ST_READ_BUSIN;

			END CASE;
		END IF;
	END PROCESS;

    -- fonction de sortie    
   with state  select busin_eated <=
	   '1'    when   ST_READ_BUSIN,
      '0'    when   others
	; 

   with state  select busout_valid <=
      '1'    when   ST_WRITE_OUT,
	  '1'    when   ST_TICK,
      '0'    when   others
	; 

	with state  select CMD_tft <=
      INIT   when   ST_READ_BUSIN,
      NOOP   when   others
	;
	
	with state SELECT busout <=
		mess_resultat  WHEN   ST_TICK,
		R_tft          WHEN   OTHERS
	;
			
	with state  select CMD_msg <=
      LOAD   when   ST_LOAD_CHCK,
      NOOP   when   others
	;
			
end montage;

