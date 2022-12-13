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

    /**
    Connect input net to NODE_CONN.
    \param portName: name of the port to connect.
    \param conn: NODE_CONN to connect.
    \throw std::invalid_argument when port does not exist.
    */
    void connIn(std::string portName, NODE_CONN<LOGICSTATE>* conn);

    /**
    Gets input port pointer.
    \param portName: name of the port to get.
    \return NODE_CONN driving this net.
    \throw std::invalid_argument when port does not exist.
    */
    NODE_CONN<LOGICSTATE>* getInConn(std::string portName);

    /**
    Gets output port pointer.
    \param portName: name of the port to get.
    \return NODE_CONN driving this net.
    \throw std::invalid_argument when port does not exist.
    */
    NODE_CONN<LOGICSTATE>* getOutConn(std::string portName);

    inline std::map<std::string, NODE_CONN<LOGICSTATE>*>& getInputs() { return NODE::getInputs(); }

    inline std::map<std::string, NODE_CONN<LOGICSTATE>>& getOutputs() { return NODE::getOutputs(); }

    static SIM_NODE* createNewObject(std::string identifier, INIT_PARAM param);
};

#endif