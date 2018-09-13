//
// Created by yogesh on 8/31/17.
//
#include "FederateConfigParser.h"

namespace po = boost::program_options;


bool FederateConfigParser::to_bool(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    std::istringstream is(str);
    bool b;
    is >> std::boolalpha >> b;
    return b;
}


void FederateConfigParser::parseJson(std::string configFileName, FederateConfig *obj) {

    // Short alias for this namespace
    namespace pt = boost::property_tree;

    // Create a root
    pt::ptree iroot;

    try {
        // Load the json file in this ptree
        pt::read_json(configFileName, iroot);
    }
    catch (pt::json_parser_error &e) {
        std::cerr << e.what() << std::endl;
        return;
    }


    ////////////////////
    /// READING      ///
    ////////////////////

    std::cout << "Reading " << configFileName << std::endl;

    //Read values


    try {
        obj->stepSize = iroot.get<double>("stepSize");
    } catch (pt::ptree_error &e) {
        std::cerr << e.what() << std::endl;
    }
    try {
        obj->lookAhead = iroot.get<double>("lookAhead");

    } catch (pt::ptree_error &e) {
        std::cerr << e.what() << std::endl;
    }
    try {
        obj->federationId = iroot.get<std::string>("federationId");

    } catch (pt::ptree_error &e) {
        std::cerr << e.what() << std::endl;
    }
    try {
        obj->federateType = iroot.get<std::string>("federateType");

    } catch (pt::ptree_error &e) {
        std::cerr << e.what() << std::endl;
    }
    try {
        obj->isLateJoiner = this->to_bool(iroot.get<std::string>("isLateJoiner"));
    } catch (pt::ptree_error &e) {
        std::cerr << e.what() << std::endl;
    }
    try {
        obj->federateRTIInitWaitTimeMs = iroot.get<int>("federateRTIInitWaitTimeMs");
    } catch (pt::ptree_error &e) {
        std::cerr << e.what() << std::endl;
    }

}

FederateConfig* FederateConfigParser::parseArgs(const int argc, char *argv[]) {
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help,h", "Help screen")
            ("configFile", po::value<std::string>()->implicit_value("configfile.json"),
             "This is the config file.json for federate configuration")
            ("federateType", po::value<std::string>()->implicit_value("unknownFederateType"),
             "Type of the Federate, eg: source, sink,pingcounter")
            ("federationId", po::value<std::string>()->implicit_value("unknownFederationName"),
             "Current Federation Name")
            ("isLateJoiner", po::value<std::string>()->implicit_value("false"),
             "Whether the federate is LateJoiner? default: false")
            ("lookAhead", po::value<double>()->implicit_value(0.0), "Lookahead values, default 0.0")
            ("stepSize", po::value<double>()->implicit_value(1.0), "Simulation step size, default:1.0");

    po::variables_map vm;

    try {
        po::store(
                po::parse_command_line(argc, argv, desc),
                vm
        );
    }
    catch (po::error const &e) {
        std::cerr << e.what() << '\n';
        exit(EXIT_FAILURE);
    }
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << '\n';
        return (FederateConfig *)0;
    }
    FederateConfig *fedConfig = new FederateConfig();
    //! First read the configfile for any information
    if (vm.count("configFile")) {
        std::cout << "configFile: " << vm["configFile"].as<std::string>() << '\n';
        this->parseJson(vm["configFile"].as<std::string>(), fedConfig);

    } else {

        std::cout << "Default configFile: " << fedConfig->configFileName << '\n';
        this->parseJson(fedConfig->configFileName, fedConfig);

    }


//    std::cout << "lookahead: " << fedConfig->lookAhead << " stepSize: " << fedConfig->stepSize
//              << " federateType: " << fedConfig->federateType << " federationID: " << fedConfig->federationId
//              << " isLateJoiner: " << std::boolalpha << fedConfig->isLateJoiner << std::endl;
//



    if (vm.count("federateType"))
        fedConfig->federateType = vm["federateType"].as<std::string>();

    if (vm.count("federationId"))
        fedConfig->federationId = vm["federationId"].as<std::string>();

    if (vm.count("isLateJoiner"))
        fedConfig->isLateJoiner = this->to_bool(vm["isLateJoiner"].as<std::string>());

    if (vm.count("lookAhead"))
        fedConfig->lookAhead = vm["lookAhead"].as<double>();

    if (vm.count("stepSize"))
        fedConfig->stepSize = vm["stepSize"].as<double>();


    std::cout << "lookahead: " << fedConfig->lookAhead << " stepSize: " << fedConfig->stepSize
              << " federateType: " << fedConfig->federateType << " federationID: " << fedConfig->federationId
              << " isLateJoiner: " << std::boolalpha << fedConfig->isLateJoiner << std::endl;

    return fedConfig;

}
