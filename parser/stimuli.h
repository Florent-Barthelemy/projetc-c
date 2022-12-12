#ifndef STIMULI_H_
#define STIMULI_H_

#include <map>
#include <list>
#include "../gate/logic.h"
#include "../graph/graph.h"
#include <exception>
#include <stdexcept>
#include "../gate/sim_node.h"

struct STIMULI
{
    long timestamp;
    LOGICSTATE newState;
};

typedef std::map<std::string, std::pair<NODE_CONN<LOGICSTATE>, std::list<STIMULI>>> STIMULI_MAP;

class STIMULI_HANDLER
{
private:
    STIMULI_MAP stimulis;
    NODE_CONN<LOGICSTATE> defNodeConn = {Z};

public:
    NODE_CONN<LOGICSTATE>* addStimuli(std::string netName, std::list<STIMULI> stimulis);
    NODE_CONN<LOGICSTATE>* getNodeConn(std::string netName);
    void updateStimuliNodes(long timestamp);
};

#endif