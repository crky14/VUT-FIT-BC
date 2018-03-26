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
 
   -- singnaly pre cnt_counter
 signal cnt_reg : std_logic_vector(7 downto 0);
 signal cnt_reg_inc : std_logic;
 signal cnt_reg_dec : std_logic;
 
 
 type instruction_type is (
         pointer_inc, -- >
         pointer_dec, -- <
         value_inc,   -- +
         value_dec,   -- -
         loop_left,   -- [
         loop_right,  -- ]
         print,       -- .
         input,       -- ,
         break,       -- ~
         finish,       -- null
	      skip
                  );  
 signal instruction : instruction_type;

 type FSA_states is (
         state_idle,
         state_fetch,
         state_decode,
         state_ptr_inc,
         state_ptr_dec,
         state_val_inc,
         state_val_inc_read,
         state_val_dec,
         state_val_dec_read,
         state_loop_left_read,
         state_loop_left_compare,
         state_loop_left_fetch,
         state_loop_left_decode,
         state_loop_left_jump,
         state_loop_right_read,
         state_loop_right_compare,
         state_loop_right_fetch,
         state_loop_right_decode,
         state_loop_right_jump,
         state_print_read,
         state_print,
         state_input,
         state_break_read,
         state_break_fetch,
         state_break_decode,
         state_break_jump,
         state_finish,
	      state_skip
                    );
 signal current_state : FSA_states;
 signal next_state : FSA_states;

 signal mx : std_logic_vector(1 downto 0);
                  
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
      when X"3E" => instruction <= pointer_inc;
      when X"3C" => instruction <= pointer_dec;
      when X"2B" => instruction <= value_inc;
      when X"2D" => instruction <= value_dec;
      when X"5B" => instruction <= loop_left;
      when X"5D" => instruction <= loop_right;
      when X"2E" => instruction <= print;
      when X"2C" => instruction <= input;
      when X"7E" => instruction <= break;
      when X"00" => instruction <= finish;
      when others => instruction <= skip;
   end case;
end process decoder;

multiplexor : process(CLK, mx, DATA_RDATA, IN_DATA) 
begin 
   case mx is 
      when "11" => DATA_WDATA <= IN_DATA;
      when "10" => DATA_WDATA <= DATA_RDATA + 1;
      when "01" => DATA_WDATA <= DATA_RDATA - 1;
		when others => DATA_WDATA <= X"00";
   end case;
end process multiplexor;


