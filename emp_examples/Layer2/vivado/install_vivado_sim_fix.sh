#!/bin/sh

# first parameter is path

# run as...
#   ./install_vivado_sim_fix.sh <path-to-vivado-fix-folder> <path-to-target-source>
#
# e.g.:
#   ./install_vivado_sim_fix.sh  vivadoSimFix/  ../submodules/RuflCore

SRC=$1
DEST=$2

echo
echo
echo "Installing Vivado sim fix into project..."
echo

echo "Vivado sim fix location: $SRC"
echo "Target source folder location: $DEST"

echo
echo

if [ ! -e $SRC/pkgVivadoSimFix.vhd ]; then
    echo "ERROR! Illegal source folder parameter, <path-to-vivado-fix-folder>/pkgVivadoSimFix.vhd must exist..."
	echo "${SRC}/pkgVivadoSimFix.vhd file does not exist!"
	echo
	echo
	echo "usage:    ./install_vivado_sim_fix.sh <path-to-vivado-fix-folder> <path-to-target-source>"
	echo "example:  ./install_vivado_sim_fix.sh  vivadoSimFix/  ../submodules/RuflCore"
	echo
	echo
    exit
fi

if [ ! -d $DEST ]; then
    echo "ERROR! Illegal target folder parameter, <path-to-target-source> directory must exist..."
	echo "${DEST} directory does not exist!"
	echo
	echo
	echo "usage:    ./install_vivado_sim_fix.sh <path-to-vivado-fix-folder> <path-to-target-source>"
	echo "example:  ./install_vivado_sim_fix.sh  vivadoSimFix/  ../submodules/RuflCore"
	echo
	echo
    exit
fi
#exit

# ------------------------- MAXIMUM -------------------------------- #
#search for MAXIMUM with white space before and open parenthesis after
echo
echo "The following files will be modified for the MAXIMUM vhd 2008 sim fix:"
grep -ril "[[:blank:]]\+MAXIMUM.*(" $DEST

