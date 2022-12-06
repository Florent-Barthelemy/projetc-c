#ifndef MODULE_H_
#define MODULE_H_

#include "../graph/graph.h"
#include "logic.h"
#include <vector>
#include "../utils/namedObj.h"
#include "../utils/typeRegisterer.h"

/**
Params sent to the modules contructors
*/
struct INIT_PARAM
{
    
};

class MODULE : public NODE<LOGICSTATE>, public NAMEDOBJ, public TYPE_REG<MODULE, INIT_PARAM, std::string>
{
public:
    MODULE(std::string name, std::vector<std::string> inputNames, std::vector<std::string> outputNames);

    /**
    Updates the current state of modules ports.
    */
    virtual void updateState() = 0;

    /**
    Updates the current state of this module and sends event to connected modules.
    */
    void updateGate();
};

#endif