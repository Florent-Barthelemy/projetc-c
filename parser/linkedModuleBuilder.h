#ifndef LINKED_MODULE_BUILDER_H_
#define LINKED_MODULE_BUILDER_H_

#include "../gate/PRIMLIB/PRIMLIB.h"
#include "../gate/sim_node.h"
#include "module.h"
#include <algorithm>
#include "objectBuilder.h"
#include <stdexcept>
#include <exception>
#include "../utils/systemMessages.h"


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



        //ERROR while trying to delete messager
        // ~LinkedModuleBuilder()
        // {
        //     delete &messager;
        // }

    private:
        SystemMessager messager = *(new SystemMessager("LinkedModuleBuilder")); 
        bool isVerbose;

        void check_IO_Connectivity(Module& mod);


};




#endif /*LINKED_MODULE_BUILDER_H_*/