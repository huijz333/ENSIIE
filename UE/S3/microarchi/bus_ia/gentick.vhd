-------------------------------------------------------------------------------
-- Ce module transfert tous les messages (????,addrsrc,addrdest,data) venant de
-- busin.
--
-- data est stocké dans un registre
--
-- Si addrdest==MYADDR, data est transmis sur busv
-- Sinon, tout le message est transféré sur busout
--
-- Du coté busin, il suit le protocole "poignée de main" (signaux: busin, 
--   busin_valid, busin_eated).
-- Du coté busout, il suit le protocole "poignée de main" (signaux: busout, 
--   busout_valid, busout_eated).
-- Du coté busv, la valeur est transmise sans check
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
        -- interface V
        busv : out STD_LOGIC_VECTOR(23 downto 0)
	 );
end gentick;

architecture montage of gentick is

-------------------------------------------------------------------------------
--  Partie Opérative
-------------------------------------------------------------------------------
    -- Registre de transfert entre busin et busout
    type T_CMD_tft is (INIT, NOOP);
    signal CMD_tft :  T_CMD_tft ; 
    signal CMD_v :  T_CMD_tft ; 
    signal R_tft   :  STD_LOGIC_VECTOR (43 downto 0);
    signal R_V       :  STD_LOGIC_VECTOR (23 downto 0);

-------------------------------------------------------------------------------
-- Partie Contrôle
-------------------------------------------------------------------------------
    -- adresse destination & bit de boucle du message lu
    alias  busin_addrdest : STD_LOGIC_VECTOR(7 downto 0) is busin(31 downto 24);

    type STATE_TYPE is (
        ST_READ_BUSIN, ST_WRITE_OUT, ST_WRITE_V
	 );
    signal state : STATE_TYPE;
    
begin

-------------------------------------------------------------------------------
--  Partie Opérative
-------------------------------------------------------------------------------

    process (reset, clk)
    begin
	   if reset = '1' then
		   R_V <= x"17A120";
	   elsif clk'event and clk = '1' then
	     IF CMD_tft = INIT THEN 
		      R_tft <= busin ;
		  END IF;
		  IF CMD_v = INIT THEN
		      --R_V <= R_tft(23 downto 0);
		  END IF ;
    end if; end process;

    busout <= R_tft;
    busv <= R_V;

-------------------------------------------------------------------------------
-- Partie Contrôle
-------------------------------------------------------------------------------
-- Inputs:  busin_valid, busout_eated, busin_addrdest
-- Outputs: busin_eated, busout_valid, CMD_tft
-------------------------------------------------------------------------------

    -- fonction de transitition    
    process (reset,clk)
    begin
        if reset = '1' then
            state <= ST_READ_BUSIN;
        elsif clk'event and clk = '1' then
            case state is
                when ST_READ_BUSIN =>
                    IF busin_valid='1' THEN
						      IF busin_addrdest = MYADDR THEN
								    state <= ST_WRITE_V ;
							   ELSE
								    state <= ST_WRITE_OUT ;
								END IF ;
						  END IF ;

				    when ST_WRITE_OUT =>
					     IF busout_eated = '1' THEN
						      state <= ST_READ_BUSIN;
						  END IF ;
						  
					 when ST_WRITE_V =>
					    state <= ST_READ_BUSIN;
						  
            end case;
        end if;
    end process;

    -- fonction de sortie    
    with state  select busin_eated <=
         '1'    when   ST_READ_BUSIN,
         '0'    when   others; 

    with state  select busout_valid <=
         '1'    when    ST_WRITE_OUT,
         '0'    when   others; 

    with state  select CMD_tft <=
         INIT   when   ST_READ_BUSIN,
         NOOP   when   others; 

	 with state  select CMD_v <=
         INIT   when   ST_WRITE_V,
         NOOP   when   others; 
			
end montage;