current_state_proc: process(RESET, CLK)
begin
   if (RESET='1') then
      current_state <= state_idle;
   elsif (CLK'event) and (CLK='1') then
      if(EN = '1') then 
         current_state <= next_state;
      end if;
   end if;
end process current_state_proc;

next_state_proc : process(current_state, instruction, IN_VLD, DATA_RDATA, OUT_BUSY)
begin
   
   CODE_EN <= '0';
   DATA_EN <= '0';
   prog_reg_inc <= '0';
   prog_reg_dec <= '0';
   ptr_reg_inc <= '0';
   ptr_reg_dec <= '0';
   cnt_reg_inc <= '0';
   cnt_reg_dec <= '0';
   mx <= "00";
   OUT_WE <= '0';
   IN_REQ <= '0';

   case current_state is
      ------------ state_idle-------------
      when state_idle =>
         next_state <= state_fetch;

      -------------state_fetch------------
      when state_fetch =>
         next_state <= state_decode;
         CODE_ADDR <= prog_reg;
         CODE_EN <= '1';

      -------------state_decode-----------
      when state_decode =>
         case instruction is 
            when  pointer_inc => next_state <= state_ptr_inc;
            when  pointer_dec => next_state <= state_ptr_dec;
            when  value_inc => next_state <= state_val_inc_read;
            when  value_dec => next_state <= state_val_dec_read;  
            when  loop_left => next_state <= state_loop_left_read;
            when  loop_right => next_state <= state_loop_right_read;
            when  print => next_state <= state_print_read;
            when  input => next_state <= state_input;
            when  break => next_state <= state_break_read; 
            when  finish => next_state <= state_finish;
				when  skip => next_state <= state_skip;  	
         end case;

      -------------state_ptr_inc----------
      when state_ptr_inc =>
         next_state <= state_fetch;
         ptr_reg_inc <= '1';
         prog_reg_inc <= '1';


      -------------state_ptr_dec----------
      when state_ptr_dec =>
         next_state <= state_fetch;
         ptr_reg_dec <= '1';
         prog_reg_inc <= '1';

      -------------state_val_inc_read-----

      when state_val_inc_read =>
         next_state <= state_val_inc;
         DATA_ADDR <= ptr_reg;
         DATA_RDWR <= '0';
         DATA_EN <= '1';

       -------------state_val_inc---------

      when state_val_inc =>
         next_state <= state_fetch;
         mx <= "10";
         DATA_RDWR <= '1';
         DATA_EN <= '1';
         prog_reg_inc <= '1';

      -------------state_val_dec_read-----

      when state_val_dec_read =>
         next_state <= state_val_dec;
         DATA_ADDR <= ptr_reg;
         DATA_RDWR <= '0';
         DATA_EN <= '1';

       -------------state_val_dec---------

      when state_val_dec =>
         next_state <= state_fetch;
         mx <= "01";
         DATA_RDWR <= '1';
         DATA_EN <= '1';
         prog_reg_inc <= '1';

       -------------state_print_read------

      when state_print_read => 
         next_state <= state_print;
         DATA_ADDR <= ptr_reg;
         DATA_RDWR <= '0';
         DATA_EN <= '1';

       -------------state_print-----------

      when state_print => 
         next_state <= state_print;
         if(OUT_BUSY = '0') then
            next_state <= state_fetch;
            OUT_DATA <= DATA_RDATA;
            OUT_WE <= '1';
            prog_reg_inc <= '1';
         end if;

       -------------state_input-----------

      when state_input => 
         next_state <= state_input;
         IN_REQ <= '1';
         if(IN_VLD = '1') then
            next_state <= state_fetch;
            mx <= "11";
            DATA_ADDR <= ptr_reg;
            DATA_RDWR <= '1';
            DATA_EN <= '1';
            prog_reg_inc <= '1';
         end if;

      -------------state_loop_left_read----------------------------------  nacita hodnotu aktulanej budnky a prejde na stav porovnania skoku
      when state_loop_left_read =>
         next_state <= state_loop_left_compare;    
         DATA_ADDR <= ptr_reg;
         DATA_RDWR <= '0';
         DATA_EN <= '1';  
         prog_reg_inc <= '1';

      when state_loop_left_compare =>
         next_state <= state_fetch;
         if(DATA_RDATA = X"00") then
            next_state <= state_loop_left_fetch;
            cnt_reg_inc <= '1';
         end if;

      when state_loop_left_fetch =>
         next_state <= state_loop_left_decode;
         CODE_ADDR <= prog_reg;
         CODE_EN <= '1';

      when state_loop_left_decode =>
         next_state <= state_loop_left_jump;
         if(instruction = loop_left) then
            cnt_reg_inc <= '1';
         elsif(instruction = loop_right) then
            cnt_reg_dec <= '1';
         end if;

      when state_loop_left_jump =>
         prog_reg_inc <= '1';
         next_state <= state_loop_left_fetch;
         if(cnt_reg = X"00") then
            next_state <= state_fetch;
         end if;
      -------------------state_loop_right_read-------------------------
      when state_loop_right_read =>
         next_state <= state_loop_right_compare;    
         DATA_ADDR <= ptr_reg;
         DATA_RDWR <= '0';
         DATA_EN <= '1';

      when state_loop_right_compare =>
         if(DATA_RDATA = X"00") then
            next_state <= state_fetch;
            prog_reg_inc <= '1';
         else
            next_state <= state_loop_right_fetch;
            cnt_reg_inc <= '1';
            prog_reg_dec <= '1';
         end if;

      when state_loop_right_fetch =>
         next_state <= state_loop_right_decode;
         CODE_ADDR <= prog_reg;
         CODE_EN <= '1';

      when state_loop_right_decode =>
         next_state <= state_loop_right_jump;
         if(instruction = loop_right) then
            cnt_reg_inc <= '1';
         elsif(instruction = loop_left) then
            cnt_reg_dec <= '1';
         end if;

      when state_loop_right_jump =>
         if(cnt_reg = X"00") then
            next_state <= state_fetch;
            prog_reg_inc <= '1';
         else
            next_state <= state_loop_right_fetch;
            prog_reg_dec <= '1';
         end if;

      -----------------break--------------------------------

      when state_break_read =>
         next_state <= state_break_fetch;
         cnt_reg_inc <= '1';
         prog_reg_dec <= '1';

      when state_break_fetch =>
         next_state <= state_break_decode;
         CODE_ADDR <= prog_reg;
         CODE_EN <= '1';

      when state_break_decode =>
         next_state <= state_break_jump;
         if(instruction = loop_left) then
            cnt_reg_inc <= '1';
         elsif(instruction = loop_right) then
            cnt_reg_dec <= '1';
         end if;

      when state_break_jump =>
         prog_reg_inc <= '1';
         next_state <= state_break_fetch;
         if(cnt_reg = X"00") then
            next_state <= state_fetch;
         end if;

      when state_finish =>
         next_state <= state_finish;

      when state_skip => 
         next_state <= state_fetch;
         prog_reg_inc <= '1';

   end case;

end process next_state_proc;


end behavioral;
 