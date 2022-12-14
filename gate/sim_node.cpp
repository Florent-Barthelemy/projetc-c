#include "sim_node.h"
#include "../utils/systemMessages.h"

SystemMessager simLogger("SimCore");

//default value
unsigned short SIM_NODE::maxDeltaCycle = 10;

SIM_NODE::SIM_NODE(std::string name, std::vector<std::string> inputNames, std::vector<std::string> outputNames) :
    NODE(inputNames, outputNames), NAMEDOBJ(name)
{
    for(auto setIT = this->outputs.begin(); setIT != this->outputs.end(); setIT++)
        setIT->second.state = X;
}

void SIM_NODE::updateGate()
{
    memStates();
    updateState();

    if(!isSameState())
    {
        this->deltaCycle++;
        if(this->deltaCycle >= SIM_NODE::maxDeltaCycle)
            simLogger.ERROR<std::out_of_range>("Max delta cycle reached in module \"" + this->getName() + "\".");

        for(std::map<std::string, NODE_CONN<LOGICSTATE>>::iterator myOutputs = this->outputs.begin(); myOutputs != this->outputs.end(); myOutputs++)
        {
            for(std::list<NODE<LOGICSTATE>*>::iterator connOutputs = myOutputs->second.outputs.begin(); connOutputs != myOutputs->second.outputs.end(); connOutputs++)
            {
                static_cast<SIM_NODE*>(*connOutputs)->updateGate();
            }
        }
    }

    this->deltaCycle = 0;
}

void SIM_NODE::memStates()
{
    prevOutStates.clear();

    for(std::map<std::string, NODE_CONN<LOGICSTATE>>::iterator myOutputs = this->outputs.begin(); myOutputs != this->outputs.end(); myOutputs++)
    {
        prevOutStates.insert({myOutputs->first, myOutputs->second.state});
    }
}

bool SIM_NODE::isSameState()
{
    for(std::map<std::string, NODE_CONN<LOGICSTATE>>::iterator myOutputs = this->outputs.begin(); myOutputs != this->outputs.end(); myOutputs++)
    {
        std::map<std::string, LOGICSTATE>::iterator savedState = this->prevOutStates.find(myOutputs->first);

        if(savedState == this->prevOutStates.end())
            simLogger.ERROR<std::domain_error>("Internal error.");

        if(savedState->second != myOutputs->second.state)
            return false;
    }

    return true;
}

void SIM_NODE::connIn(std::string portName, NODE_CONN<LOGICSTATE>* conn)
{
    try
    {
        NODE::connIn(portName, conn);
    }
    catch(const std::exception& e)
    {
        simLogger.ERROR<std::invalid_argument>("Input port \"" + portName + "\" does not exist for module \"" + this->getName() + "\"");
    }
}

NODE_CONN<LOGICSTATE>* SIM_NODE::getInConn(std::string portName)
{
    try
    {
        return NODE::getInConn(portName);
    }
    catch(const std::exception& e)
    {
        simLogger.ERROR<std::invalid_argument>("Input port \"" + portName + "\" does not exist for module \"" + this->getName() + "\"");
    }

    return NULL;
}

NODE_CONN<LOGICSTATE>* SIM_NODE::getOutConn(std::string portName)
{
    try
    {
        return NODE::getOutConn(portName);
    }
    catch(const std::exception& e)
    {
        simLogger.ERROR<std::invalid_argument>("Output port \"" + portName + "\" does not exist for module \"" + this->getName() + "\"");
    }

    return NULL;
}

SIM_NODE* SIM_NODE::createNewObject(std::string identifier, INIT_PARAM param)
{
    try
    {
        return TYPE_REG::createNewObject(identifier, param);
    }
    catch(const std::exception& e)
    {
        simLogger.ERROR<std::invalid_argument>("Gate \"" + identifier + "\" is not a valid gate type.");
    }

    return NULL;
}