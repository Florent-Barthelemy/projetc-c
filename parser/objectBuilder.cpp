#include "objectBuilder.h"



/* initializes the FSM */
void ObjectBuilder::initialize()
{
     currentState = circuitBuildState::INIT;
     nextState = circuitBuildState::INIT;
}

/* streams the lexedList string into the FSM */
map<circuitName, circuitProperties> ObjectBuilder::buildCircuit(LEXED_LIST* lexedList)
{
    //initializing the state machine
    initialize();
    
    codeSectionCompleted = true;

    if(config.isDotSyntaxCompatible)
        messager->INFO("Compiler is running in dot compatible mode");

    for (LEXED_LIST::iterator it = lexedList->begin(); it != lexedList->end(); it++)
	{
        iterateStateMachine(it); //iterating FSM with a new code element
	}

    if(!codeSectionCompleted)
         messager->SYNATX_ERROR<runtime_error>("Code stops unexpectedly, (missing quotes or end of block?)", lexedList->end()->line);
    

    int tec = getTotalElementCount();
    int circuitCount = builtCircuits.size();
    messager->INFO("\033[1;36mDONE!\033[0m : number of parsed elements : " + to_string(tec) + " > Total circuits : " + to_string(circuitCount));
     
        
    return builtCircuits;
}


int ObjectBuilder::getTotalElementCount()
{
    int ecount = 0;
    for(map<circuitName, circuitProperties>::iterator it = builtCircuits.begin(); it != builtCircuits.end(); it++)
    {
        ecount += it->second.elements.size();
    }

    return ecount;
}

int ObjectBuilder::getElementCountOfCircuit(circuitName n)
{
    return builtCircuits.at(n).elements.size();
}

