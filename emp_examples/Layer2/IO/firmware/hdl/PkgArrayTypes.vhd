-- #########################################################################
-- #########################################################################
-- ###                                                                   ###
-- ###   Use of this code, whether in its current form or modified,      ###
-- ###   implies that you consent to the terms and conditions, namely:   ###
-- ###    - You acknowledge my contribution                              ###
-- ###    - This copyright notification remains intact                   ###
-- ###                                                                   ###
-- ###   Many thanks,                                                    ###
-- ###     Dr. Andrew W. Rose, Imperial College London, 2018             ###
-- ###                                                                   ###
-- #########################################################################
-- #########################################################################

-- -------------------------------------------------------------------------
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

USE work.DataType.ALL;
-- -------------------------------------------------------------------------

-- -------------------------------------------------------------------------
PACKAGE ArrayTypes IS

  type Vector_128 is array(0 to 127) of tData;
  type Vector_64 is array(0 to 63) of tData;
  type Vector_32 is array(0 to 31) of tData;
  type Vector_16 is array(0 to 15) of tData;
  type Vector_8 is array(0 to 7) of tData;
  type Vector_4 is array(0 to 3) of tData;

  constant NullVector_128 : Vector_128 := (others => cNull);
  constant NullVector_64 : Vector_64 := (others => cNull);
  constant NullVector_32 : Vector_32 := (others => cNull);
  constant NullVector_16 : Vector_16 := (others => cNull);
  constant NullVector_8 : Vector_8 := (others => cNull);
  constant NullVector_4 : Vector_4 := (others => cNull);

  type VectorPipe_5_64 is array(0 to 4) of Vector_64;
  type VectorPipe_5_32 is array(0 to 4) of Vector_32;
  type VectorPipe_5_16 is array(0 to 4) of Vector_16;

  constant NullVectorPipe_5_64 : VectorPipe_5_64 := (others => (others => cNull) );
  constant NullVectorPipe_5_32 : VectorPipe_5_32 := (others => (others => cNull) );
  constant NullVectorPipe_5_16 : VectorPipe_5_16 := (others => (others => cNull) );
  
  type Matrix_3_64 is array(0 to 2) of Vector_64;
  type Matrix_2_64 is array(0 to 1) of Vector_64;
  type Matrix_3_32 is array(0 to 2) of Vector_32;
  type Matrix_2_32 is array(0 to 1) of Vector_32;
  type Matrix_6_16 is array(0 to 5) of Vector_16;
  type Matrix_8_8 is array(0 to 7) of Vector_8;
  type Matrix_4_8 is array(0 to 3) of Vector_8;
  type Matrix_4_4 is array(0 to 3) of Vector_4;

  constant NullMatrix_3_64 : Matrix_3_64 := (others => (others => cNull) );
  constant NullMatrix_2_64 : Matrix_2_64 := (others => (others => cNull) );
  constant NullMatrix_3_32 : Matrix_3_32 := (others => (others => cNull) );
  constant NullMatrix_2_32 : Matrix_2_32 := (others => (others => cNull) );
  constant NullMatrix_6_16 : Matrix_6_16 := (others => (others => cNull) );
  constant NullMatrix_8_8 : Matrix_8_8 := (others => (others => cNull) );
  constant NullMatrix_4_8 : Matrix_4_8 := (others => (others => cNull) );
  constant NullMatrix_4_4 : Matrix_4_4 := (others => (others => cNull) );

  type Vector is array( natural range <> ) of tData;
  type VectorPipe5 is array(0 to 4) of Vector;

  --TYPE Pipe       IS ARRAY( NATURAL RANGE <> ) OF tData;

  --TYPE Vector     IS ARRAY( INTEGER RANGE <> ) OF tData;
  ----TYPE VectorPipe IS ARRAY( NATURAL RANGE <> ) OF Vector;
  ----TYPE Matrix     IS ARRAY( INTEGER RANGE <> ) OF Vector;

  --FUNCTION NullVector( aSize      : INTEGER ) RETURN Vector;
  --FUNCTION NullPipe( aSize        : NATURAL ) RETURN Pipe;
  ----FUNCTION NullVectorPipe( aSize1 : NATURAL ; aSize2 : INTEGER ) RETURN VectorPipe;
  ----FUNCTION NullMatrix( aSize1     : INTEGER ; aSize2 : INTEGER ) RETURN Matrix;

END ArrayTypes;
-- -------------------------------------------------------------------------



-- -------------------------------------------------------------------------
PACKAGE BODY ArrayTypes IS

  FUNCTION NullVector( aSize : INTEGER ) RETURN Vector IS
    VARIABLE lRet            : Vector( 0 TO aSize-1 ) := ( OTHERS => cNull );
  BEGIN
    RETURN lRet;
  END NullVector;

--  FUNCTION NullPipe( aSize : NATURAL ) RETURN Pipe IS
--    VARIABLE lRet          : Pipe( 0 TO aSize-1 ) := ( OTHERS => cNull );
--  BEGIN
--    RETURN lRet;
--  END NullPipe;

--  --FUNCTION NullVectorPipe( aSize1 : NATURAL ; aSize2 : INTEGER ) RETURN VectorPipe IS
--  --  VARIABLE lRet                 : VectorPipe( 0 TO aSize1-1 )( 0 TO aSize2-1 ) := ( OTHERS => ( OTHERS => cNull ) );
--  --BEGIN
--  --  RETURN lRet;
--  --END NullVectorPipe;

--  --FUNCTION NullMatrix( aSize1 : INTEGER ; aSize2 : INTEGER ) RETURN Matrix IS
--  --  VARIABLE lRet             : Matrix( 0 TO aSize1 - 1 )( 0 TO aSize2 - 1 ) := ( OTHERS => ( OTHERS => cNull ) );
--  --BEGIN
--  --  RETURN lRet;
--  --END NullMatrix;

end ArrayTypes;
