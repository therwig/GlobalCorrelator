library ieee;
use ieee.std_logic_1164.all;

package met_data_types is

  type met_data is array (natural range <>) of std_logic_vector(31 downto 0);

  type met_data_array is array (natural range <>) of met_data;

end;
