#include "sim_node.h"

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
            throw std::overflow_error("Max delta cycle reached.");

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
            throw std::overflow_error("Internal error.");

        if(savedState->second != myOutputs->second.state)
            return false;
    }

    return true;
}