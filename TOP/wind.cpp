#include "wind.h"

DotLogicParserConfig simParams::dotLogParserConfig;
string simParams::compilationName = "comp_0";

string simParams::stimuliFile;
string simParams::dotFileToParse;
string simParams::outputWavedromFileName;
bool simParams::compilOnly = false;
unsigned long simParams::maxSimulationTimestamp = 20;
bool simParams::withGUI = false;

int main(int argc, char** argv)
{
    try
    {
        SystemMessager messager("windSim");


        argRegister argMapper;

        argMapper.linkMessager(&messager);

        ARG dotCompatibleArg;
        dotCompatibleArg.fullArgName = "--dotCompatible";
        dotCompatibleArg.aliasName = "-dc";
        dotCompatibleArg.argArgsCount = 0;
        dotCompatibleArg.handler = dotCompatArgHandler;
        argMapper.addArg(&dotCompatibleArg);

        ARG dotFileArg;
        dotFileArg.fullArgName = "--HDLfile";
        dotFileArg.aliasName = "-f";
        dotFileArg.argArgsCount = 1;
        dotFileArg.handler = dotFileArgHandler;
        argMapper.addArg(&dotFileArg);

        ARG stimuliFileArg;
        stimuliFileArg.fullArgName = "--sti";
        stimuliFileArg.aliasName = "-s";
        stimuliFileArg.argArgsCount = 1;
        stimuliFileArg.handler = stimuliFileArgHandler;
        argMapper.addArg(&stimuliFileArg);

        ARG compilationName;
        compilationName.fullArgName = "--compilHistoryName";
        compilationName.aliasName = "-chn";
        compilationName.argArgsCount = 1;
        compilationName.handler = stimuliFileArgHandler;
        argMapper.addArg(&compilationName);

        ARG deltaCyclesArg;
        deltaCyclesArg.fullArgName = "--setDeltaCycles";
        deltaCyclesArg.aliasName = "-dlt";
        deltaCyclesArg.argArgsCount = 1;
        deltaCyclesArg.handler = deltaCyclesArgHandler;
        argMapper.addArg(&deltaCyclesArg);

        ARG wavedromFileNameOutArg;
        wavedromFileNameOutArg.fullArgName = "--outputFile";
        wavedromFileNameOutArg.aliasName = "-o";
        wavedromFileNameOutArg.argArgsCount = 1;
        wavedromFileNameOutArg.handler = wavedromFileNameOutArgHandler;
        argMapper.addArg(&wavedromFileNameOutArg);

        ARG compilationOnlyArg{
            "--compilationOnly",
            "-c",
            0,
            compilationOnlyArgHandler
        };
        argMapper.addArg(&compilationOnlyArg);
        
        ARG timestampArg = {
            "--timestamp",
            "-t",
            1,
            simulationTimeArgHandler
        };
        argMapper.addArg(&timestampArg);

        ARG helpArg = {
            "--help",
            "-h",
            0,
            helpArgHandler
        };
        argMapper.addArg(&helpArg);


        argv += 1; //starts the argv[0] at [1] element
        argc--;

        //user configuration read
        for(int i = 0; i < argc; i++)
        {
            ARG argToCall = *argMapper.getArgAt(argv[i]);
            
            argToCall.handler(argv+(i+1), argToCall.argArgsCount);

            i += argToCall.argArgsCount;
        }

        


        DotLogicParser parser(simParams::dotLogParserConfig);
        dotLogicParserOutput circuits = parser.dot2simLogic(simParams::dotFileToParse, simParams::compilationName);
    
        if(simParams::compilOnly)
            return 0;

        STIMULI_HANDLER stimulis;
        ACQUISITION_HANDLER acquisition;
        WAVEDROM_JSON(jsonWavedrom);

        //reading stimuli file
        fstream stimuliFile(simParams::stimuliFile, ios_base::in);
        parseJSON(stimuliFile, jsonWavedrom);
        generateStimuliData(jsonWavedrom, stimulis);



        //connecting stimuliManager to circuits
        for(map<string, Module*>::iterator circuitIT = circuits->begin(); circuitIT != circuits->end(); ++circuitIT)
            for(map<string, NODE_CONN<LOGICSTATE>*>::iterator inputsIT = circuitIT->second->getInputs().begin(); inputsIT != circuitIT->second->getInputs().end(); ++inputsIT)
            {
                circuitIT->second->connIn(inputsIT->first, stimulis.getNodeConn(inputsIT->first));
            }

        for(map<string, Module*>::iterator circuitIT = circuits->begin(); circuitIT != circuits->end(); ++circuitIT)
            for(map<string, NODE_CONN<LOGICSTATE>>::iterator outputsIT = circuitIT->second->getOutputs().begin(); outputsIT != circuitIT->second->getOutputs().end(); ++outputsIT)
            {
                acquisition.registerAcquisition(&outputsIT->second, outputsIT->first);
            }

        //checking modules I/O connectivity
        for(auto modulesIt = circuits->begin(); modulesIt != circuits->end(); modulesIt++)
        {
            modulesIt->second->checkConnectivity_IO();
        }

        for(long iTimestamp = 0; iTimestamp < simParams::maxSimulationTimestamp; iTimestamp++)
        {
            stimulis.updateStimuliNodes(iTimestamp);
            acquisition.acquire(iTimestamp);
        }

        fstream acquisitionFile(simParams::outputWavedromFileName, ios_base::out | ios_base::trunc);

        generateWavedromSigs(acquisition, jsonWavedrom);
        writeJSON(acquisitionFile, jsonWavedrom);

        acquisitionFile.close();
        stimuliFile.close();
    }
    catch(const std::exception& e)
    {
        return -1;
    }

    return 0;
}
