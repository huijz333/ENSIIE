-------------------------------------------------------------------------------
-- Ce module transfert tous les messages (????,addrsrc,addrdest,data) venant de
-- busin.
-- Si addrdest = MYADDR, alors 'data' est chargé dans un registre (24bits)
-- Le registre est envoyé en sortie sur 'Vs'
-- Le message entier est transféré sur busout
--
-- Du coté busin, il suit le protocole "poignée de main" (signaux: busin, 
--   busin_valid, busin_eated).
-- Du coté busout, il suit le protocole "poignée de main" (signaux: busout, 
--   busout_valid, busout_eated).
-- Du coté Vs, le registre est transféré (24 bits)
-------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;

entity gentick is
	generic(
		MYADDR : STD_LOGIC_VECTOR(7 downto 0) :=  "00001010" -- 10
    );
    port(
        clk          : in  STD_LOGIC;
        reset        : in  STD_LOGIC;
        -- interface busin
        busin        : in  STD_LOGIC_VECTOR(43 downto 0);
        busin_valid  : in  STD_LOGIC;
        busin_eated  : out STD_LOGIC; 
        -- interface busout
        busout       : out STD_LOGIC_VECTOR(43 downto 0);
        busout_valid : out STD_LOGIC;
        busout_eated : in  STD_LOGIC; 
        -- interface v (nombre de ticks)
        busv         : out STD_LOGIC_VECTOR(23 downto 0)
	  );
end gentick;

architecture montage of gentick is

-------------------------------------------------------------------------------
--  Partie Opérative
-------------------------------------------------------------------------------
    -- Registre de transfert entre busin et busout
    type T_CMD_tft is (LOAD, NOOP);
    signal CMD_tft :  T_CMD_tft ; 
    signal R_tft   :  STD_LOGIC_VECTOR (43 downto 0);
    signal R_V     :  STD_LOGIC_VECTOR (23 downto 0);

-------------------------------------------------------------------------------
-- Partie Contrôle
-------------------------------------------------------------------------------
    -- adresse destination & bit de boucle du message lu
    alias  busin_addrdest : STD_LOGIC_VECTOR(7 downto 0) is busin(31 downto 24);

    type STATE_TYPE is (
        ST_LOAD, ST_WAIT
	 );
    signal state : STATE_TYPE;
    
begin

-------------------------------------------------------------------------------
--  Partie Opérative
-------------------------------------------------------------------------------

    process (clk)
    begin if clk'event and clk = '1' then
	     -- enregistre le message 44 bits busin
		  R_tft <= busin ;
		  -- si le message nous est destiné, on l'enregistre
		  IF CMD_tft = LOAD THEN 
		      R_V <= busin(23 downto 0);
		  END IF;
    end if; end process;

	 -- on transmet le message
    busout <= R_tft ;
	 -- on transmet le nombre de tick
	 busv   <= R_V;

-------------------------------------------------------------------------------
-- Partie Contrôle
-------------------------------------------------------------------------------
-- Inputs:  busin_valid, busout_eated, busdump_eated, busin_addrdest
-- Outputs: busin_eated, busout_valid, busdump_valid, CMD_tft
-------------------------------------------------------------------------------

    -- fonction de transitition    
    process (reset,clk)
    begin
        if reset = '1' then
            state <= ST_WAIT;
        elsif clk'event and clk = '1' then
            case state is
                when ST_WAIT =>
                    IF busin_valid='1' AND busin_addrdest = MYADDR THEN
								 state <= ST_LOAD ;
						  END IF ;

				    when ST_LOAD =>
					     state <= ST_WAIT ;

            end case;
        end if;
    end process;

    -- fonction de sortie    
    with state  select busin_eated <=
         '1'    when   ST_LOAD,
         '0'    when   others; 

    with state  select busout_valid <=
         '1'    when    ST_WAIT,
         '0'    when   others; 

    with state  select CMD_tft <=
         LOAD   when   ST_LOAD,
         NOOP   when   others; 

end montage;

