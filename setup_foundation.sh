#!/bin/bash
PROJECT_DIR=${PWD}
CPP_FOUNDATION_DIR=${PROJECT_DIR}/foundation
THIRD_PARTY=${PROJECT_DIR}/3rdparty
source $HOME/.bashrc
echo "Home: $HOME"
echo "RTI_HOME: $RTI_HOME"

# 3rdparty
echo "Compiling 3rd party libraries first"
cd ${THIRD_PARTY}
mvn clean install deploy -fae -U -B -X

# foundation
echo "Entering foundation Directory: " ${CPP_FOUNDATION_DIR}
cd ${CPP_FOUNDATION_DIR}
mvn clean install deploy -fae -U -B -X

echo "=================================================================================="
echo "Completed the compilation, installation, deployment of the C2W foundation packages"
echo "=================================================================================="
