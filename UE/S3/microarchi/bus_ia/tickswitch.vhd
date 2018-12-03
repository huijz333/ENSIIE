-------------------------------------------------------------------------------
-- Ce module prends un fil en entrée, 1 si c'est un tick, 0 sinon
-- Sa sortie change entre '0' et '1' à chaque tick
-------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use ieee.numeric_std.all;

ENTITY tickswitch IS
	PORT(
		    clk : in STD_LOGIC;
		    reset : in STD_LOGIC;
		    T   : in  STD_LOGIC;
		    S   : out STD_LOGIC
	    );
END tickswitch;

ARCHITECTURE montage OF tickswitch IS

    -------------------------------------------------------------------------------
    --  Partie Opérative
    -------------------------------------------------------------------------------
	TYPE T_CMD IS (INIT, SWITCH, NOOP);

    -- la commande courante
	signal CMD :  T_CMD; 

	 -- registre de sortie
	signal R : STD_LOGIC ;

    -------------------------------------------------------------------------------
    -- Partie Contrôle
    -------------------------------------------------------------------------------
	type STATE_TYPE is (
		ST_INIT, ST_SWITCH, ST_WAIT
	);
	
	signal state : STATE_TYPE;

begin

    -------------------------------------------------------------------------------
    --  Partie Opérative
    -------------------------------------------------------------------------------

	process (reset, clk)
	begin
		IF reset = '1' THEN
			R <= '1';
		ELSIF clk'event and clk = '1' then
			IF CMD = INIT THEN
				R <= '0';
			ELSIF CMD = SWITCH THEN
				R <= not(R);
			END IF;
		end if;
	end process;

	-- la sortie == le registre de stockage
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
						state <= ST_WAIT ;

					when ST_WAIT =>
						IF T = '1' THEN
							state <= ST_SWITCH ;
						END IF  ;
						
					when ST_SWITCH =>
						state <= ST_WAIT;

				end case;
			end if;
	end process;

	-- fonction de sortie
	with state select CMD <=
		INIT    when   ST_INIT,
		SWITCH  when   ST_SWITCH,
		NOOP    when   ST_WAIT
	;

end montage;
