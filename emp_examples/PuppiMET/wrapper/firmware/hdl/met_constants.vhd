library ieee;

use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

package met_constants is

  constant MET_ALGO_LATENCY : natural := 47;  -- Algorithm latency in 240 MHz
                                                -- ticks
  constant MAX_MET_IP_CORES : natural := 1;
  constant N_MET_IP_CORES : natural := 1;  -- Up to 6
  constant N_MET_IP_CORE_IN_CHANS : natural := 60;
  constant N_MET_IP_CORE_OUT_CHANS : natural := 60;
  constant N_CHANS_PER_CORE : natural := 60;
  constant MET_RESHAPE_FACTOR : natural := 1;
  
  constant N_IN_CHANS  : natural := 60;
  constant N_OUT_CHANS : natural := 60;
  
end;       
