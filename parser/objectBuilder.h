#ifndef OBJ_BUILDER_H_
#define OBJ_BUILDER_H_

#include <string>
#include <list>
#include <sstream>
#include <map>
#include <queue>  //for codeWord memory
#include <utility>
#include <iostream>
#include "lexer.h"


using namespace std;


typedef string portName;
typedef string connectedElementName;
typedef string circuitName;

/* intermediate 'Circuit element' data structure*/
struct circuitElementProperties
{
    string elementType;

    /*
        Inputs & Outputs objects connected are stored
        into two list<pair<string ObjectName, int inputID>>
        
        inputID specifies which I/O 'pin' number is used
    */
    map<connectedElementName, portName> inputElements;
    map<connectedElementName, portName> outputElements; 
};

/*circuitElementCollection is a list of "elementName" paired to 
its associated properties. Goal is to optimize element search during
code analysis*/
typedef map<string, circuitElementProperties> circuitElementCollection;

/* describes a circuit element as a pair string 'name', circuitElementProperties */
typedef pair<string, circuitElementProperties> circuitElement;

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
    FEILD_INITIALIZER,
    FEILD_INITIALIZER_ASSIGN,
    SET_ELEMENT_LABEL,


    /*Linkage port specifier state*/
    INPUT_PORT_SPECIFIER_BEGIN,
    LINK_ELEMENTS,
    INPUT_PORT_SPECIFIER_END,

    FEILD_INITIALIZER_END,

    DONE_INITIALIZING
};

enum token
{
  SPACE,
  DIGRAPH,
  GRAPH_BLOCK_BEGIN,
  GRAPH_BLOCK_END,
  ELEMENT_PARAMS_BEGIN,
  ELEMENT_PARAMS_END,

  INPUT_PORT_SPECIFIER_START,
  INPUT_PORT_SPECIFIER_END,


  EQUAL_SIGN,
  END_LINE_OF_CODE,
  LINKAGE,
  QUOTES
};

enum elementFeildInitializer
{
    LABEL,
};

/*  The circuitProperties struct defines the content
    of a circuit/module and the location of its inputs and outputs
*/
struct circuitProperties
{
    /*Circuit input interface elements references*/

    /*Circuit output interface elements references*/
    
    

    /*Individual elements containted into the circuit*/
    circuitElementCollection elements;
};

typedef pair<circuitName, circuitProperties> circuit; 


//useful to avoid long definitions
typedef map<circuitName, circuitProperties> objectBuilderOutput;

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

        int getTotalElementCount();

        int getElementCountOfCircuit(circuitName n);

        void initialize();

        // passes lexedList address for memory optimization
        map<circuitName, circuitProperties> buildCircuit(LEXED_LIST* lexedList);

    
    private:
        map<circuitName, circuitProperties> builtCircuits;

        //FSM related variables
        circuit currentCircuit;
        circuitElement currentCircuitElement;
        circuitBuildState currentState;
        circuitBuildState nextState;


        map<token, string> tokens;
        map<elementFeildInitializer, string> elementFeildInitializers;

        /*Small FSM code word memory to allow for syntax distinction*/
        queue<string> codeWordStack;

        bool verbose;
        bool codeSectionCompleted;

        /* method iterates the FSM*/
        void iterateStateMachine(LEXED_LIST::iterator s);

        /*Check if a word "string" is registered as a token or feild initializer*/
        bool isWordReserved(string);

        bool isAnElementFeildInitializer(string s);

        /* user text*/
        void spitErrorAndExit(string message,unsigned long line, int exitCode)
        {
            cout << "ObjectBuilder \033[1;31mERROR\033[0m @line " << line << " > " << message << endl;
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

        void spitReportMessage();
};




#endif /*OBJ_BUILDER_H_*/
