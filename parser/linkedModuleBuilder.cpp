#include "linkedModuleBuilder.h"



map<string, Module*>* LinkedModuleBuilder::buildLinkedModule(objectBuilderOutput* objBuilderOutput)
{

    map<string, Module*>* modules = new map<string, Module*>();

    for(map<circuitName, circuitProperties>::iterator currentCircuitIt  = objBuilderOutput->begin(); currentCircuitIt != objBuilderOutput->end(); ++currentCircuitIt)
    {
        //starts to create a new module
        moduleInstanceParams currentModuleInstanceParams;
        currentModuleInstanceParams.moduleName = currentCircuitIt->first;
         
        //first parsing for creating instances of objects
        for(circuitElementCollection::iterator elt = currentCircuitIt->second.elements.begin(); elt != currentCircuitIt->second.elements.end(); elt++)
        {
            //if the element contained in the module is an input or an output,
            //simply add it to the list of I/O elements name, 
            //creation of corresponding NODE_CONNs is handeled by the Module class.
            if(elt->second.elementType == "INPUT" || elt->second.elementType == "input")
                currentModuleInstanceParams.moduleInputNames.emplace_back(elt->first);

            else if(elt->second.elementType == "OUTPUT" || elt->second.elementType == "output")
                currentModuleInstanceParams.moduleOutputNames.emplace_back(elt->first);

            //If the element is not an I/O, instanciate and add the element to the module content.
            else
            {
                currentModuleInstanceParams.modContent.insert(
                    *(new pair<eltName, SIM_NODE*>(
                        elt->first,
                        SIM_NODE::createNewObject(elt->second.elementType, {elt->first})
                        )
                    )
                );
            }
        }


        //at this point, the current module is parametrized, we can instanciate a new module.
        
        Module* currentModuleInstance = new Module(currentModuleInstanceParams);


        //we need to link the module content together.


        //second parsing for linkling module content together
        for(circuitElementCollection::iterator elt = currentCircuitIt->second.elements.begin(); elt != currentCircuitIt->second.elements.end(); elt++)
        {
            //cyling through all input elements of the current elt
            for(map<connectedElementName, portName>::iterator eltLinkIt = elt->second.inputElements.begin(); eltLinkIt != elt->second.inputElements.end(); eltLinkIt++)
            {
                cout << "Element '" + eltLinkIt->first + "' connects to port '" + eltLinkIt->second + "' of element '" + elt->first + "'" << endl;

                string currentElementName = elt->first;
                string currentElementPort = eltLinkIt->second;
                string elementToLinkTo = eltLinkIt->first;


                //if the current element connects to the module input
                if(binary_search(currentModuleInstance->getModuleInputNames()->begin(),
                                 currentModuleInstance->getModuleInputNames()->end(),
                                 elementToLinkTo))

                currentModuleInstance->getContent()->find(currentElementName)->second->connIn
                (
                    currentElementPort,
                    &(currentModuleInstance->getModuleInputConns()->find(elementToLinkTo)->second)
                );
                
                

                else if(binary_search(currentModuleInstance->getModuleOutputNames()->begin(),
                                      currentModuleInstance->getModuleOutputNames()->end(),
                                      currentElementName))

                currentModuleInstance->getInternalModuleOutputs()->insert
                (
                    pair<string, NODE_CONN<LOGICSTATE>*>(
                        currentElementName,
                        currentModuleInstance->getContent()->find(elementToLinkTo)->second->getOutConn(PRIMGATE_STD_OUT)
                    )
                );

                else
                {
                    currentModuleInstance->getContent()->find(currentElementName)->second->connIn(
                        currentElementPort,
                        currentModuleInstance->getContent()->find(elementToLinkTo)->second->getOutConn(PRIMGATE_STD_OUT)
                    );
                }
                
            }
        }

        modules->insert(
            pair<string, Module*>(
                currentModuleInstance->getName(),
                currentModuleInstance   
            )
        );

    }
    
    return modules;
}




