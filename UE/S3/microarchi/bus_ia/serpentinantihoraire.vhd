-------------------------------------------------------------------------------
-- Ce bloc est un registre contenant la configuration a transmettre au
-- serpentin programmable, pour qu'il affiche un serpentin tournant dans
-- le sens anti-horaire.
-------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use ieee.numeric_std.all;

entity serpentinantihoraire is
	port(
		 Config : OUT STD_LOGIC_VECTOR(229 downto 0)
	 );
end serpentinantihoraire;

architecture montage of serpentinantihoraire is

	begin
	-- boucle sur 6 frames
		Config(229 downto 224) <= "000110";
			
		-- les frames
		Config(  6 downto  0)  <= "0011101";
		Config( 13 downto  7)  <= "0111001";
		Config( 20 downto  14) <= "1110001";
		Config( 27 downto  21) <= "1100011";
		Config( 34 downto  28) <= "1000111";
		Config( 41 downto  35) <= "0001111";
		
end montage;

