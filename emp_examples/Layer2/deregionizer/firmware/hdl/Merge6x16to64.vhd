library ieee;
use ieee.std_logic_1164.all;

use work.DataType.all;
use work.ArrayTypes.all;

library Int;
use Int.DataType;
use Int.ArrayTypes;

entity MergeArrays is
port(
    clk : in std_logic := '0';
    D : in Matrix_6_16 := NullMatrix_6_16;
    Q : out Vector_64 := NullVector_64
);
end MergeArrays;

architecture behavioral of MergeArrays is
    constant latency_l1 : integer := 5; -- The latency of the 2nd merge layer for pipelining

    -- Output of first merge layer
    signal d0 : Matrix_3_32 := NullMatrix_3_32;
    -- Output of second merge layer
    signal d0_pipe : VectorPipe_5_32 := NullVectorPipe_5_32;
    signal d1 : Matrix_2_64 := NullMatrix_2_64;
    signal d2 : Matrix_2_64 := NullMatrix_2_64;

begin

    MergeLayer1:
    for i in 0 to 2 generate
        merge : entity work.Merge16to32
        port map(
            clk => clk,
            a => d(2*i),
            b => d(2*i + 1),
            q => d0(i)
        );
    end generate;

    MergeLayer2: entity work.Merge32to64
    port map(
        clk => clk,
        a => d0(0),
        b => d0(1),
        q => d1(0)
    );

    PipeLayer2: entity work.DataPipe_32
    port map(clk, d0(2), d0_pipe);

    MergeLayer3: entity work.Merge32and64to64
    port map(
        clk => clk,
        a => d1(0),
        b => d0_pipe(latency_l1-1),
        q => Q
    );

end behavioral;
