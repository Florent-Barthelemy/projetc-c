#include "module.h"

SIM_NODE::SIM_NODE(std::string name, std::vector<std::string> inputNames, std::vector<std::string> outputNames) :
    NODE(inputNames, outputNames), NAMEDOBJ(name)
{
    for(auto setIT = this->outputs.begin(); setIT != this->outputs.end(); setIT++)
        setIT->second.state = X;
}

void SIM_NODE::updateGate()
{
    updateState();

    for(std::map<std::string, NODE_CONN<LOGICSTATE>>::iterator myOutputs = this->outputs.begin(); myOutputs != this->outputs.end(); myOutputs++)
    {
        for(std::list<NODE<LOGICSTATE>*>::iterator connOutputs = myOutputs->second.outputs.begin(); connOutputs != myOutputs->second.outputs.end(); connOutputs++)
        {
            static_cast<SIM_NODE*>(*connOutputs)->updateGate();
        }
    }   
}