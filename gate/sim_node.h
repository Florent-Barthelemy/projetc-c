#ifndef SIM_NODE_H_
#define SIM_NODE_H_

#include "../graph/graph.h"
#include "logic.h"
#include <vector>
#include <string>
#include "../utils/namedObj.h"
#include "../utils/typeRegisterer.h"

/**
Params sent to the modules contructors
*/
struct INIT_PARAM
{
    std::string nodeName;
    
};

class SIM_NODE : public NODE<LOGICSTATE>, public NAMEDOBJ, public TYPE_REG<SIM_NODE, INIT_PARAM, std::string>
{
private:
    std::map<std::string, LOGICSTATE> prevState;

public:
    SIM_NODE(std::string name, std::vector<std::string> inputNames, std::vector<std::string> outputNames);

    /**
    Updates the current state of modules ports.
    */
    virtual void updateState() = 0;

    /**
    Updates the current state of this module and sends event to connected modules.
    */
    virtual void updateGate();
};

#endif