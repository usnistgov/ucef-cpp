#!/bin/bash
# Author: Yogesh Barve
export PATH=$PATH:/home/vagrant/c2wt-dev/dependency/omnetpp/omnetpp-4.6/bin
export RTI_HOME=/home/vagrant/c2wt-dev/dependency/portico/portico-2.1.0
export INETMANET_HOME=/home/vagrant/c2wt-dev/dependency/inet/inetmanet-2.0-inetmanet-2.2/
export CONFIGNAME='gcc-debug'


export LD_LIBRARY_PATH=$RTI_HOME/jre/lib/amd64/server/

# export C2WTROOT=

# -I$(C2WTROOT_PROJ)/generated/core/cpp \
#     -I$(C2WTROOT_PROJ)/generated/core/cpp/include \
#     -I$(C2WTROOT_PROJ)/core/src/cpp/include \
#     -I$(C2WTROOT_PROJ)/generated/MapperEx/cpp/MapperEx/include \
#     -I$(C2WTROOT_PROJ)/generated/MapperEx/cpp/MapperEx \



mvn_install_deploy() {
	echo "Maven Compiling...."
	mvn clean install
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
mvn_install_deploy

echo "=================================================================================="
echo "Completed the compilation, installation, deployment of the C2W foundation packages"
echo "=================================================================================="
