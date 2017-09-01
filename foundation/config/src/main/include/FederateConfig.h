//
// Created by yogesh on 8/31/17.
//

#include "iostream"
class FederateConfig {

public:
    FederateConfig(){

    }
    FederateConfig(std::string federateType, std::string federationId, bool isLateJoiner, double lookAhead, double stepSize){
        this->federateType = federateType;
        this->federationId = federationId;
        this->isLateJoiner=isLateJoiner;
        this->lookAhead = lookAhead;
        this->stepSize = stepSize;
    };


    std::string federateType;
    std::string federationId;
    bool isLateJoiner = false;
    long federateRTIInitWaitTimeMs=20;
    double lookAhead = 0.1;
    double stepSize =1 ;
    std::string configFileName = "configfile.json";
};

