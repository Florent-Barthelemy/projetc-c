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

    for (LEXED_LIST::iterator it = lexedList->begin(); it != lexedList->end(); it++)
	{
        iterateStateMachine(it); //iterating FSM with a new code element
	}

    if(!codeSectionCompleted)
         spitErrorAndExit("Code stops unexpectedly, (missing quotes or end of block?)",lexedList->end()->line, -1);
    
    spitReportMessage();
        
    return builtCircuits;
}

void ObjectBuilder::spitReportMessage()
{
    if(verbose)
    {
      int tec = getTotalElementCount();
      int circuitCount = builtCircuits.size();
      cout << "objectBuilder \033[1;35mDONE!\033[0m > number of parsed elements : " + to_string(tec) << endl;
      cout << "                    > Total circuits : " + to_string(circuitCount) << endl;
    }

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
    unsigned long currentLine = it->line;
    
    switch(currentState)
    {

        case circuitBuildState::INIT :
            if(codeWord == tokens.at(DIGRAPH))
                nextState = circuitBuildState::CREATE_NEW_CIRCUIT;
            else
                spitErrorAndExit("Bad syntax, expected ' " + tokens.at(DIGRAPH)+ " ' got ' "
                                + codeWord + " ' instead.",currentLine, -1);
        break;
        
        case circuitBuildState::CREATE_NEW_CIRCUIT:
            /* digraph keyword has been received, trying to
               create a new circuit with a name corresponding 
            to the current word*/

            codeSectionCompleted = false;

            if(isWordReserved(codeWord))
                spitErrorAndExit("digraph name " + codeWord + " is either a token or reserved.",currentLine, -1);
                //program stops, syntax error
            else
            {
                //Syntax is OK, instanciate a new circuit
                currentCircuit = *(new circuit());
                currentCircuit.first = codeWord;

                spitVerboseMessage("Creating circuit, name : " + codeWord);

                nextState = circuitBuildState::DESCRIPTION_BEGIN;
            }

        break;

        case circuitBuildState::DESCRIPTION_BEGIN :
            if(codeWord != tokens.at(GRAPH_BLOCK_BEGIN))
                spitErrorAndExit("' " + codeWord + " ' Invalid digraph block initializer.",currentLine, -1);
            else
                spitVerboseMessage("Starting '" + currentCircuit.first + "' code analysis");
                nextState = circuitBuildState::READ_NEW_CIRCUIT_HDL_LINE_FIRST;
        break;


        case circuitBuildState::READ_NEW_CIRCUIT_HDL_LINE_FIRST :
            if(codeWord == tokens.at(END_LINE_OF_CODE))
                //if line is empty
                nextState = currentState;

            else if(codeWord == tokens.at(GRAPH_BLOCK_END))
            {
                //the current module/circuit description ends
                if(codeWord == tokens.at(GRAPH_BLOCK_END))
                {
                    auto elt = builtCircuits.insert(*(new circuit(currentCircuit)));

                    if(elt.second == false)
                        spitErrorAndExit("Double definition of circuit ' " + elt.first->first + "'.",currentLine, -1);
                   
                    codeSectionCompleted = true;
                }
                else
                    spitErrorAndExit("Unexpected token '" + codeWord + "' after end of block.",currentLine, -1);

                nextState = circuitBuildState::DESCRIPTION_END;
            }
            else if(isWordReserved(codeWord))
                //new line starts by a reserved word
                spitErrorAndExit("Element name '" + codeWord + "' is either a token or reserved.",currentLine, -1);
            else
            {
                codeWordStack.push(codeWord);
                nextState = circuitBuildState::READ_NEW_CIRCUIT_HDL_LINE_SECOND;
            }
                  
        break;

        case circuitBuildState::READ_NEW_CIRCUIT_HDL_LINE_SECOND :
            if(codeWord == tokens.at(LINKAGE)) //if linkage info
            {
                spitVerboseMessage("Found new Linkage info on '" + codeWordStack.front() + "'.");
                nextState = circuitBuildState::ADD_ELEMENT_LINK_INFO;
            }

            //else if it is a new element
            else if(codeWord == tokens.at(ELEMENT_PARAMS_BEGIN)) 
            {
                currentCircuitElement = currentCircuitElement = *(new circuitElement);
                currentCircuitElement.first = codeWordStack.front();

                spitVerboseMessage("Created new element '" + currentCircuitElement.first + "' in '" + currentCircuit.first + "' circuit");
                
                nextState = circuitBuildState::FEILD_INITIALIZER;
            }

            else //if neither, it is a bad syntax
                spitErrorAndExit("Invalid properties feild token '" + codeWord + "' after '" + codeWordStack.front() + "' element.",currentLine, -1);

            break;
        

        //TODO - ADD ELEMENT LINK INFO
        case circuitBuildState::ADD_ELEMENT_LINK_INFO :
        {
            //check if -> operands (elements) exists
            bool leftOpDoNotEsist = false;
            bool rightOpDoNotEsist = false;

            if(currentCircuit.second.elements.find(codeWordStack.front()) == currentCircuit.second.elements.end()) 
                leftOpDoNotEsist = true;
            else if(currentCircuit.second.elements.find(codeWord) == currentCircuit.second.elements.end())
                rightOpDoNotEsist = true;
            
            if(rightOpDoNotEsist)
               //Right operand does not exist
               spitErrorAndExit("LinkageError : can't link object '" + codeWord
                              + "' with object '" + codeWordStack.front()  + "', as '" + codeWord + "' is not declared.",currentLine, -1);

            else if(leftOpDoNotEsist)
               //Left operand does not exist
               spitErrorAndExit("LinkageError : can't link object '" + codeWordStack.front()
                              + "' with object '" +  codeWord + "', as '" + codeWordStack.front() + "' is not declared.",currentLine, -1);

            codeWordStack.push(codeWord);

            //at this point, codeWordStack contains the left operand and the right operand.
            //next step is to check for a port specifier.

            nextState = circuitBuildState::INPUT_PORT_SPECIFIER_BEGIN;
            
        }
        break;


        case circuitBuildState::INPUT_PORT_SPECIFIER_BEGIN :
            if(codeWord != tokens.at(INPUT_PORT_SPECIFIER_START))
                spitErrorAndExit("LinkageError : Missing port specifier after '" + codeWordStack.back() + "', got '" 
                               + codeWord + "' instead.",currentLine, -1);
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
                spitErrorAndExit("'" + leftOperand +   "' cannot have a port name which is a token or a reserved name, got '" + _portName + "'.",currentLine, -1);

            spitVerboseMessage("LINKAGE :: '" + leftOperand +   "' output connects to port '" + _portName + "' of '" + rightOperand + "'");

            //at this point, we know that the operands do exist
            circuitElementCollection::iterator leftElement = currentCircuit.second.elements.find(leftOperand);
            circuitElementCollection::iterator rightElement = currentCircuit.second.elements.find(rightOperand);
            

            //add leftElement link info (output info)
            leftElement->second.outputElements.insert(pair<connectedElementName, portName>(rightElement->first, "OUT"));

            //add rightElement link info (input info)
            rightElement->second.inputElements.insert(pair<connectedElementName, portName>(leftElement->first, _portName));
            
            map<connectedElementName, portName>::iterator elt = rightElement->second.inputElements.find(leftOperand);

            /*

                could check double definitions (two inputs connected together)
                if it is not done on a top level.

                Exception handeled in linkedModuleBuidler.cpp
            
            
            */

            nextState = circuitBuildState::INPUT_PORT_SPECIFIER_END;
        }
        break;

        case circuitBuildState::INPUT_PORT_SPECIFIER_END :
            if(codeWord != tokens.at(INPUT_PORT_SPECIFIER_END))
                spitErrorAndExit("Invalid port specifier end token, have'" + codeWord 
                               + "' expected '" + tokens.at(INPUT_PORT_SPECIFIER_END) + "'.",currentLine, -1);
            else
                nextState = circuitBuildState::READ_NEW_CIRCUIT_HDL_LINE_FIRST;
        break;


        case circuitBuildState::LINE_OF_CODE_END :
            if(codeWord != tokens.at(END_LINE_OF_CODE))
                spitErrorAndExit("Missing '"+ tokens.at(END_LINE_OF_CODE) 
                               + "' got unexpected token '" + codeWord + "' instead.",currentLine, -1);
            else
                nextState = circuitBuildState::READ_NEW_CIRCUIT_HDL_LINE_FIRST;
        break;


        /* When a new initializer has been detected, this state first checks if it
           is followed by a property initializer (=) and then redirects to the corresponding state*/
        case circuitBuildState::FEILD_INITIALIZER :
            if(codeWord == tokens.at(ELEMENT_PARAMS_END))
                nextState = circuitBuildState::FEILD_INITIALIZER_END;
            
            else if(!isAnElementFeildInitializer(codeWord))
                spitErrorAndExit("Invalid initializer : '" + codeWord + "' is not a valid property",currentLine, -1);
            
            else
            {
                //here, codeWord is a valid and well syntaxed feild initializer,
                //next step is to check if it has an assign sybol next
                codeWordStack.pop();
                codeWordStack.push(codeWord);
                nextState = circuitBuildState::FEILD_INITIALIZER_ASSIGN;
            }

        break;


        case circuitBuildState::FEILD_INITIALIZER_ASSIGN :
                if(codeWord != tokens.at(EQUAL_SIGN))
                    spitErrorAndExit("Missing assign symbol after initializer ' " + codeWordStack.front() + " ', expected ' " 
                                    + tokens.at(EQUAL_SIGN) + "' instead.",currentLine, -1);
                else
                {
                    //find what feild to init, the initializer name
                    //is stored into the codeWordStack
                    
                    //if it is a label feild
                    if(codeWordStack.front() == elementFeildInitializers.at(LABEL)) 
                        nextState = circuitBuildState::SET_ELEMENT_LABEL;


                    //removing the initializer name from the stack,
                    //it is not needed anymore.
                    codeWordStack.pop();
                }

        break;

        /* codeWord is the element name */
        case circuitBuildState::SET_ELEMENT_LABEL : 
            if(isWordReserved(codeWord))
                spitErrorAndExit("Invalid [" + elementFeildInitializers.at(LABEL) 
                               + "] property value ' " + codeWord + " '",currentLine, -1);
            else
            {
                //if the property has an unreserved name, assign 'type' property
                currentCircuitElement.second.elementType = codeWord;

                //Next property read
                nextState = circuitBuildState::FEILD_INITIALIZER;
            }
        break;

        case circuitBuildState::FEILD_INITIALIZER_END:
            if(codeWord != tokens.at(END_LINE_OF_CODE))
                spitErrorAndExit("Missing '"+ tokens.at(END_LINE_OF_CODE) 
                               + "' got unexpected token '" + codeWord + "' instead.",currentLine, -1);
            else
            {
                //new circuit element is a well constructed object, 
                //add currentCircuitElement to the current circuit.

                pair<circuitElementCollection::iterator, bool > elt = currentCircuit.second.elements.insert(*(new circuitElement(currentCircuitElement)));
                if(elt.second == false)
                    //If the elements already exists
                    //elt.first->first corresponds to the currently existing element name
                    spitErrorAndExit("Double definition of object '" + elt.first->first 
                                   + "' in circuit '" + currentCircuit.first + "', object is already defined with type ' " + elt.first->second.elementType + " '.",currentLine, -1);
                
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
                spitErrorAndExit("Unexpected token '" + codeWord + "' after end of block.",currentLine, -1);
            
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


