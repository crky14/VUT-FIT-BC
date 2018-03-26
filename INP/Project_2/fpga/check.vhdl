-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2017 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): DOPLNIT
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni z pameti (DATA_RDWR='0') / zapis do pameti (DATA_RDWR='1')
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA obsahuje stisknuty znak klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna pokud IN_VLD='1'
   IN_REQ    : out std_logic;                     -- pozadavek na vstup dat z klavesnice
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- pokud OUT_BUSY='1', LCD je zaneprazdnen, nelze zapisovat,  OUT_WE musi byt '0'
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

 -- singnaly pre prog_counter
 signal prog_reg : std_logic_vector(11 downto 0);
 signal prog_reg_inc : std_logic;
 signal prog_reg_dec : std_logic;
 
  -- singnaly pre ptr_counter
 signal ptr_reg : std_logic_vector(9 downto 0);
 signal ptr_reg_inc : std_logic;
 signal ptr_reg_dec : std_logic;
 
   -- singnaly pre ptr_counter
 signal cnt_reg : std_logic_vector(7 downto 0);
 signal cnt_reg_inc : std_logic;
 signal cnt_reg_dec : std_logic;
 
 
 type inst_type is (
			pointer_inc, -- >
			pointer_dec, -- <
			value_inc,	 -- +
			value_dec,	 -- -
			loop_left,	 -- [
			loop_right,	 -- ]
			print,		 -- .
			input,		 -- ,
			break,		 -- ~
			finish		 -- null
			         );  
 signal inst_type : inst;

 type FSA_states is (
			state_idle,
			state_fetch,
			state_decode,
			state_ptr_inc,
			state_ptr_dec,
			state_val_inc,
			state_val_dec
						  );
 signal FSA_states : current_state;
 signal FSA_states : next_state;
						
begin


prog_cnt : process(RESET, CLK)
begin
   if (RESET = '1') then
      prog_reg <= (others => '0');
   elsif (CLK'event AND CLK = '1') then
		if(prog_reg_inc = '1') then 
			prog_reg <= prog_reg + 1;
		elsif(prog_reg_dec = '1') then
			prog_reg <= prog_reg - 1;
		end if;
	end if;
end process prog_cnt;

ptr_proc : process(RESET, CLK)
begin
   if (RESET = '1') then
      ptr_reg <= (others => '0');
   elsif (CLK'event AND CLK = '1') then
		if(ptr_reg_inc = '1') then 
			ptr_reg <= ptr_reg + 1;
		elsif(ptr_reg_dec = '1') then
			ptr_reg <= ptr_reg - 1;
		end if;
	end if;
end process ptr_proc;

cnt_proc : process(RESET, CLK)
begin
   if (RESET = '1') then
      cnt_reg <= (others => '0');
   elsif (CLK'event AND CLK = '1') then
		if(cnt_reg_inc = '1') then 
			cnt_reg <= cnt_reg + 1;
		elsif(cnt_reg_dec = '1') then
			cnt_reg <= cnt_reg - 1;
		end if;
	end if;
end process cnt_proc;

decoder : process(CODE_DATA)
begin
   case CODE_DATA is
      when "0x3E" => inst <= pointer_inc;
		when "0x3C" => inst <= pointer_dec;
		when "0x2B" => inst <= value_inc;
		when "0x2D" => inst <= value_dec;
		when "0x5B" => inst <= while_left;
		when "0x5D" => inst <= while_right;
		when "0x2E" => inst <= print;
		when "0x2C" => inst <= input;
		when "0x7E" => inst <= break;
		when "0x00" => inst <= finish;
   end case;
end process decoder;


current_state_proc: process(RESET, CLK)
begin
   if (RST='1') then
      current_state <= state_idle;
   elsif (CLK'event) and (CLK='1') then
		if(EN = '1') then 
			current_state <= next_state;
		end if;
   end if;
end process current_state_proc;

next_state_proc : process(current_state, inst)
begin


	case current_state is
		------------ state_idle-------------
		when state_idle =>
			next_state <= state_fetch;
		-------------state_fetch------------
		when state_fetch =>
			next_state <= state_decode;
			
	end case;

end process next_state_proc;


end behavioral;
 