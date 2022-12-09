#include "systemMessages.h" 

std::ostream* SystemMessager::unifiedMessageStream = &std::cout;
std::string SystemMessager::messageSeparator = ">";

void SystemMessager::printModuleName()
{
    *unifiedMessageStream << sender + " ";
}

void SystemMessager::WARNING(std::string msg)
{
    printModuleName();
    *unifiedMessageStream << "\033[1;43;30mWARNING\033[0m";
    *unifiedMessageStream << " " + messageSeparator + " ";
    *unifiedMessageStream << msg << std::endl;
}

void SystemMessager::INFO(std::string msg)
{
    printModuleName();
    *unifiedMessageStream << "\033[1;32mINFO\033[0m";
    *unifiedMessageStream << " " + messageSeparator + " ";
    *unifiedMessageStream << msg << std::endl;
}

void SystemMessager::UNIMPLEMENTED(std::string msg)
{
    printModuleName();
    *unifiedMessageStream << "\033[1;35mUNIMPLEMENTED\033[0m";
    *unifiedMessageStream << " " + messageSeparator + " ";
    *unifiedMessageStream << msg << std::endl;
     std::__throw_bad_function_call();
}

/*
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
        }*/

