#include "stimuli.h"

NODE_CONN<LOGICSTATE>* STIMULI_HANDLER::addStimuli(std::string netName, std::list<STIMULI> stimulis)
{
    std::pair<STIMULI_MAP::iterator, bool> insertElement = this->stimulis.insert({netName, {{X, {}}, stimulis}});

    if(!insertElement.second)
        throw std::out_of_range("Element already exists.");

    return &insertElement.first->second.first;
}

NODE_CONN<LOGICSTATE>* STIMULI_HANDLER::getNodeConn(std::string netName)
{
    STIMULI_MAP::iterator srchStimuli = this->stimulis.find(netName);
    if(srchStimuli == this->stimulis.end())
        throw std::out_of_range("Net name not found in data base");

    return &srchStimuli->second.first;
}

void STIMULI_HANDLER::updateStimuliNodes(long timestamp)
{
    for(STIMULI_MAP::iterator it = this->stimulis.begin(); it != stimulis.end(); ++it)
        for(std::list<STIMULI>::iterator lit = it->second.second.begin(); lit != it->second.second.end(); ++lit)
            if(lit->timestamp > timestamp)
            {
                if(lit != it->second.second.begin())
                {
                    lit--;
                    it->second.first.state = lit->newState;
                    for(std::list<NODE<LOGICSTATE>*>::iterator nodeIT = it->second.first.outputs.begin(); nodeIT != it->second.first.outputs.end(); ++nodeIT)
                        static_cast<SIM_NODE*>(*nodeIT)->updateGate();
                }

                break;
            }
}