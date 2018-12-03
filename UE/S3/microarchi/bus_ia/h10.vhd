
-------------------------------------------------------------------------------
-- Ce module prends un fil en entrée, 1 si c'est un tick, 0 sinon
-- Sa sortie vaut successivement 0 puis 1 tous les 10 ticks
-------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use ieee.numeric_std.all;

-- diode h10
ENTITY h10 IS
	PORT(
		    clk : in STD_LOGIC;
		    reset : in STD_LOGIC;
		    T   : in  STD_LOGIC;
		    S   : out STD_LOGIC
	    );
END h10;

ARCHITECTURE montage OF h10 IS

    -------------------------------------------------------------------------------
    --  Partie Opérative
    -------------------------------------------------------------------------------
	TYPE T_CMD IS (INIT, COUNT, NOOP);

    -- la commande courante
	signal CMD :  T_CMD; 

	 -- registre de comptagne 10 => 9 => ... => 0 => 10 => ...
	signal C : unsigned(3 downto 0);
	signal R : STD_LOGIC ;

	 -- boolean vaux 1 si C est à 0, 0 sinon
	signal C_IS_ZERO:  STD_LOGIC;

    -------------------------------------------------------------------------------
    -- Partie Contrôle
    -------------------------------------------------------------------------------
	type STATE_TYPE is (
	ST_INIT, ST_COUNT
);
signal state : STATE_TYPE;

begin

    -------------------------------------------------------------------------------
    --  Partie Opérative
    -------------------------------------------------------------------------------

	process (reset, clk)
	begin
		IF reset = '1' THEN
			C <= to_unsigned(10, 4);
			R <= '1';
		ELSIF clk'event and clk = '1' then
			IF CMD = INIT THEN
				C <= to_unsigned(10, 4);
				R <= not(R);
			ELSIF CMD = COUNT AND T = '1' THEN
				C <= C - 1;
			END IF;
		end if;
	end process;

	C_IS_ZERO <= '1' WHEN C = 0  ELSE '0' ;
	S <= R ;

    -------------------------------------------------------------------------------
    -- Partie Contrôle
    -------------------------------------------------------------------------------
    -- Inputs:  T
    -- Outputs: S, CMD
    -------------------------------------------------------------------------------

    -- fonction de transitition    
		process (reset, clk)
		begin
			if reset = '1' then
				state <= ST_INIT;
			elsif clk'event and clk = '1' then
				case state is
					when ST_INIT =>
						state <= ST_COUNT ;

					when ST_COUNT =>
						IF C_IS_ZERO = '1' THEN
							state <= ST_INIT ;
						END IF  ;

				end case;
			end if;
		end process;

	 -- fonction de sortie
		with state  select CMD <=
		INIT    when   ST_INIT,
		COUNT   when   ST_COUNT;

	end montage;

