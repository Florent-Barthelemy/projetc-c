#ifndef WINDY_H_
#define WINDY_H_


#include "../parser/dotLogicParser.h"
#include "../utils/systemMessages.h"
#include "../parser/wavedrom/wavedrom.h"
#include "../utils/traps.h"
#include <functional>
#include <stdexcept>
#include "help.h"
#include <set>

int main(int argc, char** argv);


typedef std::function<void(char**, int)> argHandler;

struct ARG
{
    private:
        int uid;
        static int uidCounter;
    public:
        string fullArgName;
        string aliasName;
        int argArgsCount;

        argHandler handler;
        
        string helpMessage;

        static void resetUidCounter() { uidCounter = 0; };
        void setup() {uid = uidCounter++;}
        const int getUid() const { return uid; }
};

bool operator<(const ARG& o1, const ARG& o2)
{
    return o1.fullArgName < o2.fullArgName;
}

bool operator>(const ARG& o1, const ARG& o2)
{
    return o1.fullArgName > o2.fullArgName;
}

struct argRegister
{
    private :
        map<string, ARG> argCbkMap;
        SystemMessager* messager;
    
    public :

        const map<string, ARG>& getMap() { return argCbkMap; }
        const ARG* getArgAt(string argName)
        {
            auto charARGptr = argCbkMap.find(argName);
            
            if(charARGptr == argCbkMap.end())
                messager->ERROR<runtime_error>("Argument '" + argName + "' is not registered, check command syntax.");
            
            //returns the corresponding ARG
            return &charARGptr->second; 
            
        }

        void linkMessager(SystemMessager* m)
        {
            messager = m;
        }

        void addArg(ARG* arg)
        {
            //adding full arg name
            auto addedArgFull = argCbkMap.insert(*(new pair<string, ARG>(
                arg->fullArgName,
                *arg
            )));

            //adding alias arg name
            auto addedArgAlias = argCbkMap.insert(*(new pair<string, ARG>(
                arg->aliasName,
                *arg
            )));

            if(addedArgFull.second == false || addedArgAlias.second == false)
                throw new runtime_error("Argument already exists");
        }
};

void printArgs(char** args, int len)
{
    for(int i = 0; i < len; i++)
    {
        std::cout << "[" << *(args + i) << "]" << endl;
    }
}

struct simParams
{
    public:
        static DotLogicParserConfig dotLogParserConfig;
        
        static string compilationName;
        static string dotFileToParse;

        static string stimuliFile;

        static string outputWavedromFileName;

        static bool compilOnly;
        static bool showWaveViewer;

        static unsigned long maxSimulationTimestamp;

        static bool helpShown;
};

//===================== ARGS HANDLER DEFINITIONS =====================//

void showHelpMessage(argRegister reg)
{
    std::set<ARG> args;

    cout << "\n wind [args]\n";
    for(auto argIt = reg.getMap().begin(); argIt != reg.getMap().end(); ++argIt)
    {
        args.insert(argIt->second);
    } 

    for (auto setIT = args.begin(); setIT != args.end(); ++setIT)
    {
        cout << "\t" << setIT->fullArgName << ", " << setIT->aliasName
             << ": " << setIT->helpMessage << endl;
    }
}

void setSimParamsDefaultValues()
{
    simParams::dotLogParserConfig.objBuilderConfig.isDotSyntaxCompatible = false;
}

void dotCompatArgHandler(char** argArgs, int argArgsCount)
{
    simParams::dotLogParserConfig.objBuilderConfig.isDotSyntaxCompatible = true;
}   

void dotFileArgHandler(char** argArgs, int argArgsCount)
{
    simParams::dotFileToParse = argArgs[0];
}

void stimuliFileArgHandler(char** argArgs, int argArgsCount)
{
    simParams::stimuliFile = argArgs[0];
}

void compilationNameArgHandler(char** argArgs, int argArgsCount)
{
    simParams::compilationName = argArgs[0];
}

void deltaCyclesArgHandler(char** argArgs, int argArgsCount)
{
    SIM_NODE::setMaxDelta(stoi(argArgs[0]));
}

void wavedromFileNameOutArgHandler(char** argArgs, int argArgsCount)
{
    simParams::outputWavedromFileName = argArgs[0];
}

void simulationTimeArgHandler(char** argArgs, int argArgsCount)
{
    simParams::maxSimulationTimestamp = stoll(argArgs[0]);
}

void helpArgHandler(char** argArgs, int argArgsCount)
{
    simParams::helpShown = true;
}

void compilationOnlyArgHandler(char** argArgs, int argArgsCount)
{
    simParams::compilOnly = true;
}

void showGuiSimResultsArgHandler(char** argArgs, int argArgsCount)
{
    if(simParams::outputWavedromFileName.size() == 0)
    {
        simParams::outputWavedromFileName = "outwave.json";
    }

    simParams::showWaveViewer = true;
}


#endif