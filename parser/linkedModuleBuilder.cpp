#include "linkedModuleBuilder.h"



Module* LinkedModuleBuilder::buildLinkedModule(objectBuilderOutput* objBuilderOutput)
{

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

                string currentElementName = eltLinkIt->first;
                string currentElementPort = eltLinkIt->second;
                string elementNameOfTheConecteed;
                NODE_CONN<LOGICSTATE>* nodeToConnect;

                // //if the element at which we connect to (the connecteed) is an input
                // if(binary_search(currentModuleInstance->getModuleInputNames()->begin(),
                //                  currentModuleInstance->getModuleInputNames()->end(),
                //                  elementNameOfTheConecteed))

                // cout << "'" + elementNameOfTheConecteed + "' is an input" << endl;
                
                // else if(binary_search(currentModuleInstance->getModuleOutputNames()->begin(),
                //                       currentModuleInstance->getModuleOutputNames()->end(),
                //                       elementNameOfTheConecteed))

                // cout << "'" + elementNameOfTheConecteed + "' is an output" << endl;

                // else
                // {
                //     nodeToConnect = currentModuleInstance->getContent()->find(elt->first)->second->getOutConn(PRIMGATE_STD_OUT);

                //     currentModuleInstance->getContent()->find(currentElementName)->connIn(
                //         currentElementPort,
                //         nodeToConnect);
                // }

                
            
            
            //     //finding if the incomming connection is an input of the module.
            //     if(binary_search(currentModuleInstance->getModuleInputNames()->begin(), currentModuleInstance->getModuleInputNames()->end(),  eltLinkIt->first))
            //         currentModuleInstance->getContent()->at(elt->first)->connIn(
            //             eltLinkIt->second,
            //             &(currentModuleInstance->getModuleInputConns()->at(eltLinkIt->first))
            //         );

            //     //finding if the incomming connection is an output of the module (it means feedback).
            //     else if(binary_search(currentModuleInstance->getModuleOutputNames()->begin(), currentModuleInstance->getModuleOutputNames()->end(),  eltLinkIt->first))
            //         currentModuleInstance->getContent()->at(elt->first)->connIn(
            //             eltLinkIt->second,
            //             currentModuleInstance->getOutConn(eltLinkIt->first)
            //         );

            //     //otherwise, it is a gate to link inside the module
            //     else
            //     {
            //         currentModuleInstance->getContent()->at(elt->first)->connIn(
            //         eltLinkIt->second,
            //         currentModuleInstance->getContent()->at(eltLinkIt->first)->getOutConn(PRIMGATE_STD_OUT)
            //         );
            //     }
                
            }
        }



    }
    
    moduleInstanceParams p;
    p.moduleName = "SUPER MODULE";
    p.moduleInputNames = {"A", "B"};
    p.moduleOutputNames = {"S1", "s2"};

    

    Module* builtModule = new Module(p);

  


    return builtModule;
}




