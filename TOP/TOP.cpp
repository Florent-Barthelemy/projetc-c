#include "TOP.h"
#include "../utils/systemMessages.h"

using namespace std;

SystemMessager topLogger("TOP");

int main(int argc, char** argv)
{
    try
    {
        int iResult;
        fstream HDLFile("src/circuit.dot", ios_base::in);
        fstream stimuliFile("src/stimulis.json", ios_base::in);

        map<string, Module*>* circuits;
        STIMULI_HANDLER stimulis;

        registerGates();

        {
            ObjectBuilder modulesBuilder(true);
            LEXED_LIST lex;
            DELIM_MAP delims;

            configDOTBuilder(modulesBuilder, delims);

            iResult = ::lex(HDLFile, delims, lex);
            objectBuilderOutput buildOut = modulesBuilder.buildCircuit(&lex);
            circuits = LinkedModuleBuilder({true}).buildLinkedModule(&buildOut);
        }

        {
            WAVEDROM_JSON(jsonWavedrom);
            parseJSON(stimuliFile, jsonWavedrom);
            generateStimuliData(jsonWavedrom, stimulis);
        }

        for(map<string, Module*>::iterator circuitIT = circuits->begin(); circuitIT != circuits->end(); ++circuitIT)
            for(map<string, NODE_CONN<LOGICSTATE>*>::iterator inputsIT = circuitIT->second->getInputs().begin(); inputsIT != circuitIT->second->getInputs().end(); ++inputsIT)
            {
                circuitIT->second->connIn(inputsIT->first, stimulis.getNodeConn(inputsIT->first));
                
                cout << inputsIT->first << "|";
            }

        for(map<string, Module*>::iterator circuitIT = circuits->begin(); circuitIT != circuits->end(); ++circuitIT)
            for(map<string, NODE_CONN<LOGICSTATE>>::iterator outputsIT = circuitIT->second->getOutputs().begin(); outputsIT != circuitIT->second->getOutputs().end(); ++outputsIT)
            {
                cout << outputsIT->first << "|";
            }

        for(long iTimestamp = 0; iTimestamp < 10; iTimestamp++)
        {
            stimulis.updateStimuliNodes(iTimestamp);
            cout << endl;

            for(map<string, Module*>::iterator circuitIT = circuits->begin(); circuitIT != circuits->end(); ++circuitIT)
                for(map<string, NODE_CONN<LOGICSTATE>*>::iterator inputsIT = circuitIT->second->getInputs().begin(); inputsIT != circuitIT->second->getInputs().end(); ++inputsIT)
                {
                    cout << inputsIT->second->state << "|";
                }

            for(map<string, Module*>::iterator circuitIT = circuits->begin(); circuitIT != circuits->end(); ++circuitIT)
                for(map<string, NODE_CONN<LOGICSTATE>>::iterator outputsIT = circuitIT->second->getOutputs().begin(); outputsIT != circuitIT->second->getOutputs().end(); ++outputsIT)
                {
                    cout << outputsIT->second.state << "|";
                }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}