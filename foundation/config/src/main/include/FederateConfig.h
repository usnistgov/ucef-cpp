//
// Created by yogesh on 8/31/17.
//

#ifndef  __FederateConfig__ 
#define __FederateConfig__
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
        this->federateRTIInitWaitTimeMs =20;
        this->configFileName = "configfile.json";

        
    };


    std::string federateType;
    std::string federationId;
    bool isLateJoiner;// = false;
    long federateRTIInitWaitTimeMs;//=20;
    double lookAhead;// = 0.1;
    double stepSize ;//=1 ;
    std::string configFileName ;//= "configfile.json";
};

#endif