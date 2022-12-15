#include "systemMessages.h" 

std::ostream* SystemMessager::unifiedMessageStream = &std::cout;
std::string SystemMessager::messageSeparator = ">";
std::vector<messageType> SystemMessager::shownMessages = {_INFO, _WARN, _ERROR, _SYNTAX_ERR, _UNIMP};
std::map<messageType, std::function<void(void)>> SystemMessager::traps = *(new std::map<messageType, std::function<void(void)>>());


void SystemMessager::printModuleName()
{
    *unifiedMessageStream << sender + " ";
}

void SystemMessager::WARNING(std::string msg)
{
    if(isShownMessage(_WARN))
    {
        printModuleName();
        *unifiedMessageStream << "\033[1;43;30mWARNING\033[0m";
        *unifiedMessageStream << " " + messageSeparator + " ";
        *unifiedMessageStream << msg << std::endl;
    }

    callTrap(_WARN);
}

void SystemMessager::INFO(std::string msg)
{
    if(isShownMessage(_INFO))
    {
        printModuleName();
        *unifiedMessageStream << "\033[1;35mINFO\033[0m";
        *unifiedMessageStream << " " + messageSeparator + " ";
        *unifiedMessageStream << msg << std::endl;
    }

    callTrap(_INFO);
}

void SystemMessager::DEBUG(std::string msg)
{
    if(isShownMessage(_DEBG))
    {
        printModuleName();
        *unifiedMessageStream << "\033[1;34mDEBUG\033[0m";
        *unifiedMessageStream << " " + messageSeparator + " ";
        *unifiedMessageStream << msg << std::endl;
    }

    callTrap(_DEBG);
}

void SystemMessager::UNIMPLEMENTED(std::string msg)
{
    if(isShownMessage(_UNIMP))
    {
        printModuleName();
        *unifiedMessageStream << "\033[1;35mUNIMPLEMENTED\033[0m";
        *unifiedMessageStream << " " + messageSeparator + " ";
        *unifiedMessageStream << msg << std::endl;
    }

    callTrap(_UNIMP);
    throw std::runtime_error(msg);
}


