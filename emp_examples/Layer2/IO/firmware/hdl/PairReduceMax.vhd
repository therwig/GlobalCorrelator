library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library Utilities;
use Utilities.Utilities.all;
use Utilities.Debugging.all;

use work.DataType.all;
use work.ArrayTypes.all;

entity PairReduceMax is
generic(
  id : string := "0"
);
port(
  clk : in std_logic := '0';
  d : in Vector;
  q : out Vector
);
end PairReduceMax;

architecture behavioral of PairReduceMax is

constant len : integer := d'length;
constant intLen : integer := 2 * ((len + 1) / 2);
constant qLen : integer := (len + 1) / 2;

component PairReduceMax is
generic(
  id : string := "0"
);
port(
  clk : in std_logic := '0';
  d : in Vector; --(0 to intLen / 2 - 1);
  q : out Vector --(0 to qLen / 2 - 1) 
);
end component PairReduceMax;

begin

G1 : if d'length <= 1 generate
    q <= d;
end generate;

G2 : if d'length = 2 generate
    process(clk)
    begin
        if rising_edge(clk) then
            if d(d'left) > d(d'right) then
                q(q'left) <= d(d'left);
            else
                q(q'left) <= d(d'right);
            end if;
        end if;
    end process;
end generate;

GN : if d'length > 2 generate
  -- Lengths are rounded up to nearest even
  signal dInt : Vector(0 to intLen - 1) := NullVector(intLen);
  signal qInt : Vector(0 to qLen - 1) := NullVector(qLen);
  begin
    dInt(0 to d'length - 1) <= d;

    GNComps:
    for i in 0 to qLen - 1 generate
        Comp:
        process(clk)
        begin
        if rising_edge(clk) then
            if dInt(2*i) > dInt(2*i+1) then
                qInt(i) <= dInt(2*i);
            else
                qInt(i) <= dInt(2*i+1);
            end if;
        end if;
        end process;
    end generate;
    --Reduce0 : PairReduceMax
    --generic map(id => id & "_A")
    --port map(clk, dInt(0 to dInt'length / 2 - 1), qInt(0 to qInt'length / 2 - 1));

    --Reduce1 : PairReduceMax
    --generic map(id => id & "_B")
    --port map(clk, dInt(dInt'length / 2 to dInt'length - 1), qInt(qInt'length / 2 to qInt'length - 1));

    Reduce2 : PairReduceMax
    generic map(id => id & "_C")
    port map(clk, qInt, q);

end generate;

--  DebugIn:
--  entity work.Debug
--  generic map("RI" & id, "./")
--  port map(clk, d);

--  DebugOut:
--  entity work.Debug
--  generic map("RO" & id, "./")
--  port map(clk, q);

end behavioral;
