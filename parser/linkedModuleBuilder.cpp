#include "linkedModuleBuilder.h"



Module* LinkedModuleBuilder::buildLinkedModule(objectBuilderOutput* objBuilderOutput)
{
    





    // Module mod("MOD_1", {"IN1", "IN2"}, {"OUT", "OUT1"});

    //rencontre IN1 label="INPUT"

    
    moduleInstanceParams p;

    p.moduleInputConns.push_back("IN1");
    p.moduleOutputConns.push_back("IN2");
    p.name = "SUPER_MODULE";
    p.moduleContent = *(new map<string, SIM_NODE*>());

    Module* builtModule = new Module(p);

    return builtModule;
}




