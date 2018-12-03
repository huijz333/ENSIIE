
-------------------------------------------------------------------------------
-- Ce module prends un fil en entrée, 1 si c'est un tick, 0 sinon
-- Sa sortie vaut successivement 0 puis 1 tous les 10 ticks
-------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use ieee.numeric_std.all;

-- diode h10
ENTITY serpentinmux IS
	PORT(
		-- interrupteur de selection
		 E1    : IN STD_LOGIC;
		 E0    : IN STD_LOGIC;
		 
		 -- les 4 configurations possibles
		 Config3 : IN STD_LOGIC_VECTOR(229 downto 0);
		 Config2 : IN STD_LOGIC_VECTOR(229 downto 0);
		 Config1 : IN STD_LOGIC_VECTOR(229 downto 0);
		 Config0 : IN STD_LOGIC_VECTOR(229 downto 0);
			
		 -- la config selectionné
		 SelectedConfig : OUT STD_LOGIC_VECTOR(229 downto 0)
	);
END serpentinmux;

ARCHITECTURE montage OF serpentinmux IS

   -------------------------------------------------------------------------------
   --  Partie Opérative
   -------------------------------------------------------------------------------
	signal E : STD_LOGIC_VECTOR(1 downto 0);
	
	
	begin
	
		E(1) <= E1;
		E(0) <= E0;

		-------------------------------------------------------------------------------
		-- Partie Contrôle
		-------------------------------------------------------------------------------
		-- Inputs:  T
		-- Outputs: S, CMD
		-------------------------------------------------------------------------------

		-- fonction de sortie    
		WITH E SELECT SelectedConfig <=
			Config0 WHEN "00",
			Config1 WHEN "01",
			Config2 WHEN "10",
			Config3 WHEN "11"
		;


end montage;

