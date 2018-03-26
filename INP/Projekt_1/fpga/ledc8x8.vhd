library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port ( 
		ROW		:	out std_logic_vector(0 to 7);
		LED		: 	out std_logic_vector(0 to 7);
		SMCLK	:	in std_logic;
		RESET	:	in std_logic
);
end ledc8x8;

architecture main of ledc8x8 is
		signal cnt:		std_logic_vector(7 downto 0);
		signal shift: 	std_logic;
		signal rows:	std_logic_vector(7 downto 0);
		signal cols:	std_logic_vector(7 downto 0);	
begin
	
	-- shift nadobuda log 1 ak counter narata 255 inak log 0
	shift <= '1' when cnt = "11111111" else '0'; -- shifter sa nastavi na log 1 ak citac zaznamena SMCLK 256x
	-- citac na spomalenie SMCLK
	counter: process(RESET, SMCLK)
	begin
			if(RESET = '1')then	
					cnt <= "00000000";					-- asynchronny reset pomocou RESET
			elsif(SMCLK'event and SMCLK = '1')then	-- spustanie counteru hodinovym signalom SMCLK
					if(shift = '0')then
						cnt <= cnt + 1;					-- counter do 255
					else
						cnt <= "00000000";				-- vynulovanie na 0 
					end if;
			end if;
	end process counter;
	
	--otocny register
	rotate: process(RESET, SMCLK)
	begin
			if(RESET = '1')then	
					rows <= "10000000";   				-- asynchronny reset pomocou RESET
			elsif(SMCLK'event and SMCLK = '1' and shift = '1')then			-- posuv riadku ak shift ma hodnotu log 1
					rows <= rows(0) & rows(7 downto 1);								-- tocenie registru pomocou konkatenacie
			end if;
	end process rotate;
	
	-- dekoder pre rozsvecovanie lediek
	dekoder: process(rows)
	begin
			case rows is
				when "10000000" => cols <= "00001111";
				when "01000000" => cols <= "11101111";
				when "00100000" => cols <= "11101111";
				when "00010000" => cols <= "01101001";
				when "00001000" => cols <= "10010110";
				when "00000100" => cols <= "11110111";
				when "00000010" => cols <= "11110110";
				when "00000001" => cols <= "11111001";
				when others => cols <= "11111111";
			end case;
	end process dekoder;

	ROW <= rows;
	LED <= cols;

end main;