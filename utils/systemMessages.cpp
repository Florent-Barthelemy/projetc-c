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
    *unifiedMessageStream << "\033[1;35mINFO\033[0m";
    *unifiedMessageStream << " " + messageSeparator + " ";
    *unifiedMessageStream << msg << std::endl;
}

void SystemMessager::DEBUG(std::string msg)
{
    printModuleName();
    *unifiedMessageStream << "\033[1;34mDEBUG\033[0m";
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
