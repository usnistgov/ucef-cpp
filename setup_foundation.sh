#!/bin/bash


mvn_install_deploy() {
	echo "Maven install..."
	mvn clean install deploy -U
}

traverse_dir(){
	echo ${1}
	cd ${1} 
	mvn_install_deploy
	cd ..
}

PROJECT_DIR=${PWD}
CPP_FOUNDATION_DIR=${PROJECT_DIR}/foundation
THIRD_PARTY=${PROJECT_DIR}/3rdparty

echo "Compiling 3rd party libraries first"

# 3rdparty
cd ${THIRD_PARTY}

# 3rdparty/portico-hla
traverse_dir "portico-hla"

# 3rdparty/mysql-connector
traverse_dir "mysql-connector"

# 3rdparty
mvn_install_deploy

# .
cd ${PROJECT_DIR}

# foundation
echo "Entering foundation Directory: " ${CPP_FOUNDATION_DIR}
cd ${CPP_FOUNDATION_DIR}

# foundation/*
traverse_dir "core"
traverse_dir "C2WMySQLLogger"
traverse_dir "C2WConsoleLogger"
traverse_dir "config"
traverse_dir "rti-base"
traverse_dir "SynchronizedFederate"
traverse_dir "OmnetFederate"
traverse_dir "ExecutionAssembler"

# foundation/
mvn_install_deploy

echo "=================================================================================="
echo "Completed the compilation, installation, deployment of the C2W foundation packages"
echo "=================================================================================="
