#include "../gate/module.h"
#include "../gate/PRIMLIB/PRIMLIB.h"

MODULE* andCreator(INIT_PARAM params)
{
    return new AND("test");
}

void initialiser_tb()
{
    MODULE::registerType("AND", andCreator);

    MODULE* and1 = MODULE::createNewObject("AND", {});
}