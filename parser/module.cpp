#include "module.h"

void Module::checkConnectivity_internal()
{
    for(auto elementIt = getContent()->begin(); elementIt != getContent()->end(); ++elementIt)
    {
       map<string, NODE_CONN<LOGICSTATE>*> inpts = elementIt->second->getInputs();

       for(map<string, NODE_CONN<LOGICSTATE>*>::const_iterator inputPortIt = inpts.begin(); inputPortIt != inpts.end(); ++inputPortIt)
       {
            if(inputPortIt->second == 0x00)
                messager.WARNING("Input port '" + inputPortIt->first + "' of element '" + elementIt->first + "' in module '" + this->getName() +"' is floating, simulation will default its state to X");
       }
    }
}

void Module::checkConnectivity_IO()
{
    //checking module I/O connectivity problems
    for(auto it = this->getModuleInputNames()->begin(); it != this->getModuleInputNames()->end(); ++it)
    {
        auto cInputConn = this->getInConn(*it);
        if(*&cInputConn == 0x00)
            messager.WARNING("Input port '" + *it + "' of module '" + this->getName() 
                    + "' is floating, simulation will default its state to X.");
    }
}