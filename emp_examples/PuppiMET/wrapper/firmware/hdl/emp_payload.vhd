-- null_algo
--
-- Do-nothing top level algo for testing
--
-- Dave Newbold, July 2013

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

use work.ipbus.all;
use work.emp_data_types.all;
use work.top_decl.all;

use work.emp_data_types.all;
use work.emp_device_decl.all;
--use work.mp7_ttc_decl.all;

use work.met_data_types.all;
use work.met_constants.all;

entity emp_payload is
	port(
		clk: in std_logic; -- ipbus signals
		rst: in std_logic;
		ipb_in: in ipb_wbus;
		ipb_out: out ipb_rbus;
		clk_payload: in std_logic_vector(2 downto 0);
		rst_payload: in std_logic_vector(2 downto 0);
		clk_p: in std_logic; -- data clock
		rst_loc: in std_logic_vector(N_REGION - 1 downto 0);
		clken_loc: in std_logic_vector(N_REGION - 1 downto 0);
		ctrs: in ttc_stuff_array;
		bc0: out std_logic;
		d: in ldata(4 * N_REGION - 1 downto 0); -- data in
		q: out ldata(4 * N_REGION - 1 downto 0); -- data out
		gpio: out std_logic_vector(29 downto 0); -- IO to mezzanine connector
		gpio_en: out std_logic_vector(29 downto 0) -- IO to mezzanine connector (three-state enables)
	);
		
end emp_payload;

architecture rtl of emp_payload is

  signal rst_loc_reg : std_logic_vector(N_REGION - 1 downto 0);       
  constant N_FRAMES_USED : natural := 1;
  signal start_met : std_logic_vector(5 downto 0);
  signal d_met : met_data_array(N_MET_IP_CORES - 1 downto 0)(N_MET_IP_CORE_IN_CHANS - 1 downto 0);
  signal q_met : met_data_array(N_MET_IP_CORES - 1 downto 0)(N_MET_IP_CORE_OUT_CHANS - 1 downto 0);

begin

	ipb_out <= IPB_RBUS_NULL;

    multiplex : entity work.multiplexer
      port map(
        clk => clk_p,
        d => d(N_IN_CHANS - 1 downto 0),
        start_met => start_met,
        q_met => d_met
      );

   selector_gen : process (clk_p)
   begin  -- process selector_gen
     if clk_p'event and clk_p = '1' then  -- rising clock edge
       rst_loc_reg <= rst_loc;
      end if;
    end process selector_gen;

    generate_met_cores : for i in N_MET_IP_CORES - 1 downto 0 generate
      met_algo : entity work.met_ip_wrapper
        PORT MAP (
          clk    => clk_p,
          rst    => rst_loc(i),
          start  => start_met(i),
          -- start  => start_met(i),
          input  => d_met(i),
          done   => open,
          idle   => open,
          ready  => open,
          output => q_met(i)
        );
    end generate generate_met_cores;

    demux : entity work.demultiplexer
      port map (
        clk => clk_p,
        d_met => q_met,
        q => q(N_OUT_CHANS - 1 downto 0)
      );

     bc0 <= '0';
     gpio <= (others => '0');
     gpio_en <= (others => '0');

end rtl;
