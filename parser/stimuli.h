#ifndef STIMULI_H_
#define STIMULI_H_

#include <map>
#include <list>
#include "../gate/logic.h"
#include "../graph/graph.h"
#include <exception>
#include <stdexcept>
#include "../gate/sim_node.h"

struct TIMED_SIG
{
    long timestamp;
    LOGICSTATE state;
};

//STIMULIS

typedef std::map<std::string, std::pair<NODE_CONN<LOGICSTATE>, std::vector<TIMED_SIG>>> STIMULI_MAP;

class STIMULI_HANDLER
{
private:
    STIMULI_MAP stimulis;
    NODE_CONN<LOGICSTATE> defNodeConn = {Z};

public:
    NODE_CONN<LOGICSTATE>* addStimuli(std::string netName, std::vector<TIMED_SIG> stimulis);
    NODE_CONN<LOGICSTATE>* getNodeConn(std::string netName);
    void updateStimuliNodes(long timestamp);
};

//ACQUISITION

typedef std::map<NODE_CONN<LOGICSTATE>*, std::pair<std::string, std::vector<TIMED_SIG>>> ACQUISITION_MAP;

class ACQUISITION_HANDLER
{
private:
    ACQUISITION_MAP acquisitions;

public:
    void registerAcquisition(NODE_CONN<LOGICSTATE>* conn, std::string name);
    void acquire(long timestamp);
    void saveAcquisition(std::map<std::string, std::vector<TIMED_SIG>>& container);

};

#endif