grep -ril "[[:blank:]]\+MAXIMUM.*(" $DEST | xargs sed -i s/LIBRARY\ *SimFix.*\;//gI #first revert (I for case insensitive)
grep -ril "[[:blank:]]\+MAXIMUM.*(" $DEST | xargs sed -i s/use\ *\.pkgVivadoSimFix.*\;//gI #first revert (I for case insensitive)
grep -ril "[[:blank:]]\+MAXIMUM.*(" $DEST | xargs sed -i s/LIBRARY.*IEEE\;/LIBRARY\ IEEE\;\ LIBRARY\ SimFix\;\ use\ SimFix\.pkgVivadoSimFix\.all\;/gI #then insert (I for case insensitive) 
grep -ril "[[:blank:]]\+MAXIMUM.*(" $DEST | xargs sed -i s/MAxiMUM.*\(/MAXIMUM\_simfix\(/gI #case insensitive replace (I for case insensitive)

# for testing/debugging:
# sed -i s/.*use.*work\.pkgVivadoSimFix.*\;//gI submodules/RuflCore/firmware/hdl/ReuseableElements/PkgUtilities.vhd
# sed -i s/LIBRARY.*IEEE\;/LIBRARY\ IEEE\;\ use\ work\.pkgVivadoSimFix\.all\;/gI submodules/RuflCore/firmware/hdl/ReuseableElements/PkgUtilities.vhd
# sed -i s/MAxiMUM.*\(/MAXIMUM\_simfix\(/gI submodules/RuflCore/firmware/hdl/ReuseableElements/PkgUtilities.vhd

# ------------------------- MINIMUM -------------------------------- #
#search for MINIMUM with white space before and open parenthesis after
echo
echo "The following files will be modified for the MINIMUM vhd 2008 sim fix:"
grep -ril "[[:blank:]]\+MINIMUM.*(" $DEST

grep -ril "[[:blank:]]\+MINIMUM.*(" $DEST | xargs sed -i s/LIBRARY\ *SimFix.*\;//gI #first revert (I for case insensitive)
grep -ril "[[:blank:]]\+MINIMUM.*(" $DEST | xargs sed -i s/use\ *\.pkgVivadoSimFix.*\;//gI #first revert (I for case insensitive)
grep -ril "[[:blank:]]\+MINIMUM.*(" $DEST | xargs sed -i s/LIBRARY.*IEEE\;/LIBRARY\ IEEE\;\ LIBRARY\ SimFix\;\ use\ SimFix\.pkgVivadoSimFix\.all\;/gI #then insert (I for case insensitive) 
grep -ril "[[:blank:]]\+MINIMUM.*(" $DEST | xargs sed -i s/MIniMUM.*\(/MINIMUM\_simfix\(/gI #case insensitive replace (I for case insensitive)


# ------------------------- Matrix16 -------------------------------- #
#search for MMatrix16INIMUM with white space before and open parenthesis after
echo
echo "The following files will be modified for the Matrix16 vhd 2008 sim fix:"
grep -ril "Matrix\(.*\)\(0.*15\)" $DEST
grep -ril "Matrix\(.*\)\(0.*15\)" $DEST | xargs sed -ri "s|Matrix\(([^)]+)\)\(0.*15\)|Matrix16\(\1\)|gI"

# ------------------------- Matrix32 -------------------------------- #
#search for Matrix32 with white space before and open parenthesis after
echo
echo "The following files will be modified for the Matrix32 vhd 2008 sim fix:"
grep -ril "Matrix\(.*\)\(0.*31\)" $DEST
grep -ril "Matrix\(.*\)\(0.*31\)" $DEST | xargs sed -ri "s|Matrix\(([^)]+)\)\(0.*31\)|Matrix32\(\1\)|gI"

# ------------------------- Matrix64 -------------------------------- #
#search for Matrix64 with white space before and open parenthesis after
echo
echo "The following files will be modified for the Matrix64 vhd 2008 sim fix:"
grep -ril "Matrix\(.*\)\(0.*63\)" $DEST
grep -ril "Matrix\(.*\)\(0.*63\)" $DEST | xargs sed -ri "s|Matrix\(([^)]+)\)\(0.*63\)|Matrix64\(\1\)|gI"

# ------------------------- Matrix4 -------------------------------- #
#search for Matrix4 with white space before and open parenthesis after
echo
echo "The following files will be modified for the Matrix4 vhd 2008 sim fix:"
grep -ril "Matrix\(.*\)\(0.*3\)" $DEST
grep -ril "Matrix\(.*\)\(0.*3\)" $DEST | xargs sed -ri "s|Matrix\(([^)]+)\)\(0.*3\)|Matrix4\(\1\)|gI"

# ------------------------- Matrix8 -------------------------------- #
#search for Matrix8 with white space before and open parenthesis after
echo
echo "The following files will be modified for the Matrix8 vhd 2008 sim fix:"
grep -ril "Matrix\(.*\)\(0.*7\)" $DEST
grep -ril "Matrix\(.*\)\(0.*7\)" $DEST | xargs sed -ri "s|Matrix\(([^)]+)\)\(0.*7\)|Matrix8\(\1\)|gI"


echo
echo "The following files will be modified for the NullMatrix vhd 2008 sim fix:"
grep -ril "Int.ArrayTypes.NullMatrix\(.*\)\(.*,.*\)"
grep -ril "Int.ArrayTypes.NullMatrix\(.*\)\(.*,.*\)" $DEST | xargs sed -ri "s|Int.ArrayTypes.NullMatrix\(.*,.*\)|\( OTHERS => \( OTHERS => Int.DataType.cNull \) \)|gI"
grep -ril "IO.ArrayTypes.NullMatrix\(.*\)\(.*,.*\)"
grep -ril "IO.ArrayTypes.NullMatrix\(.*\)\(.*,.*\)" $DEST | xargs sed -ri "s|IO.ArrayTypes.NullMatrix\(.*,.*\)|\( OTHERS => \( OTHERS => IO.DataType.cNull \) \)|gI"
grep -ril "NullMatrix\(.*\)\(.*,.*\)" $DEST
grep -ril "NullMatrix\(.*\)\(.*,.*\)" $DEST | xargs sed -ri "s|NullMatrix\(.*,.*\)|\( OTHERS => \( OTHERS => cNull \) \)|gI"

#
# Must catch and replace, e.g. "11" before "1", otherwise replacement will break (e.g. same for 64 and 4, 32 and 2, ...)
#   In general, replace 3 digit numbers, then 2 digit ones, then 1

# ------------------------- VectorPipe128 -------------------------------- #
#search for VectorPipe64 with white space before and open parenthesis after
echo
echo "The following files will be modified for the VectorPipe12 vhd 2008 sim fix:"
grep -ril "VectorPipe\(.*\)\(0.*127\)" $DEST
grep -ril "VectorPipe\(.*\)\(0.*127\)" $DEST | xargs sed -ri "s|VectorPipe\(([^)]+)\)\(0.*127\)|VectorPipe128\(\1\)|gI"

# ------------------------- VectorPipe64 -------------------------------- #
#search for VectorPipe64 with white space before and open parenthesis after
echo
echo "The following files will be modified for the VectorPipe64 vhd 2008 sim fix:"
grep -ril "VectorPipe\(.*\)\(0.*63\)" $DEST
grep -ril "VectorPipe\(.*\)\(0.*63\)" $DEST | xargs sed -ri "s|VectorPipe\(([^)]+)\)\(0.*63\)|VectorPipe64\(\1\)|gI"

# ------------------------- VectorPipe32 -------------------------------- #
#search for VectorPipe32 with white space before and open parenthesis after
echo
echo "The following files will be modified for the VectorPipe32 vhd 2008 sim fix:"
grep -ril "VectorPipe\(.*\)\(0.*31\)" $DEST
grep -ril "VectorPipe\(.*\)\(0.*31\)" $DEST | xargs sed -ri "s|VectorPipe\(([^)]+)\)\(0.*31\)|VectorPipe32\(\1\)|gI"

# ------------------------- VectorPipe16 -------------------------------- #
#search for VectorPipe16 with white space before and open parenthesis after
echo
echo "The following files will be modified for the VectorPipe16 vhd 2008 sim fix:"
grep -ril "VectorPipe\(.*\)\(0.*15\)" $DEST
grep -ril "VectorPipe\(.*\)\(0.*15\)" $DEST | xargs sed -ri "s|VectorPipe\(([^)]+)\)\(0.*15\)|VectorPipe16\(\1\)|gI"

# ------------------------- VectorPipe12 -------------------------------- #
#search for VectorPipe12 with white space before and open parenthesis after
echo
echo "The following files will be modified for the VectorPipe12 vhd 2008 sim fix:"
grep -ril "VectorPipe\(.*\)\(0.*11\)" $DEST
grep -ril "VectorPipe\(.*\)\(0.*11\)" $DEST | xargs sed -ri "s|VectorPipe\(([^)]+)\)\(0.*11\)|VectorPipe12\(\1\)|gI"

# ------------------------- VectorPipe2 -------------------------------- #
#search for VectorPipe2 with white space before and open parenthesis after
echo
echo "The following files will be modified for the VectorPipe2 vhd 2008 sim fix:"
grep -ril "VectorPipe\(.*\)\(0.*1\)" $DEST
grep -ril "VectorPipe\(.*\)\(0.*1\)" $DEST | xargs sed -ri "s|VectorPipe\(([^)]+)\)\(0.*1\)|VectorPipe2\(\1\)|gI"

echo
echo "The following files will be modified for the NullVectorPipe vhd 2008 sim fix:"
grep -ril "Int.ArrayTypes.NullVectorPipe\(.*\)\(.*,.*\)" $DEST
grep -ril "Int.ArrayTypes.NullVectorPipe\(.*\)\(.*,.*\)" $DEST | xargs sed -ri "s|Int.ArrayTypes.NullVectorPipe\(.*,.*\)|\( OTHERS => \( OTHERS => Int.DataType.cNull \) \)|gI"
grep -ril "IO.ArrayTypes.NullVectorPipe\(.*\)\(.*,.*\)" $DEST
grep -ril "IO.ArrayTypes.NullVectorPipe\(.*\)\(.*,.*\)" $DEST | xargs sed -ri "s|IO.ArrayTypes.NullVectorPipe\(.*,.*\)|\( OTHERS => \( OTHERS => IO.DataType.cNull \) \)|gI"
grep -ril "NullVectorPipe\(.*\)\(.*,.*\)" $DEST
grep -ril "NullVectorPipe\(.*\)\(.*,.*\)" $DEST | xargs sed -ri "s|NullVectorPipe\(.*,.*\)|\( OTHERS => \( OTHERS => cNull \) \)|gI"


# ------------------------- Vector128 -------------------------------- #
#search for Vector128 with white space before and open parenthesis after
echo
echo "The following files will be modified for the Vector128 vhd 2008 sim fix:"
grep -ril "Vector(0.*127)" $DEST
grep -ril "Vector(0.*127)" $DEST | xargs sed -ri "s|Vector\(0.*127\)|Vector128|gI"
# ------------------------- Vector64 -------------------------------- #
#search for Vector64 with white space before and open parenthesis after
echo
echo "The following files will be modified for the Vector64 vhd 2008 sim fix:"
grep -ril "Vector(0.*63)" $DEST
grep -ril "Vector(0.*63)" $DEST | xargs sed -ri "s|Vector\(0.*63\)|Vector64|gI"
# ------------------------- Vector32 -------------------------------- #
#search for Vector32 with white space before and open parenthesis after
echo
echo "The following files will be modified for the Vector32 vhd 2008 sim fix:"
grep -ril "Vector(0.*31)" $DEST
grep -ril "Vector(0.*31)" $DEST | xargs sed -ri "s|Vector\(0.*31\)|Vector32|gI"
# ------------------------- Vector16 -------------------------------- #
#search for Vector16 with white space before and open parenthesis after
echo
echo "The following files will be modified for the Vector16 vhd 2008 sim fix:"
grep -ril "Vector(0.*15)" $DEST
grep -ril "Vector(0.*15)" $DEST | xargs sed -ri "s|Vector\(0.*15\)|Vector16|gI"
# ------------------------- Vector12 -------------------------------- #
#search for Vector12 with white space before and open parenthesis after
echo
echo "The following files will be modified for the Vector12 vhd 2008 sim fix:"
grep -ril "Vector(0.*11)" $DEST
grep -ril "Vector(0.*11)" $DEST | xargs sed -ri "s|Vector\(0.*11\)|Vector12|gI"
# ------------------------- Vector8 -------------------------------- #
#search for Vector8 with white space before and open parenthesis after
echo
echo "The following files will be modified for the Vector8 vhd 2008 sim fix:"
grep -ril "Vector(0.*7)" $DEST
grep -ril "Vector(0.*7)" $DEST | xargs sed -ri "s|Vector\(0.*7\)|Vector8|gI"
# ------------------------- Vector2 -------------------------------- #
#search for Vector2 with white space before and open parenthesis after
echo
echo "The following files will be modified for the Vector2 vhd 2008 sim fix:"
grep -ril "Vector(0.*1)" $DEST
grep -ril "Vector(0.*1)" $DEST | xargs sed -ri "s|Vector\(0.*1\)|Vector2|gI"

echo
echo "The following files will be modified for the NullVector vhd 2008 sim fix:"
grep -ril "Int.ArrayTypes.NullVector(.*)" $DEST
grep -ril "Int.ArrayTypes.NullVector(.*)" $DEST | xargs sed -ri "s|Int.ArrayTypes.NullVector\(.*\)|\( OTHERS => Int.DataType.cNull \)|gI"
grep -ril "IO.ArrayTypes.NullVector(.*)" $DEST
grep -ril "IO.ArrayTypes.NullVector(.*)" $DEST | xargs sed -ri "s|IO.ArrayTypes.NullVector\(.*\)|\( OTHERS => IO.DataType.cNull \)|gI"
grep -ril "NullVector(.*)" $DEST
grep -ril "NullVector(.*)" $DEST | xargs sed -ri "s|NullVector\(.*\)|\( OTHERS => cNull \)|gI"


echo
echo
echo "Done installing Vivado sim fix!"


exit


##### EXAMPLE EXCERPTS

#modify number of threads if passed as 3rd parameter
if [ "x$3" != "x" ]; then
    sed -i "s|general\.maxThreads.*|general\.maxThreads\ $3|g" $DEST/vivado/properties.tcl
fi

#modify speed grade if passed as 4th parameter
cp $DEST/../correlator-layer1-refPrj/Makefile $DEST/Makefile
if [ "x$4" == "x13" ]; then
    sed -i "s|xcvu9p\-flgc2104\-1\-e|xcvu13p\-flga2577\-1\-e|g" $DEST/Makefile
elif [ "x$4" != "x" ]; then
    sed -i "s|xcvu9p\-flgc2104\-1\-e|xcvu9p\-flgc2104\-$4\-e|g" $DEST/Makefile
    #Ales says vu13p-flga2577-1-e and vu9p-flga2577-2-e
fi



#modify HLS vhdl if passed as 5th parameter
if [ "x$5" != "x" ]; then
    echo "cp $5/*.vhd $DEST/algo/pfalgo3_small/"
    rm $DEST/algo/pfalgo3_small/*.vhd
    cp $5/*.vhd $DEST/algo/pfalgo3_small/
fi

cp $DEST/../correlator-layer1-refPrj/ruckus.tcl $DEST/ruckus.tcl
echo "loadSource      -dir \"$::DIR_PATH/algo/\"" >> $DEST/ruckus.tcl
echo "loadIpCore      -dir \"$::DIR_PATH/algo-ip/\"" >> $DEST/ruckus.tcl
if [ -d $DEST/input96b_src ]; then
    echo "loadSource      -dir \"$::DIR_PATH/input96b_src/\"" >> $DEST/ruckus.tcl
fi
if [ -d $DEST/output_sort ]; then
    echo "loadSource      -dir \"$::DIR_PATH/output_sort/\"" >> $DEST/ruckus.tcl
fi


if [ -d $DEST/algo/pfalgo3_small ]; then
    echo "loadSource      -dir \"$::DIR_PATH/algo/pfalgo3_small/\"" >> $DEST/ruckus.tcl
fi
if [ -d $DEST/algo/pfsplit ]; then
    echo "loadSource      -dir \"$::DIR_PATH/algo/pfsplit/\"" >> $DEST/ruckus.tcl
fi

echo "set_property IOSTANDARD LVDS [get_ports clk_300_clk_p]" >> $DEST/constraints/constraints.xdc
echo "set_property IOSTANDARD LVDS [get_ports clk_300_clk_n]" >> $DEST/constraints/constraints.xdc

echo
echo
echo "Done installing regionizer!"
