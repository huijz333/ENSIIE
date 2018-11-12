
-------------------------------------------------------------------------------
-- Ce module prends un entier 24 bits E en entrée
--	Puis, sa sortie est à 0 pendant E cycle consécutifs, puis passe à 1 pour un cycle
-- Ensuite, E est rechargé
-------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use ieee.numeric_std.all;

entity h100 is
    port(
        clk : in STD_LOGIC;
		  reset : in STD_LOGIC;
        E   : in  STD_LOGIC_VECTOR(23 downto 0);
        T   : out STD_LOGIC
	 );
end h100;

architecture montage of h100 is

-------------------------------------------------------------------------------
--  Partie Opérative
-------------------------------------------------------------------------------
    type T_CMD is (LOAD, DECR, NOOP);
	 -- la commande courante
    signal CMD :  T_CMD; 
	 -- le registre de stockage, compteur des clocks
    signal R   :  unsigned (23 downto 0);
	 -- boolean vrai si R est à 0
	 signal R_IS_NULL:  STD_LOGIC;
	
-------------------------------------------------------------------------------
-- Partie Contrôle
-------------------------------------------------------------------------------
    type STATE_TYPE is (
        ST_LOAD, ST_DECR, ST_TICK
	 );
    signal state : STATE_TYPE;
    
begin

-------------------------------------------------------------------------------
--  Partie Opérative
-------------------------------------------------------------------------------

    process (clk)
    begin if clk'event and clk = '1' then
	     IF CMD = LOAD THEN 
		      R <= x"0F4240";
		  ELSIF CMD = DECR THEN
				R <= R - 1;
		  END IF;
    end if; end process;
	 
	 R_IS_NULL <= '1' WHEN R = x"000000" ELSE '0' ;

-------------------------------------------------------------------------------
-- Partie Contrôle
-------------------------------------------------------------------------------
-- Inputs:  E
-- Outputs: T, CMD
-------------------------------------------------------------------------------

    -- fonction de transitition    
    process (reset, clk)
    begin
	      if reset = '1' then
			    state <= ST_LOAD;
			elsif clk'event and clk = '1' then
            case state is
                when ST_LOAD =>
					     state <= ST_DECR ;
				    when ST_DECR =>
						  IF R_IS_NULL = '1' THEN
						      state <= ST_TICK;
						  END IF ;
					 when ST_TICK =>
					     state <= ST_LOAD ;
            end case;
        end if;
    end process;

    -- fonction de sortie    
    with state  select T <=
         '1'    when   ST_TICK,
         '0'    when   others;

	 with state  select CMD <=
         LOAD   when   ST_LOAD,
         DECR   when   ST_DECR,
         NOOP   when   ST_TICK;
			
end montage;

