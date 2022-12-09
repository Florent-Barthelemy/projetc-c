#ifndef LINKED_MODULE_BUILDER_H_
#define LINKED_MODULE_BUILDER_H_

#include "../gate/PRIMLIB/PRIMLIB.h"
#include "../gate/sim_node.h"
#include "module.h"
#include "../utils/funcs.h"
#include <algorithm>
#include "objectBuilder.h"


struct LinkedModuleBuilderConfig
{
    bool isVerbose;

};

typedef string linkedModuleName;

class LinkedModuleBuilder
{

    public:
        LinkedModuleBuilder(LinkedModuleBuilderConfig conf) : isVerbose(conf.isVerbose) {}

        map<string, Module*>* buildLinkedModule(objectBuilderOutput* objBuilderOutput);

    private:
        bool isVerbose;


};




#endif /*LINKED_MODULE_BUILDER_H_*/