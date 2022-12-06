#ifndef MODULE_H_
#define MODULE_H_

#include "../graph/graph.h"
#include "logic.h"
#include <vector>
#include "../utils/namedObj.h"
#include "../utils/typeRegisterer.h"

struct INIT_PARAM
{
    
};

class MODULE : public NODE<LOGICSTATE>, public NAMEDOBJ, public TYPE_REG<MODULE, INIT_PARAM, std::string>
{
public:
    MODULE(std::string name, std::vector<std::string> inputNames, std::vector<std::string> outputNames);

    virtual void updateState() = 0;

    void updateGate();
};

#endif