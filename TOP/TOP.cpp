#include "TOP.h"
#include "../utils/systemMessages.h"

using namespace std;

SystemMessager topLogger("TOP");

int main(int argc, char** argv)
{
    int iResult;
    fstream HDLFile("../TOP/src/circuit.dot", ios_base::in);
    fstream stimuliFile("../TOP/src/stimulis.json", ios_base::in);
    fstream acquisitionFile("../TOP/src/acquisition.json", ios_base::out | ios_base::trunc);

    try
    {
        map<string, Module*>* circuits;
        STIMULI_HANDLER stimulis;
        ACQUISITION_HANDLER acquisition;
        WAVEDROM_JSON(jsonWavedrom);

        registerGates();

        {
            ObjectBuilder modulesBuilder({true, false});
            LEXED_LIST lex;
            DELIM_MAP delims;

            configDOTBuilder(modulesBuilder, delims);

            iResult = ::lex(HDLFile, delims, lex);
            objectBuilderOutput buildOut = modulesBuilder.buildCircuit(&lex);
            circuits = LinkedModuleBuilder({true}).buildLinkedModule(&buildOut);
        }

        {
            parseJSON(stimuliFile, jsonWavedrom);
            generateStimuliData(jsonWavedrom, stimulis);
        }

        for(map<string, Module*>::iterator circuitIT = circuits->begin(); circuitIT != circuits->end(); ++circuitIT)
            for(map<string, NODE_CONN<LOGICSTATE>*>::iterator inputsIT = circuitIT->second->getInputs().begin(); inputsIT != circuitIT->second->getInputs().end(); ++inputsIT)
                circuitIT->second->connIn(inputsIT->first, stimulis.getNodeConn(inputsIT->first));

        for(map<string, Module*>::iterator circuitIT = circuits->begin(); circuitIT != circuits->end(); ++circuitIT)
            for(map<string, NODE_CONN<LOGICSTATE>>::iterator outputsIT = circuitIT->second->getOutputs().begin(); outputsIT != circuitIT->second->getOutputs().end(); ++outputsIT)
                acquisition.registerAcquisition(&outputsIT->second, outputsIT->first);

        for(long iTimestamp = 0; iTimestamp < 4; iTimestamp++)
        {
            stimulis.updateStimuliNodes(iTimestamp);
            acquisition.acquire(iTimestamp);
        }

        generateWavedromSigs(acquisition, jsonWavedrom);
        writeJSON(acquisitionFile, jsonWavedrom);

        QApplication a(argc, argv);
        MainWindow window(acquisition, stimulis);
        window.show();
        a.exec();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    HDLFile.close();
    stimuliFile.close();
    acquisitionFile.close();

    return 0;
}