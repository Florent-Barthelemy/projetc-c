#include "stimuli.h"
#include "../utils/systemMessages.h"

SystemMessager stimuliLogger("Stimuli");

NODE_CONN<LOGICSTATE>* STIMULI_HANDLER::addStimuli(std::string netName, std::vector<TIMED_SIG> stimulis)
{
    std::pair<STIMULI_MAP::iterator, bool> insertElement = this->stimulis.insert({netName, {{X, {}}, stimulis}});

    if(!insertElement.second)
        stimuliLogger.ERROR<std::domain_error>("Stimuli \"" + netName + "\" already exists.");

    return &insertElement.first->second.first;
}

NODE_CONN<LOGICSTATE>* STIMULI_HANDLER::getNodeConn(std::string netName)
{
    STIMULI_MAP::iterator srchStimuli = this->stimulis.find(netName);
    if(srchStimuli == this->stimulis.end())
    {
        stimuliLogger.WARNING("Failed to find stimuli for input \"" + netName + "\". Connecting to Z state.");
        return &defNodeConn;
    }

    return &srchStimuli->second.first;
}

void STIMULI_HANDLER::updateStimuliNodes(long timestamp)
{
    for(STIMULI_MAP::iterator it = this->stimulis.begin(); it != stimulis.end(); ++it)
        for(std::vector<TIMED_SIG>::iterator lit = it->second.second.begin(); lit != it->second.second.end(); ++lit)
            if(lit->timestamp >= timestamp)
            {
                if(lit->timestamp > timestamp && lit != it->second.second.begin())
                    lit--;
                else if(lit->timestamp > timestamp)
                    break;
                
                it->second.first.state = lit->state;
                for(std::list<NODE<LOGICSTATE>*>::iterator nodeIT = it->second.first.outputs.begin(); nodeIT != it->second.first.outputs.end(); ++nodeIT)
                    static_cast<SIM_NODE*>(*nodeIT)->updateGate();

                break;
            }
}