void ObjectBuilder::iterateStateMachine(LEXED_LIST::iterator it)
{
    //cout << "FSM codeWord : '" << codeWord << "'\n";

    string codeWord = it->cnt;
    map<string, circuitElementProperties> importedBlockModules;
    unsigned long currentLine = it->line;
    
    switch(currentState)
    {

        case circuitBuildState::INIT :
            if(codeWord == tokens.at(DIGRAPH))
                nextState = circuitBuildState::CREATE_NEW_CIRCUIT;
            else
                messager->SYNATX_ERROR<runtime_error>("Bad syntax, expected ' " + tokens.at(DIGRAPH)+ " ' got ' "
                                + codeWord + " ' instead.",currentLine);
        break;
        
        case circuitBuildState::CREATE_NEW_CIRCUIT:
            /* digraph keyword has been received, trying to
               create a new circuit with a name corresponding 
            to the current word*/

            codeSectionCompleted = false;

            if(isWordReserved(codeWord))
                messager->SYNATX_ERROR<runtime_error>("digraph name " + codeWord + " is either a token or reserved.",currentLine);
                //program stops, syntax error
            else
            {
                //Syntax is OK, instanciate a new circuit
                currentCircuit = *(new circuit());

                currentCircuit.first = codeWord;

                messager->DEBUG("Creating circuit, name : " + codeWord);

                nextState = circuitBuildState::DESCRIPTION_BEGIN;
            }

        break;

        case circuitBuildState::DESCRIPTION_BEGIN :
            if(codeWord != tokens.at(GRAPH_BLOCK_BEGIN))
                messager->SYNATX_ERROR<runtime_error>("' " + codeWord + " ' Invalid digraph block initializer.",currentLine);
            else
                messager->DEBUG("Starting '" + currentCircuit.first + "' code analysis");
                nextState = circuitBuildState::READ_NEW_CIRCUIT_HDL_LINE_FIRST;
        break;


        case circuitBuildState::READ_NEW_CIRCUIT_HDL_LINE_FIRST :

            if(codeWord == tokens.at(END_LINE_OF_CODE))
                //if line is empty
                nextState = currentState;

            else if(codeWord == tokens.at(USING))
                nextState = circuitBuildState::USING_STATEMENT;


            else if(codeWord == tokens.at(GRAPH_BLOCK_END))
            {
                //the current module/circuit description ends
                if(codeWord == tokens.at(GRAPH_BLOCK_END))
                {
                    auto elt = builtCircuits.insert(*(new circuit(currentCircuit)));
                    if(elt.second == false)
                        messager->SYNATX_ERROR<runtime_error>("Double definition of circuit ' " + elt.first->first + "'.",currentLine);
                   
                    for(auto it = elt.first->second.elements.begin(); it != elt.first->second.elements.end(); ++it)
                    {
                        if(it->second.elementType == typeTokens.at(INPUT)) //if an output is detected in the module, add it to the inputs vector
                            elt.first->second.inputElements.insert(
                                *(new pair<string, string>(
                                    it->first,
                                    ""
                                ))); //no need to check bouble output def.
                        
                        if(it->second.elementType == typeTokens.at(OUTPUT)) //if an output is detected in the module
                            elt.first->second.outputElements.insert(
                                *(new pair<string, string>(
                                    it->first,
                                    ""
                                ))); //no need to check bouble output def.
                    }

                    codeSectionCompleted = true;
                }
                else
                    messager->SYNATX_ERROR<runtime_error>("Unexpected token '" + codeWord + "' after end of block.",currentLine);

                nextState = circuitBuildState::DESCRIPTION_END;
            }
            else if(isWordReserved(codeWord))
                //new line starts by a reserved word
                messager->SYNATX_ERROR<runtime_error>("Element name '" + codeWord + "' is either a token or reserved.",currentLine);
            else
            {
                codeWordStack.push(codeWord);
                nextState = circuitBuildState::READ_NEW_CIRCUIT_HDL_LINE_SECOND;
            }
                  
        break;

        case circuitBuildState::USING_STATEMENT:
            if(codeWord == tokens.at(MODULE))
                nextState = circuitBuildState::IMPORT_MODULE;
            else
                messager->SYNATX_ERROR<runtime_error>("Unkown import type '" + codeWord + "'.", currentLine);
        break;

        case circuitBuildState::IMPORT_MODULE:
            if(builtCircuits.find(codeWord) == builtCircuits.end())
                messager->SYNATX_ERROR<runtime_error>("Undefined reference to module '" + codeWord + "'.", currentLine);
            
            currentCircuit.second.elements.insert
            (
                *(new pair<string, circuitElementProperties>(
                    codeWord, //name of the imported module
                    *(new circuitElementProperties({})
                    )
                ))
            );
            
            messager->UNIMPLEMENTED("Module import is not yet implemented.");

            nextState = circuitBuildState::LINE_OF_CODE_END;
        break;

        case circuitBuildState::READ_NEW_CIRCUIT_HDL_LINE_SECOND :
            if(codeWord == tokens.at(LINKAGE)) //if linkage info
            {
                messager->DEBUG("Found new Linkage info on '" + codeWordStack.front() + "'.");
                nextState = circuitBuildState::ADD_ELEMENT_LINK_INFO;
            }

            //else if it is a new element
            else if(codeWord == tokens.at(ELEMENT_PARAMS_BEGIN)) 
            {
                currentCircuitElement = *(new circuitElement);
                currentCircuitElement.first = codeWordStack.front();

                messager->DEBUG("Created new element '" + currentCircuitElement.first + "' in '" + currentCircuit.first + "' circuit");
                
                codeWordStack.pop();
                nextState = circuitBuildState::FEILD_INITIALIZER;
            }

            else //if neither, it is a bad syntax
                messager->SYNATX_ERROR<runtime_error>("Invalid properties feild token '" + codeWord + "' after '" + codeWordStack.front() + "' element.",currentLine);

            break;

        

        //TODO - ADD ELEMENT LINK INFO
        case circuitBuildState::ADD_ELEMENT_LINK_INFO :
        {
            //check if -> operands (elements) exists
            bool leftOpDoNotEsist = false;
            bool rightOpDoNotEsist = false;


            //if the left element is neither declared in the current module or in a previously compiled one
            circuitElementCollection::iterator leftElement = currentCircuit.second.elements.find(codeWordStack.front());
            if(leftElement == currentCircuit.second.elements.end()) 
                leftOpDoNotEsist = true;

            //if the right element is neither declared in the current module or in a previously compiled one
            circuitElementCollection::iterator rightElement = currentCircuit.second.elements.find(codeWord);
            if(rightElement == currentCircuit.second.elements.end())
                rightOpDoNotEsist = true;
            
            if(rightOpDoNotEsist)
               //Right operand does not exist
               messager->SYNATX_ERROR<runtime_error>("LinkageError : can't link object '" + codeWord
                              + "' with object '" + codeWordStack.front()  + "', as '" + codeWord + "' is not declared.",currentLine);

            else if(leftOpDoNotEsist)
               //Left operand does not exist
               messager->SYNATX_ERROR<runtime_error>("LinkageError : can't link object '" + codeWordStack.front()
                              + "' with object '" +  codeWord + "', as '" + codeWordStack.front() + "' is not declared.",currentLine);

            //at this point, codeWordStack contains the left operand and the right operand.
            //next step is to check for a port specifier.

            //compatibility workaround
            if(config.isDotSyntaxCompatible)
            {
                //if element is a latch
                if(leftElement->second.elementType == typeTokens.at(__DOT_COMPATIBLE_DFF) ||
                    leftElement->second.elementType == typeTokens.at(__DOT_COMPATIBLE_DLATCH))
                {

                }
                else //element has n inputs, n outputs
                {
                    string rightElementPortName = "IN" + to_string(rightElement->second.__DOT_COMPATIBLE_INPUT_COUNTER);
                    

                    //adding link info
                    auto rightInCheck = rightElement->second.inputElements.find(leftElement->first);
                    
                    
                    if(rightInCheck != rightElement->second.inputElements.end())
                        messager->SYNATX_ERROR<runtime_error>("Output of '" + leftElement->first + "' is already connected to '" + currentCircuitElement.first, currentLine);
                    
                    
                    else
                    {
                        leftElement->second.outputElements.insert(pair<connectedElementName, portName>(rightElement->first,"OUT"));
                        rightElement->second.inputElements.insert(pair<connectedElementName, portName>(leftElement->first, rightElementPortName));
                        rightElement->second.__DOT_COMPATIBLE_INPUT_COUNTER++;
                    }
                }   
                codeWordStack.pop();
                nextState = circuitBuildState::LINE_OF_CODE_END;
            }

            else
            {
                //at this point, codeWordStack contains the left operand and the right operand.
                //next step is to check for a port specifier.
                codeWordStack.push(codeWord);
                nextState = circuitBuildState::INPUT_PORT_SPECIFIER_BEGIN;
            } 
            
        }
        break;


        case circuitBuildState::INPUT_PORT_SPECIFIER_BEGIN :
            if(codeWord == tokens.at(END_LINE_OF_CODE) && config.isDotSyntaxCompatible)
                nextState = circuitBuildState::__DOT_COMPATIBLE_LINK_ELEMENTS;
                

            else if(codeWord != tokens.at(INPUT_PORT_SPECIFIER_START) && !config.isDotSyntaxCompatible)
                messager->SYNATX_ERROR<runtime_error>("LinkageError : Missing port specifier after '" + codeWordStack.back() + "', got '" 
                               + codeWord + "' instead.",currentLine);
            
            else
                nextState = circuitBuildState::LINK_ELEMENTS;
        break;


        case circuitBuildState::LINK_ELEMENTS :
        {
            string _portName = codeWord;
            
            string leftOperand = codeWordStack.front();
            codeWordStack.pop();

            string rightOperand = codeWordStack.front();
            codeWordStack.pop();

            //if the port name is reserved, error
            if(isWordReserved(codeWord))
                messager->SYNATX_ERROR<runtime_error>("'" + leftOperand +   "' cannot have a port name which is a token or a reserved name, got '" + _portName + "'.",currentLine);

            messager->DEBUG("LINKAGE :: '" + leftOperand +   "' output connects to port '" + _portName + "' of '" + rightOperand + "'");

            //at this point, we know that the operands exists and are either a module of a circuit elt
            //if the right operand is a module, handle special case :

            circuitElementCollection::iterator leftElement = currentCircuit.second.elements.find(leftOperand);
            circuitElementCollection::iterator rightElement = currentCircuit.second.elements.find(rightOperand);
            
             /*


                Checking if two inputs are connected together, keeping this check
                in intermediate parsing for easier implementation of Hi-Z logic on top.


            */
           map<connectedElementName, portName>::iterator elt = rightElement->second.inputElements.find(leftOperand);
        
        for(auto inputEltIt = rightElement->second.inputElements.begin(); inputEltIt != rightElement->second.inputElements.end(); ++inputEltIt)
            {
                if(_portName == inputEltIt->second)
                {
                    messager->WARNING("Output of '" + leftOperand + "' connected with output of '" + inputEltIt->first
                    + "' in module '" + currentCircuit.first + "'.");

                    messager->UNIMPLEMENTED("Hi-Z logic is not implemented");
                }
            }

            //add leftElement link info (output info)
            leftElement->second.outputElements.insert(pair<connectedElementName, portName>(rightElement->first, "OUT"));

            //add rightElement link info (input info)
            rightElement->second.inputElements.insert(pair<connectedElementName, portName>(leftElement->first, _portName));
            
            


            nextState = circuitBuildState::INPUT_PORT_SPECIFIER_END;
        }
        break;

        case circuitBuildState::INPUT_PORT_SPECIFIER_END :
            if(codeWord != tokens.at(INPUT_PORT_SPECIFIER_END))
                messager->SYNATX_ERROR<runtime_error>("Invalid port specifier end token, have'" + codeWord 
                               + "' expected '" + tokens.at(INPUT_PORT_SPECIFIER_END) + "'.",currentLine);
            else
                nextState = circuitBuildState::LINE_OF_CODE_END;
        break;


        case circuitBuildState::LINE_OF_CODE_END :
            if(codeWord != tokens.at(END_LINE_OF_CODE))
                messager->SYNATX_ERROR<runtime_error>("Missing '"+ tokens.at(END_LINE_OF_CODE) 
                               + "' got unexpected token '" + codeWord + "' instead.",currentLine);
            else
                nextState = circuitBuildState::READ_NEW_CIRCUIT_HDL_LINE_FIRST;
        break;


        /* When a new initializer has been detected, this state first checks if it
           is followed by a property initializer (=) and then redirects to the corresponding state*/
        case circuitBuildState::FEILD_INITIALIZER :
            if(codeWord == tokens.at(ELEMENT_PARAMS_END))
                nextState = circuitBuildState::FEILD_INITIALIZER_END;
            
            else if(!isAnElementFeildInitializer(codeWord))
                messager->SYNATX_ERROR<runtime_error>("Invalid initializer : '" + codeWord + "' is not a valid property",currentLine);
            
            else
            {
                //here, codeWord is a valid and well syntaxed feild initializer,
                //next step is to check if it has an assign sybol next
                //codeWordStack.pop();
                codeWordStack.push(codeWord);
                nextState = circuitBuildState::FEILD_INITIALIZER_ASSIGN;
            }
        break;


        case circuitBuildState::FEILD_INITIALIZER_ASSIGN :
                if(codeWord != tokens.at(EQUAL_SIGN))
                    messager->SYNATX_ERROR<runtime_error>("Missing assign symbol after initializer ' " + codeWordStack.front() + " ', expected ' " 
                                    + tokens.at(EQUAL_SIGN) + "' instead.",currentLine);
                else
                {
                    //find what feild to init, the initializer name
                    //is stored into the codeWordStack
                    
                    //if it is a label feild
                    if(codeWordStack.front() == elementFeildInitializers.at(LABEL)) 
                        nextState = circuitBuildState::SET_ELEMENT_LABEL;
                    
                    //if initializer is a select signal link and it is a mux (compat mode only)
                    else if(codeWordStack.front() == elementFeildInitializers.at(__DOT_COMPATIBLE_SEL_LINK) &&
                        currentCircuitElement.second.elementType == typeTokens.at(typeToken::__DOT_COMPATIBLE_MUX2) && config.isDotSyntaxCompatible)
                        nextState = circuitBuildState::__DOT_COMPPATIBLE_LINK_SELECT_MUX2;

                    

                }

        break;

        case circuitBuildState::__DOT_COMPPATIBLE_LINK_SELECT_MUX2:
            if(currentCircuitElement.second.__DOT_COMPATIBLE_MUX_SEL_CONNECTED == true)
                messager->SYNATX_ERROR<runtime_error>("Cannot add '" + elementFeildInitializers.at(__DOT_COMPATIBLE_SEL_LINK) 
                                                + "' feild initializer to '" + currentCircuitElement.first + "' feild is already assigned", currentLine);
            
            else if(currentCircuit.second.elements.find(codeWord) == currentCircuit.second.elements.end())
                messager->SYNATX_ERROR<runtime_error>("Can't link sel signal of MUX2 '" + currentCircuitElement.first + "' to '" + codeWord + "' as it is not declared.", currentLine);
            
            else
            {
                
                currentCircuitElement.second.inputElements.insert(*(new pair<connectedElementName, portName>(
                        codeWord,
                        PRIMGATE_STD_MUX2_IN_SEL
                )));

                currentCircuitElement.second.__DOT_COMPATIBLE_MUX_SEL_CONNECTED = true;
            }

            //check if sel signal exists,
            //it it exists, add sel signal with standard sel input.

            //removing the initializer name from the stack,
            //it is not needed anymore.
            codeWordStack.pop();

            //next property read
            nextState = circuitBuildState::FEILD_INITIALIZER;
        break;

        /* codeWord is the element name */
        case circuitBuildState::SET_ELEMENT_LABEL : 
            if(isWordReserved(codeWord))
                messager->SYNATX_ERROR<runtime_error>("Invalid [" + elementFeildInitializers.at(LABEL) 
                               + "] property value ' " + codeWord + " '",currentLine);
            else
            {
                //if the property has an unreserved name, assign 'type' property
                currentCircuitElement.second.elementType = codeWord;

                //removing the initializer name from the stack,
                //it is not needed anymore.
                codeWordStack.pop();

                //Next property read
                nextState = circuitBuildState::FEILD_INITIALIZER;
            }
        break;

        case circuitBuildState::FEILD_INITIALIZER_END:
            if(codeWord != tokens.at(END_LINE_OF_CODE))
                messager->SYNATX_ERROR<runtime_error>("Missing '"+ tokens.at(END_LINE_OF_CODE) 
                               + "' got unexpected token '" + codeWord + "' instead.",currentLine);
            else
            {
                //new circuit element is a well constructed object, 
                //add currentCircuitElement to the current circuit.

                pair<circuitElementCollection::iterator, bool > elt = currentCircuit.second.elements.insert(*(new circuitElement(currentCircuitElement)));
                if(elt.second == false)
                    //If the elements already exists
                    //elt.first->first corresponds to the currently existing element name
                    messager->SYNATX_ERROR<runtime_error>("Double definition of object '" + elt.first->first 
                                   + "' in circuit '" + currentCircuit.first + "', object is already defined with type ' " + elt.first->second.elementType + " '.",currentLine);
                
                else
                    //if the element is a new element, proceed with next line.
                    nextState = circuitBuildState::READ_NEW_CIRCUIT_HDL_LINE_FIRST;
            }
        break;

        case circuitBuildState::DESCRIPTION_END :

            //try to insert a circuit/module into the built list
            if(codeWord == tokens.at(DIGRAPH))
                nextState = circuitBuildState::CREATE_NEW_CIRCUIT;
            else
                messager->SYNATX_ERROR<runtime_error>("Unexpected token '" + codeWord + "' after end of block.",currentLine);
            
        break;

    }

     currentState = nextState;

}


//could switch map<token, string> to map<token, string>* 
bool ObjectBuilder::isWordReserved(string s)
{
    for(map<token, string>::iterator it = tokens.begin(); it != tokens.end(); it++)
    {
        if(it->second == s)
            return true;
    }



    return isAnElementFeildInitializer(s);
}

bool ObjectBuilder::isAnElementFeildInitializer(string s)
{
    for(map<elementFeildInitializer, string>::iterator it = elementFeildInitializers.begin(); it != elementFeildInitializers.end(); it++)
    {
        if(it->second == s)
            return true;
    }

    return false;
}




