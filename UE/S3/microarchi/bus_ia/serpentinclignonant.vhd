-------------------------------------------------------------------------------
-- Ce bloc est un registre contenant la configuration a transmettre au
-- serpentin programmable, pour qu'il affiche un serpentin clignotant
-------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use ieee.numeric_std.all;

entity serpentinclignotant is
	port(
		 Config     : OUT STD_LOGIC_VECTOR(229 downto 0)
	 );
end serpentinclignotant;

architecture montage of serpentinclignotant is

	begin
		-- boucle sur 2 frames
		Config(229 downto 224) <= "000010";
		
		-- 2 frames, 1 allumé, 1 éteinte
		Config(  6 downto  0) <= "1111111";
		Config( 13 downto  7) <= "0000001";

end montage;

