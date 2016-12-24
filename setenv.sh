#!/bin/sh


export EXEDIR="$HOME/Projects/C2NG/c2wtng-cpp/examples/HelloWorld/Execution/target/bin-exec"
export BOOST_LIB_DIR="$HOME/Projects/C2NG/boostlib/lib"
export BOOST_INC_DIR="$HOME/Projects/C2NG/boostlib/include"
export RTI_HOME="$HOME/portico-2.1.0"
export RTI_LIB_DIR="$RTI_HOME/lib/gcc4"
export MYSQL_LIB_DIR="$HOME/Projects/C2NG/mysql-connector-c++-1.0.5-osx10.5-x86-64bit/lib"
export DYLD_LIBRARY_PATH=".:$BOOST_LIB_DIR/lib:$RTI_LIB_DIR:$MYSQL_LIB_DIR/lib:$DYLD_LIBRARY_PATH"
echo "Setting environment variables (BOOST_LIB_DIR, RTI_LIB_DIR, MYSQL_LIB_DIR, etc.)"
