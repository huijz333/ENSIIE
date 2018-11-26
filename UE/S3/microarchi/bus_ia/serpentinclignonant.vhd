
-------------------------------------------------------------------------------
--	Entree:
--		clk, reset: la clock et le reset
--		T : un tick
--
-- Sortie:
--		Si : 	'1' ou '0' selon que le i-ème segment du 7-segment doit
--				etre allumé ou éteinds
--
-------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use ieee.numeric_std.all;

entity serpentinclignotant is
	port(
		-- sorties vers le 7-segment
		 Config     : OUT STD_LOGIC_VECTOR(229 downto 0)
	 );
end serpentinclignotant;

architecture montage of serpentinclignotant is

begin

	-- boucle sur 2 frames
	Config(229 downto 224) <= "000010";
	
	-- 2 frames, 1 allumé, 1 éteinte
	Config(  6 downto  0) <= "1111111";
	Config(223 downto  7) <= (others => '0');

end montage;

