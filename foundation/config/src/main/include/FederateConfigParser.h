//
// Created by yogesh on 8/31/17.
//

#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "FederateConfig.h"

#include "iostream"
class FederateConfigParser {

private:
    void parseJson(std::string configFileName, FederateConfig *obj);
    bool to_bool(std::string str);

public:
    FederateConfig* parseArgs(const int argc, char *argv[]);
    FederateConfigParser(){};
    ~FederateConfigParser();

};


