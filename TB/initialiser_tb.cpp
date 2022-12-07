#include "../gate/module.h"
#include "../gate/PRIMLIB/PRIMLIB.h"

SIM_NODE* andCreator(INIT_PARAM params)
{
    return new AND("test");
}

void initialiser_tb()
{
    SIM_NODE::registerType("AND", andCreator);

    SIM_NODE* and1 = SIM_NODE::createNewObject("AND", {});
}