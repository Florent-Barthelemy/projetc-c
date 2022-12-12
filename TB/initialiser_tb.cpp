#include "../gate/sim_node.h"
#include "../gate/PRIMLIB/PRIMLIB.h"

void initialiser_tb()
{
    SIM_NODE::registerType("AND", andCreator);

    SIM_NODE* and1 = SIM_NODE::createNewObject("AND", {});
}