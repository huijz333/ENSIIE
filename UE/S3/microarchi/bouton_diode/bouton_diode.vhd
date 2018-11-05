-- includes
LIBRARY IEEE;
USE IEEE.std_logic_1164.ALL;
USE IEEE.numeric_std.ALL;

ENTITY bouton_diode IS
	PORT (
		-- le bouton
		clk : IN STD_LOGIC;
		rst : IN STD_LOGIC;
		B : IN  STD_LOGIC;
		D : OUT STD_LOGIC
	);
END bouton_diode ;

ARCHITECTURE arch OF bouton_diode IS
	TYPE STATE_TYPE IS (debut, a0, a1, e0);
	SIGNAL state : STATE_TYPE ;
	
	BEGIN
		PROCESS(clk,rst)
			BEGIN
				IF rst='1' THEN 
					state <= debut ;
				ELSIF clk'EVENT AND clk='1' THEN
				
					CASE state IS
						WHEN debut =>
							IF B='1' THEN
								state <= a0 ;
							END IF;
							
						WHEN a0 =>
							IF B='0' THEN
								state <= a1 ;
							END IF;
							
						WHEN a1 =>
							IF B='1' THEN
								state <= e0 ;
							END IF;
							
						WHEN e0 =>
							IF B='0' THEN
								state <= debut ;
							END IF;
							
					END CASE ;
				END IF;
		END PROCESS ;
		
		WITH state SELECT D <=
			'0' WHEN debut,
			'1' WHEN a0,
			'1' WHEN a1,
			'0' WHEN OTHERS
		;
		
		
END arch;
