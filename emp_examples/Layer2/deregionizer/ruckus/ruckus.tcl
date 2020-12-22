# Load local source Code and constraints
loadSource -lib "IO" -dir "$::DIR_PATH/../firmware/hdl/" -fileType "VHDL 2008"
loadSource -lib "IO" -dir "$::DIR_PATH/../../IO/firmware/hdl/" -fileType "VHDL 2008"
loadSource -lib "IO" -path "$::DIR_PATH/../../components/firmware/hdl/PkgIO.vhd" -fileType "VHDL 2008"
loadSource -lib "Int" -path "$::DIR_PATH/../../components/firmware/hdl/PkgInt.vhd" -fileType "VHDL 2008"
loadSource -lib "Int" -path "$::DIR_PATH/../../Int/firmware/hdl/PkgArrayTypes.vhd" -fileType "VHDL 2008"


#loadSource -lib "Int" -path "$::DIR_PATH/../../Int/firmware/hdl/PkgArrayTypes.vhd" -fileType "VHDL 2008"

