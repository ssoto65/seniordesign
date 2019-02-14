-- Adafruit RGB LED Matrix Display Driver
-- Top Level Entity
--
-- Copyright (c) 2012 Brian Nezvadovitz <http://nezzen.net>
-- This software is distributed under the terms of the MIT License shown below.
--
-- Permission is hereby granted, free of charge, to any person obtaining a copy
-- of this software and associated documentation files (the "Software"), to
-- deal in the Software without restriction, including without limitation the
-- rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
-- sell copies of the Software, and to permit persons to whom the Software is
-- furnished to do so, subject to the following conditions:
--
-- The above copyright notice and this permission notice shall be included in
-- all copies or substantial portions of the Software.
--
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
-- FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
-- IN THE SOFTWARE.

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;

use work.rgbmatrix.all; -- Constants & Configuration

entity top_level is
    port (
      --uP signals
        uP_clk  : in std_logic;
		  uP_CS	 : in std_logic;
        uP_wr_en: in std_logic;
        uP_ale1 : in std_logic;
        uP_addr : in std_logic_vector(11 downto 0);
        uP_data : in std_logic_vector(7 downto 0);
      --matrix signals
        clk_in  : in std_logic;
        rst_n   : in std_logic;
        clk_out : out std_logic;
        r1      : out std_logic;
        r2      : out std_logic;
        b1      : out std_logic;
        b2      : out std_logic;
        g1      : out std_logic;
        g2      : out std_logic;
        a       : out std_logic;
        b       : out std_logic;
        c       : out std_logic;
        d       : out std_logic;
        lat     : out std_logic;
        oe      : out std_logic;
		--debug signals
		  int_addr : out std_logic_vector(15 downto 0);
		  int_data : out std_logic_vector(7 downto 0);
		  int_data_valid : out std_logic;
		  int_clk : out std_logic
    );
end top_level;

architecture str of top_level is

    -- Reset signals
    signal rst, rst_p, jtag_rst_out, clk_100 : std_logic;

    -- Memory signals
    signal addr : std_logic_vector(ADDR_WIDTH-1 downto 0);
    signal addr_incoming,addr_mem : std_logic_vector(ADDR_WIDTH downto 0);
    signal data_incoming : std_logic_vector(DATA_WIDTH/2-1 downto 0);
    signal data_outgoing : std_logic_vector(DATA_WIDTH-1 downto 0);
	 signal data_incoming_byte : std_logic_vector(7 downto 0);
	 signal color_sel	: std_logic_vector(1 downto 0);

    -- Flags
    signal data_valid, word_wr_en : std_logic;
begin

    -- Reset button is an "active low" input, invert it so we can treat is as
    -- "active high", then OR it with the JTAG reset command output signal.
    rst_p <= not rst_n;
    rst <= rst_p or jtag_rst_out;


    -- LED panel controller
    U_LEDCTRL : entity work.ledctrl
        port map (
            rst => '0', --rst,
            clk_in => clk_in,
            -- Connection to LED panel
            clk_out => clk_out,
            rgb1(2) => r1,
            rgb1(1) => g1,
            rgb1(0) => b1,
            rgb2(2) => r2,
            rgb2(1) => g2,
            rgb2(0) => b2,
            led_addr(3) => d,
            led_addr(2) => c,
            led_addr(1) => b,
            led_addr(0) => a,
            lat => lat,
            oe  => oe,
            -- Connection with framebuffer
            addr => addr,
            data => data_outgoing-- X"100000000010"--data_outgoing
        );
		  
		  U_PLL: entity work.PLL
				port map(
					inclk0 => clk_in,
					c0 => clk_100,
					locked => open
				);
			

    -- -- Virtual JTAG interface
    -- U_JTAGIFACE : entity work.jtag_iface
        -- port map (
            -- rst     => rst,
            -- rst_out => open, --jtag_rst_out,
            -- output  => open, --data_incoming,
            -- valid   => open --data_valid
        -- );

--     U_CNT : entity work.cnt
--         port map (
--             clk     => clk_in,
--             rst     => '0', --rst,
--             output  => data_incoming,
--             valid   => data_valid
--         );
--
--     -- Special memory for the framebuffer
--     U_MEMORY : entity work.memory
--         port map (
--             rst => '0', --rst,
--             -- Writing side
--             clk_wr =>  data_valid,--'0', --open,
--             input  => data_incoming,--std_logic_vector(to_unsigned(85, DATA_WIDTH)), --open, --
--             -- Reading side
--             clk_rd => clk_in,
--             addr   => addr,
--             output => data_outgoing
--         );

		U_RAM: entity work.RAM
			port map(
				rdclock => clk_in,
				rdaddress => addr,
				q => data_outgoing,
				wrclock => clk_in,
				wraddress => addr_mem,
				wren => word_wr_en,
				data => data_incoming
			);

		U_Interface: work.uP_interface
			port map(
				clk_in => uP_clk,
				wr_en => uP_wr_en,
				ale1 => uP_ale1,
				uP_CS => uP_CS,
				rst => '0',
				uP_addr => uP_addr,
				uP_data => uP_data,
				clk_out => clk_in,
				matrix_data => data_incoming_byte,
				matrix_addr(11 downto 2) => addr_incoming,
				matrix_addr(1 downto 0) => color_sel,
				rd_data_valid => data_valid
			);
		
		U_Word_Filler: work.word_filler
			port map(
				clk => clk_in,
				rst => '0',
				color_sel => color_sel,
				addr_in => addr_incoming,
				addr_out => addr_mem,
				byte_wr_en => data_valid,
				byte_in => data_incoming_byte,
				word_out => data_incoming,
				word_wr_en => word_wr_en
			);
			
			--int_addr(11 downto 2) <= addr_incoming;
			--int_addr(1 downto 0)<= color_sel;
			int_addr(11 downto 0)<= uP_addr;
			int_data <= uP_data;
			int_data_valid <= uP_wr_en;
			int_clk <= uP_clk;

end str;
