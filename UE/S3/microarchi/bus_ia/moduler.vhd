
-------------------------------------------------------------------------------
--	Entrée
--		clk, reset : la clock et le reset
--		T : un tick ('1' ou '0')
--		E : un entier entre 0 et 7, controllant la vitesse du tick 'S'
--
--	Sortie
--		S : un tick plus ou moins lent que 'T' en fonction de 'E'
--
-------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use ieee.numeric_std.all;

-- moduler
entity moduler is
	port(
		    clk : in STD_LOGIC;
		    reset : in STD_LOGIC;
		    T     : in  STD_LOGIC;
		    E2    : in STD_LOGIC;
		    E1    : in STD_LOGIC;
		    E0    : in STD_LOGIC;
		    S     : out STD_LOGIC
	    );
end moduler;

architecture montage of moduler is

    -------------------------------------------------------------------------------
    --  Partie Opérative
    -------------------------------------------------------------------------------
	type T_CMD is (INIT, COUNT);

    -- la commande courante
	signal CMD :  T_CMD; 

	 -- registre de stockage de la valeur du compteur à charger
	signal V_E : unsigned(7 downto 0);

	 -- registre de stockage des 3 bits en un vecteur
	signal E : STD_LOGIC_VECTOR(2 downto 0);
	
	-- registre de comptagne V_E => V_E - 1 => ... 0 => V_E => ...
	signal C : unsigned(7 downto 0);
	
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

	process (reset, clk, V_E)
		begin
			IF reset = '1' THEN
				C <= V_E;
			ELSIF clk'event and clk = '1' then
				IF CMD = INIT THEN
					C <= V_E;
				ELSIF CMD = COUNT AND T = '1' THEN
					C <= C - 1;
				END IF;
			end if;
	end process;

	C_IS_ZERO <= '1' WHEN C = 0  ELSE '0' ;

	E(0) <= E0;
	E(1) <= E1;
	E(2) <= E2;

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
			COUNT   when   ST_COUNT
		;

		with E select V_E <=
			to_unsigned( 2, 8) when "000",
			to_unsigned( 5, 8) when "001",
			to_unsigned( 7, 8) when "010",
			to_unsigned(10, 8) when "011",
			to_unsigned(12, 8) when "100",
			to_unsigned(15, 8) when "101",
			to_unsigned(17, 8) when "110",
			to_unsigned(20, 8) when "111"
		;
		
		S <= C_IS_ZERO;

	end montage;

