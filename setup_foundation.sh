#!/bin/bash
# Author: Yogesh Barve
# export RTI_HOME=/home/vagrant/c2wt-dev/dependency/portico/portico-2.1.0

mvn_install_deploy() {
	echo "Maven Compiling...."
	mvn clean install -U
	echo "Maven Deploying to the Archiva....."
	mvn deploy
	echo "Deployment Completed...."

}

traverse_dir(){
	echo ${1}
	cd ${1} 
	mvn_install_deploy
	cd ..
}

PROJECT_DIR=${PWD}


CPP_FOUNDATION_DIR=$PROJECT_DIR/foundation 
thirdParty=$PROJECT_DIR/3rdparty

echo "Compiling the 3rdParty"
cd $thirdParty
traverse_dir "portico-hla"
traverse_dir "mysql-connector"
mvn_install_deploy
cd ..

echo "Entering foundation Directory: " $CPP_FOUNDATION_DIR
cd $CPP_FOUNDATION_DIR/


traverse_dir "core"
traverse_dir "C2WMySQLLogger"
traverse_dir "C2WConsoleLogger"
traverse_dir "rti-base"
traverse_dir "SynchronizedFederate"
traverse_dir "OmnetFederate"
traverse_dir "ExecutionAssembler"
mvn_install_deploy

echo "=================================================================================="
echo "Completed the compilation, installation, deployment of the C2W foundation packages"
echo "=================================================================================="