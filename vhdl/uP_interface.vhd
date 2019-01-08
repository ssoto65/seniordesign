library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;

entity uP_interface is
  port(
    clk_in        : in std_logic;
    wr_en         : in std_logic;
    ale1          : in std_logic;
    rst           : in std_logic;
    uP_addr       : in std_logic_vector(7 downto 0);
    uP_data       : in std_logic_vector(7 downto 0);
    clk_out       : in std_logic;
    matrix_addr   : out std_logic_vector(15 downto 0);
    matrix_data   : out std_logic_vector(7 downto 0);
    rd_data_valid : out std_logic);
end uP_interface;



architecture STR of uP_interface is

signal read_s,read_empty : std_logic;
signal addr_high: std_logic_vector(7 downto 0);

type STATE_TYPE is (WAIT_STATE,READ_STATE);
signal state, next_state : STATE_TYPE := WAIT_STATE;

  begin

  process(ale1,uP_addr)
  begin
    if(ale1 = '1') then
      addr_high <= uP_addr;
    end if;

  end process;

  U_FIFO: entity work.FIFO
    port map(
      data(23 downto 16) => uP_data,
      data(15 downto 8) => addr_high,
      data(7 downto 0) => uP_addr,
      wrclk => clk_in,
      wrreq => not(wr_en),
      wrfull => open,
      rdclk => clk_out,
      rdreq => read_s,
      rdempty => read_empty,
      q(23 downto 16) => matrix_data,
      q(15 downto 0) => matrix_addr
    );

    process(clk_out,rst)
    begin
      if(rst = '1') then
        read_s <= '0';
        state <= WAIT_STATE;
      elsif(rising_edge(clk_out)) then

        state <= state;
        rd_data_valid <= '0';
        read_s <= '0';

        case state is
          when WAIT_STATE =>

            if(read_empty = '0') then
              read_s <= '1';
              state <= READ_STATE;
            end if;

          when READ_STATE =>

            rd_data_valid <= '1';
            state <= WAIT_STATE;

            if(read_empty = '0') then
              read_s <= '1';
              state <= READ_STATE;
            end if;
        end case;

      end if;

    end process;




end STR;
