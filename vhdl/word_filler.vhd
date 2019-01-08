library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;

entity word_filler is
  port(
    clk             : in  std_logic;
    rst             : in  std_logic;
    byte_wr_en      : in  std_logic;
    byte_in         : in  std_logic_vector(7 downto 0);
    word_out        : out std_logic_vector(23 downto 0);
    word_wr_en      : out std_logic
  );
end word_filler;



architecture BHV of word_filler is

  begin

  process(clk,rst)
    variable byte_count : unsigned(3 downto 0) := 0;
  begin

    if(rst = '1') then

    elsif(rising_edge(clk))then
      word_wr_en <= '0';

      if(byte_wr_en = '1') then
        word_out((23-(8*byte_count)) downto (8*(2-byte_count))) <= byte_in;
        byte_count := byte_count +1;
      end if;

      if(byte_count = 3) then
        byte_count := 0;
        word_wr_en <= '1';

      end if;

    end if;

  end process;

end BHV;
