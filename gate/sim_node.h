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
    std::map<std::string, LOGICSTATE> prevOutStates;
    short deltaCycle = 0;
    static unsigned short maxDeltaCycle;

    void memStates();
    bool isSameState();

public:
    /**
    SIM_NODE constructor.
    \param name: name of the object.
    \param inputNames: names of the inputs ports to create.
    \param outputNames: names of the output ports to create.
    */
    SIM_NODE(std::string name, std::vector<std::string> inputNames, std::vector<std::string> outputNames);

    /**
    Updates the current state of modules ports.
    */
    virtual void updateState() = 0;

    /**
    Updates the current state of this module and sends event to connected modules.
    \throw std::overflow_error when delta cycle limit is reached.
    */
    void updateGate();

    /**
    Sets the max delta cycle allowed for each SIM_NODE.
    \param max: max delta cycle to set.
    */
    static inline void setMaxDelta(unsigned short max) { SIM_NODE::maxDeltaCycle = max; }
};

#endif