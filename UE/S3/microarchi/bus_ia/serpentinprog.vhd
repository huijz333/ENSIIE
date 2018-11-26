
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

entity serpentinprog is
	port(
		-- entrées
		 clk    : in STD_LOGIC;
		 reset  : in STD_LOGIC;
		 Config : in STD_LOGIC_VECTOR(229 downto 0);
		 T      : in  STD_LOGIC;

		-- sorties vers le 7-segment
		 S0     : OUT STD_LOGIC;
		 S1     : OUT STD_LOGIC;
		 S2     : OUT STD_LOGIC;
		 S3     : OUT STD_LOGIC;
		 S4     : OUT STD_LOGIC;
		 S5     : OUT STD_LOGIC;
		 S6     : OUT STD_LOGIC
	 );
end serpentinprog;

architecture montage of serpentinprog is

    -------------------------------------------------------------------------------
    --  Partie Opérative
    -------------------------------------------------------------------------------
	type T_CMD is (INIT, COUNT, NOOP);

    -- la commande courante
	signal CMD :  T_CMD;

	 -- N : nombre de valeur à faire défiler en boucle par le 7-segment
	signal R_N : unsigned(7 downto 0);

	 -- i : valeur du segment courant modulo N
	signal R_i : unsigned(7 downto 0);

	 -- R_S : tableau coddant les valeurs des 7 segments
	type SERP_ARRAY_T is array (0 to 31) of STD_LOGIC_VECTOR(6 downto 0);
	signal R_S : SERP_ARRAY_T;

    -------------------------------------------------------------------------------
    -- Partie Contrôle
    -------------------------------------------------------------------------------
	type STATE_TYPE is (
	ST_INIT
);
signal state : STATE_TYPE;

begin

	-------------------------------------------------------------------------------
	--  Partie Opérative
	-------------------------------------------------------------------------------

	process (reset, clk)
	begin
		IF reset = '1' THEN -- au reset, on remet tous les segments programmés à 0 (éteinds)
			for I in 0 to 31 loop
				R_S(I) <= "00000000";
			end loop;
		ELSIF clk'event and clk = '1' then
			IF CMD = INIT THEN
				-- TODO
				END IF;
			END IF;
		END process;

	-------------------------------------------------------------------------------
	-- Partie Contrôle
	-------------------------------------------------------------------------------
	-- Inputs:  T
	-- Outputs: S, CMD
	-------------------------------------------------------------------------------

	-- fonction de transitition    
		process (reset, clk)
		begin
			if clk'event and clk = '1' then
			-- TODO : switch states
				end if;
			end process;

	-- fonction de sortie
			with state  select CMD <=
			INIT    when   ST_INIT,
			INIT    when   OTHERS; --TODO

		end montage;

