
-------------------------------------------------------------------------------
--	Sortie : la configuration d'un serpentin clignotant
-------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use ieee.numeric_std.all;

entity serpentinclignotant is
	port(
		-- la clock
		 clk    : in STD_LOGIC;

		-- sorties vers le 7-segment
		 Config     : OUT STD_LOGIC_VECTOR(229 downto 0)
	 );
end serpentinclignotant;

architecture montage of serpentinclignotant is

begin

 -- fonction de transitition    
	PROCESS (clk)
	begin
		IF clk'event and clk = '1' THEN
			-- boucle sur 2 frames
			Config(229 downto 224) <= "000010";
			
			-- 2 frames, 1 allumé, 1 éteinte
			Config(  6 downto  0) <= "1111111";
			Config( 13 downto  7) <= "0000001";
			Config(223 downto 14) <= (others => '0');
			
		END IF;
	END PROCESS;
end montage;

