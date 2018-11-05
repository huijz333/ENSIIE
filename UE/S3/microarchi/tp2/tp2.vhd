-- includes
LIBRARY IEEE;
USE IEEE.std_logic_1164.ALL;
USE IEEE.numeric_std.ALL;

-- prototypage
ENTITY tp2 IS
	PORT (
		e   : IN  STD_LOGIC_VECTOR (3 downto 0) ; -- 2^4 = 16, codes les entiers de 0 à 9
		s   : OUT STD_LOGIC_VECTOR (6 downto 0) ; -- 1 bit par segment (abcdefg)_2, bit de poids faible 'à droite'
		err : OUT STD_LOGIC
	);
END tp2;

-- architecture
ARCHITECTURE arch OF tp2 IS
	BEGIN
		WITH e SELECT s <=
			"0000001" WHEN "0000", -- 0 => abcdef-
			"1001111" WHEN "0001", -- 1 => -bc----
			"0010010" WHEN "0010",
			"0000110" WHEN "0011",
			"1001100" WHEN "0100",
			"0100100" WHEN "0101",
			"0100000" WHEN "0110",
			"0001101" WHEN "0111",
			"0000000" WHEN "1000",
			"0000100" WHEN "1001",
			"0110000" WHEN OTHERS  -- >= 10 => affiches 'E'
		;
		
		WITH e SELECT err <=
			'0' WHEN "0000", -- diode eteinte
			'0' WHEN "0001", -- diode eteinte
			'0' WHEN "0010",
			'0' WHEN "0011",
			'0' WHEN "0100",
			'0' WHEN "0101",
			'0' WHEN "0110",
			'0' WHEN "0111",
			'0' WHEN "1000",
			'0' WHEN "1001",
			'1' WHEN OTHERS  -- diode allumée (>= 10)
		;

END arch ;