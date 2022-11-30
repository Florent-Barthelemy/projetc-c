#ifndef OBJ_BUILDER_H_
#define OBJ_BUILDER_H_

#include <string>
#include <list>
#include <sstream>
#include <map>
#include <queue>  //for codeWord memory
#include <utility>
#include <iostream>
#include "parserStaticData.h"

using namespace std;

/* circuit build FSM states */
enum class circuitBuildState : uint8_t
{
    INIT,

    CREATE_NEW_CIRCUIT,
    SET_CIRCUIT_NAME,

    DESCRIPTION_BEGIN,
    DESCRIPTION_END,

    LINE_OF_CODE_END,

     /*  states combo to check if the new HDL line is either
         linkage info or a new element to create */
    READ_NEW_CIRCUIT_HDL_LINE_FIRST, 
    READ_NEW_CIRCUIT_HDL_LINE_SECOND, 
    
    /*  token was delimiters.LINKAGE */
    ADD_ELEMENT_LINK_INFO, 

    /*  token was delimiters.ELEMENT_PARAMS_BEGIN '['
        instanciates a new circuitElement */
    CREATE_NEW_CIRCUIT_ELEMENT_START, 

    /*  if previous token was delimiters.ELEMENT_PARAMS_END ']' */
    CREATE_NEW_CIRCUIT_ELEMENT_END,

    /*  Sets the type of the newly created element  */
    SET_ELEMENT_LABEL

};

enum token
{
  SPACE,
  DIGRAPH,
  GRAPH_BLOCK_BEGIN,
  GRAPH_BLOCK_END,
  ELEMENT_PARAMS_BEGIN,
  ELEMENT_PARAMS_END,
  EQUAL_SIGN,
  END_LINE_OF_CODE,
  LINKAGE,
  QUOTES
};

enum elementFeildInitializer
{
    LABEL,
};

/* intermediate 'Circuit element' data structure
*/
struct circuitElementProperties
{
    string elementType;

    /*
        Inputs & Outputs objects connected are stored
        into two list<pair<string ObjectName, int inputID>>
        
        inputID specifies which I/O 'pin' number is used
    */

    map<string, int> inputElements;
    map<string, int> outputElements; 
};

/*circuitElementCollection is a list of "elementName" paired to 
its associated properties. Goal is to optimize element search during
code analysis*/
typedef map<string, circuitElementProperties> circuitElementCollection;

/* describes a circuit element as a pair string 'name', circuitElementProperties */
typedef pair<string, circuitElementProperties> circuitElement;


/*  The circuit struct contains 
    the "digraph" name and a list of elements
    contained in it as well as their link to each other
*/
struct circuit
{
    /*Name of the circuit*/
    string name;

    /*Individual elements containted into the circuit*/
    circuitElementCollection elements;
};



class ObjectBuilder
{
    /*
      
    
    */
    public:
        ObjectBuilder(bool isVerbose) : verbose(isVerbose) {}

        void associateToken(token tk, string val) {
            tokens.insert(pair<token, string>(tk, val));
        }

        void associateElementFeild(elementFeildInitializer f, string val) {
            elementFeildInitializers.insert(pair<elementFeildInitializer, string>(f, val));
        }

        void initialize();

        // passes lexedList address for memory optimization
        list<circuit> buildCircuit(list<string>* lexedList);

    
    private:
        list<circuit> builtCircuits;

        //FSM related variables
        circuit currentCircuit;
        circuitBuildState currentState;
        circuitBuildState nextState;


        map<token, string> tokens;
        map<elementFeildInitializer, string> elementFeildInitializers;

        /*Small FSM code word memory to allow for syntax distinction*/
        queue<string> codeWordStack;

        bool verbose;

        /* method iterates the FSM*/
        void iterateStateMachine(string s);

        /*Check if a word "string" is registered as a token or feild initializer*/
        bool isWordReserved(string);

        bool isAnElementFeildInitializer(string s);

        /* user text*/
        void spitErrorAndExit(string message, int exitCode)
        {
            cout << "ObjectBuilder \033[1;31mERROR\033[0m > " << message << endl;
            //terminate the program
            cout << "Exitting...\n";
            exit(exitCode);
        }

        void spitUnimplementedAndExit(string message, int exitCode)
        { 
            if(verbose)
            {
                cout << "ObjectBuilder \033[1;35mUNIMPLEMENTED\033[0m > " << message << endl;
                //terminate the program
                cout << "Exitting...\n";
            }
            exit(exitCode);
        }

        void spitVerboseMessage(string message)
        {
            if(verbose)
                cout << "objectBuilder \033[1;32mINFO\033[0m > " << message << endl;
        }
};




#endif /*OBJ_BUILDER_H_*/
