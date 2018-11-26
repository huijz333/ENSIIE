
-------------------------------------------------------------------------------
--	Entree:
--		clk, reset : la clock et le reset
--		T          : un tick
--		Config     : la configuration du serpentin (32 * 7 + 6 = 230)
--
-- Sortie:
--		S(i) : '1' ou '0' selon que le i-ème segment du 7-segment doit
--				 etre allumé ou éteinds
--
-------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use ieee.numeric_std.all;

entity serpentinprog is
	port(
		-- entrées
		 clk    : in STD_LOGIC;
		 reset  : in STD_LOGIC;
		 T      : in  STD_LOGIC;
		 Config : in STD_LOGIC_VECTOR(229 downto 0);

		-- sorties vers le 7-segment
		 S      : OUT STD_LOGIC_VECTOR(6 downto 0)
	 );
end serpentinprog;

architecture montage of serpentinprog is

  -------------------------------------------------------------------------------
    --  Partie Opérative
    -------------------------------------------------------------------------------

	 -- registre de comptage de 1 à N
	signal I : unsigned(5 downto 0);
	signal I_GT_N : STD_LOGIC;

	-- Registre de commande
	type T_CMD is (INIT, GO_NEXT, NOOP);
	signal CMD : T_CMD ; 
	
    -------------------------------------------------------------------------------
    -- Partie Contrôle
    -------------------------------------------------------------------------------
	type STATE_TYPE is (
		ST_INIT, ST_WAIT, ST_NEXT
	);
	signal state : STATE_TYPE;

begin

    -------------------------------------------------------------------------------
    --  Partie Opérative
    -------------------------------------------------------------------------------

	PROCESS (reset, clk)
	begin
		IF clk'event and clk = '1' then
			-- si on initialise, on met le compteur à 0
			IF CMD = INIT THEN
				I <= "000000";
			-- si on doit passer à la frame suivante
			ELSIF CMD = GO_NEXT THEN
				I <= I + 1;
				IF I_GT_N = '1' THEN
					I <= "000000";
				END IF;
			END IF;
		END IF;
	END PROCESS;
	I_GT_N <= '0' WHEN (I < unsigned(Config(229 downto 224))) ELSE '0';

    -------------------------------------------------------------------------------
    -- Partie Contrôle
    -------------------------------------------------------------------------------
    -- Inputs:  T
    -- Outputs: S, CMD
    -------------------------------------------------------------------------------

    -- fonction de transitition    
		PROCESS (reset, clk)
		begin
			IF reset = '1' THEN
				state <= ST_INIT;
			ELSIF clk'event and clk = '1' THEN
				CASE state IS
					when ST_INIT =>
						state <= ST_WAIT ;

					when ST_WAIT =>
						IF T = '1' THEN
							state <= ST_NEXT ;
						END IF  ;
					
					WHEN ST_NEXT =>
						state <= ST_WAIT;

				END CASE;
			END IF;
		END PROCESS;

	 -- fonction de sortie
		with state  select CMD <=
			INIT     when   ST_INIT,
			NOOP     when   ST_WAIT,
			GO_NEXT  when   ST_NEXT
		;
		
		WITH I SELECT S <=
			Config(  6 downto   0) WHEN to_unsigned(0, 6),
			Config( 13 downto   7) WHEN to_unsigned(1, 6),
			Config( 20 downto  14) WHEN to_unsigned(2, 6),
			Config( 27 downto  21) WHEN to_unsigned(3, 6),
			Config( 34 downto  28) WHEN to_unsigned(4, 6),
			Config( 41 downto  35) WHEN to_unsigned(5, 6),
			Config( 48 downto  42) WHEN to_unsigned(6, 6),
			Config( 55 downto  49) WHEN to_unsigned(7, 6),
			Config( 62 downto  56) WHEN to_unsigned(8, 6),
			Config( 69 downto  63) WHEN to_unsigned(9, 6),
			Config( 76 downto  70) WHEN to_unsigned(10, 6),
			Config( 83 downto  77) WHEN to_unsigned(11, 6),
			Config( 90 downto  84) WHEN to_unsigned(12, 6),
			Config( 97 downto  91) WHEN to_unsigned(13, 6),
			Config(104 downto  98) WHEN to_unsigned(14, 6),
			Config(111 downto 105) WHEN to_unsigned(15, 6),
			Config(118 downto 112) WHEN to_unsigned(16, 6),
			Config(125 downto 119) WHEN to_unsigned(17, 6),
			Config(132 downto 126) WHEN to_unsigned(18, 6),
			Config(139 downto 133) WHEN to_unsigned(19, 6),
			Config(146 downto 140) WHEN to_unsigned(20, 6),
			Config(153 downto 147) WHEN to_unsigned(21, 6),
			Config(160 downto 154) WHEN to_unsigned(22, 6),
			Config(167 downto 161) WHEN to_unsigned(23, 6),
			Config(174 downto 168) WHEN to_unsigned(24, 6),
			Config(181 downto 175) WHEN to_unsigned(25, 6),
			Config(188 downto 182) WHEN to_unsigned(26, 6),
			Config(195 downto 189) WHEN to_unsigned(27, 6),
			Config(202 downto 196) WHEN to_unsigned(28, 6),
			Config(209 downto 203) WHEN to_unsigned(29, 6),
			Config(216 downto 210) WHEN to_unsigned(30, 6),
			Config(223 downto 217) WHEN others
			
		;

end montage;

