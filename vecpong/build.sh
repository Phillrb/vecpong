#!/bin/bash
##
##  Created by Martin White 2014
##  Copyright (c) 2014 Martin White. All rights reserved.
##  Extended by Phillip Riscombe-Burton 2015
##
##	Permission is hereby granted, free of charge, to any person obtaining a
##	copy of this software and associated documentation files (the
##	"Software"), to deal in the Software without restriction, including
##	without limitation the rights to use, copy, modify, merge, publish,
##	distribute, sublicense, and/or sell copies of the Software, and to
##	permit persons to whom the Software is furnished to do so, subject to
##	the following conditions:
##
##	The above copyright notice and this permission notice shall be included
##	in all copies or substantial portions of the Software.
##
##	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
##	OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
##	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
##	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
##	CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
##	TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
##	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
##
##
#########  MAKE SURE ASSEMBLER FILES ARE .S    #########
#########     OR THEY'LL BE LOST ON CLEAN      #########

#
# This build script is executed by xcode on build.
# It is linked by: TARGETS -> [target] -> Info -> External Build Tool Configuration
# The only parameter passed in is the project name and it is assumed that this
# script lives in a folder of the same name as the project.
#
# The script:
# - Assembles the assembly stubs (crt.s and crt0.s)
# - Compiles the C headers and main.c into assembly (main.s)
# - Assembles the new main.s assembly file
# - Runs the linker
# - Converts the output into a vectrex binary file
# - launches the binary in the simulator (ParaJVE)
#

BIN=$1

SRC=src
INC=include
OBJ=intermediate

echo "BIN NAME: "$BIN".bin"

function errorTrap()
{
rc=$?
exit $rc
}

trap errorTrap ERR;

#get path of this script - BIN file will be dumped alongside
pushd `dirname $0` > /dev/null
SCRIPTPATH=`pwd -P`
popd > /dev/null

# create intermediate dir if needed
mkdir -p $BIN/$OBJ

# Assemble CRT0
echo "Assembling CRT stub..."
/usr/local/bin/as6809 -logpcb $BIN/$OBJ/crt0 $BIN/src/crt0.s

# Compile main program to assembler
echo "Compiling C to ASM..."
/usr/local/bin/m6809-cc1 -I$BIN/$INC \
$BIN/$SRC/main.c \
-dumpbase $BIN/$SRC/main.c  \
-mno-direct -mint8 -msoft-reg-count=0 -auxbase main \
-O3 -o $BIN/$OBJ/main.s

# Assemble main code
echo "Assembling main code..."
/usr/local/bin/as6809 -logpcb $BIN/$OBJ/main.s

# Link it all up
echo "Linking..."
/usr/local/bin/aslink -m -nwsu -b .text=0x0 $BIN/$OBJ/main.s19 $BIN/$OBJ/crt0.rel $BIN/$OBJ/main.rel

# Make Vectrex binary
echo "Converting output to Vectrex binary..."
/usr/local/bin/srec_cat $BIN/$OBJ/main.s19 -o $BIN/$BIN.bin -binary

# Launch in ParaJVE
echo "Launching binary in ParaJVE..."
open -a ParaJVE --args -game=$SCRIPTPATH/$BIN.bin -Debug
