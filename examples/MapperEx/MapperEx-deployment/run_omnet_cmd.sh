#!/bin/bash
# Author: Yogesh Barve
export PATH=$PATH:/home/vagrant/c2wt-dev/dependency/omnetpp/omnetpp-4.6/bin
export RTI_HOME=/home/vagrant/c2wt-dev/dependency/portico/portico-2.1.0
export INETMANET_HOME=/home/vagrant/c2wt-dev/dependency/inet/inetmanet-2.0-inetmanet-2.2/
export CONFIGNAME='gcc-debug'


export LD_LIBRARY_PATH=$RTI_HOME/jre/lib/amd64/server/
export OMNET_PATH=$PWD/target/classes

cd target/bin-exec/

#./target/bin-exec/MapperEx-OmnetFederate-cpp -u Cmdenv -c General -n MapperEx.ini
./MapperEx-OmnetFederate-cpp -u Cmdenv -c General -n MapperEx.ini