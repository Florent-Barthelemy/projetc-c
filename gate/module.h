#include "../graph/graph.h"
#include "logic.h"
#include <vector>
#include "../utils/namedObj.h"

class MODULE : public NODE<LOGICSTATE>, public NAMEDOBJ
{
public:
    MODULE(std::string name, std::vector<std::string> inputNames, std::vector<std::string> outputNames);

    virtual void updateState() = 0;

    void updateGate();
};