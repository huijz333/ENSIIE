-- includes
LIBRARY IEEE;
USE IEEE.std_logic_1164.ALL;
USE IEEE.numeric_std.ALL;

-- prototypage
ENTITY tp1 IS
	PORT (
		A, B, C    : IN  STD_LOGIC;
		S0, S1, S2 : OUT STD_LOGIC
	);
END tp1;

-- architecture
ARCHITECTURE arch OF tp1 IS
	BEGIN
		S0 <= not(( A     and not(B and C))  or  (not(A) and  B and C ));
		S1 <= not((not(A) and not(B xor C))  or  (A      and (B xor C)));
		S2 <= S0 AND S1;
END arch ;