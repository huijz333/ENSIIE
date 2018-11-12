-- includes
LIBRARY IEEE;
USE IEEE.std_logic_1164.ALL;
USE IEEE.numeric_std.ALL;

-- prototypage
ENTITY tp2 IS
	PORT (
		e : IN  STD_LOGIC_VECTOR (3 downto 0) ; -- 2^4 = 16, codes les entiers de 0 à 9
		s : OUT STD_LOGIC_VECTOR (6 downto 0)   -- 1 bit par segment (abcdefg)_2, bit de poids faible 'à droite'
	);
END tp2;

-- architecture
ARCHITECTURE arch OF tp2 IS
	BEGIN
		WITH e SELECT s <=
			"1111110" WHEN "0000", -- 0 => abcdef-
			"0110000" WHEN "0001", -- 1 => -bc----
			"1101101" WHEN "0010",
			"1111001" WHEN "0011",
			"0110011" WHEN "0100",
			"1011011" WHEN "0101",
			"1011111" WHEN "0110",
			"1110010" WHEN "0111",
			"1111111" WHEN "1000",
			"1111011" WHEN "1001",
			"1001111" WHEN "----"  -- >= 10 => affiches 'E'
		;
END arch ;