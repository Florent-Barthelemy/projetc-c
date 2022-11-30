#include "objectBuilder.h"



/* initializes the FSM */
void ObjectBuilder::initialize()
{
     currentState = circuitBuildState::INIT;
     nextState = circuitBuildState::INIT;
}



/* streams the lexedList string into the FSM */
list<circuit> ObjectBuilder::buildCircuit(list<string>* lexedList)
{
    //initializing the state machine
    initialize();

    for (list<string>::iterator it = lexedList->begin(); it != lexedList->end(); it++)
	{
        iterateStateMachine(*it); //iterating FSM with a new code element
	}
    //Placeholder
    return *(new list<circuit>());
}


void ObjectBuilder::iterateStateMachine(string codeWord)
{
    //cout << "FSM codeWord : '" << codeWord << "'\n";

    switch(currentState)
    {

        case circuitBuildState::INIT :
            if(codeWord == tokens.at(DIGRAPH))
                nextState = circuitBuildState::CREATE_NEW_CIRCUIT;
            else
                spitErrorAndExit("Bad syntax, expected ' " + tokens.at(DIGRAPH)+ " ' got ' "
                                + codeWord + " ' instead.", -1);
        break;
        
        case circuitBuildState::CREATE_NEW_CIRCUIT:
            /* digraph keyword has been received, trying to
               create a new circuit with a name corresponding 
            to the current word*/

            if(isWordReserved(codeWord))
                spitErrorAndExit("digraph name " + codeWord + " is either a token or reserved.", -1);
                //program stops, syntax error
            else
            {
                //Syntax is OK, instanciate a new circuit
                currentCircuit = *(new circuit());
                currentCircuit.name = codeWord;

                builtCircuits.push_back(currentCircuit);

                spitVerboseMessage("Creating circuit, name : " + codeWord);

                nextState = circuitBuildState::DESCRIPTION_BEGIN;
            }

        break;

        case circuitBuildState::DESCRIPTION_BEGIN :
            if(codeWord != tokens.at(GRAPH_BLOCK_BEGIN))
                spitErrorAndExit("' " + codeWord + " ' Invalid digraph block initializer.", -1);
            else
                spitVerboseMessage("Starting '" + currentCircuit.name + "' code analysis");
                nextState = circuitBuildState::READ_NEW_CIRCUIT_HDL_LINE_FIRST;
        break;


        case circuitBuildState::READ_NEW_CIRCUIT_HDL_LINE_FIRST :
            if(isWordReserved(codeWord))
                spitErrorAndExit("Element name '" + codeWord + "' is either a token or reserved.", -1);
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
                spitVerboseMessage("Found new element '" + codeWordStack.front() + "' in '" + currentCircuit.name + ".");
                nextState = circuitBuildState::CREATE_NEW_CIRCUIT_ELEMENT_START;
            }

            else //if neither, it is a bad syntax
                spitErrorAndExit("Invalid properties feild token '" + codeWord + "' after '" + codeWordStack.front() + "' element.", -1);

            break;
        

        //TODO - ADD ELEMENT LINK INFO
        case circuitBuildState::ADD_ELEMENT_LINK_INFO :
            spitUnimplementedAndExit("'->' Linkage instruction not implemented yet.", -1);
        break;

        case circuitBuildState::CREATE_NEW_CIRCUIT_ELEMENT_START :
            if(!isAnElementFeildInitializer(codeWord))
                spitErrorAndExit("Unexpected feild initializer '" + codeWord + "'", -1);
            else
            {
                
                //instanticate a new circuit element
                circuitElement e(codeWordStack.front(), *(new circuitElementProperties));

                //remove element name from stack
                codeWordStack.pop();

                //add element to current circuit
                currentCircuit.elements.insert(e);

                spitVerboseMessage("Created new circuit element with name : '" + e.first + "'.");

                
                //find what to init
                if(codeWord == elementFeildInitializers.at(LABEL)) //if initializer is a LABEL
                    nextState = circuitBuildState::SET_ELEMENT_LABEL;
            }
        break;

        case circuitBuildState::SET_ELEMENT_LABEL :
            spitUnimplementedAndExit("label initializer not implemented yet.", -1);
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


