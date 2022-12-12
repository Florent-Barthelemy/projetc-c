#include "stimuli.h"
#include "../utils/systemMessages.h"

//STIMULIS

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

//ACQUISITION

SystemMessager acquisitionLogger("Acquisition");

void ACQUISITION_HANDLER::registerAcquisition(NODE_CONN<LOGICSTATE>* conn, std::string name)
{
    std::pair<ACQUISITION_MAP::iterator, bool> insertRes = this->acquisitions.insert({conn, {name, {}}});

    if(!insertRes.second)
        stimuliLogger.WARNING("Acquisition \"" + name + "\" already exists (net already registered).");
}

void ACQUISITION_HANDLER::acquire(long timestamp)
{
    for(ACQUISITION_MAP::iterator acqIT = this->acquisitions.begin(); acqIT != this->acquisitions.end(); ++acqIT)
    {
        if(acqIT->second.second.size() == 0)
            acqIT->second.second.push_back({timestamp, acqIT->first->state});
        else
            if(acqIT->second.second.back().state != acqIT->first->state)
                acqIT->second.second.push_back({timestamp, acqIT->first->state});
    }
}

void ACQUISITION_HANDLER::saveAcquisition(std::map<std::string, std::vector<TIMED_SIG>>& container)
{
    for(ACQUISITION_MAP::iterator acqIT = this->acquisitions.begin(); acqIT != this->acquisitions.end(); ++acqIT)
    {
        if(!container.insert({acqIT->second.first, acqIT->second.second}).second)
            stimuliLogger.ERROR<std::domain_error>("Net name \"" + acqIT->second.first + "\" describes to different nets.");
    }
}