#ifndef SYS_MSG_H_
#define SYS_MSG_H_

#include <string>
#include <stdexcept>
#include <iostream>
#include <exception>
#include <vector>
#include <map>
#include <functional>

enum messageType
{
    _WARN,
    _INFO,
    _ERROR,
    _SYNTAX_ERR,
    _UNIMP,
    _DEBG,
    _ALL
};

class SystemMessager
{
    public:
        /// @brief General message manager for normalized messaging
        /// @param senderName Name of the message sender class
        /// @param targetStream Target output stream
        SystemMessager(std::string senderName) : sender(senderName) {};

        SystemMessager() = delete;

        /// @brief Sends a warning message to the stream
        ///        with a normalized format
        /// @param msg Message to print
        void WARNING(std::string msg);

        /// @brief Sends an INFO message to to the stream
        ///        with a normalized format
        /// @param msg Message to print
        void INFO(std::string msg);

        /// @brief Sends an error message to the stream
        ///        with a normalized format (throws an exception)
        /// @param msg Message to print
        /// @param ex std::exception to return
        /// @tparam ERR error typename
        template <typename ERR>
        void ERROR(std::string msg);

        /// @brief Sends a .dot syntax error message to the stream
        ///        with a normalized format (throws an exception)
        /// @param msg Message to print
        /// @param ex std::exception to return
        /// @tparam ERR error typename
        template <typename ERR>
        void SYNATX_ERROR(std::string msg, unsigned long line);

        /// @brief Sends an unimplemented function message and throws an exception
        /// @param msg Message to print
        void UNIMPLEMENTED(std::string msg);
        
        /// @brief Sends a debug message to the stream (low level info)
        /// @param msg Debug message
        void DEBUG(std::string msg);

        /*Setters*/
    
        /// @brief Sets the messager output stream
        /// @param os The messaging stream
        static void setMessagerStream(std::ostream* os) { unifiedMessageStream = os; }

        /// @brief Sets the separator printed between the type and the message 
        /// @param s Separator
        static void setMessageSeparator(std::string s) { messageSeparator = s; }

        /// @brief Sets the shown messages types throughout all the instances of he messager
        /// @param messagesToShow {TYPE1, TYPE2, TYPEn} the messages types to show
        static void setShownMessages(std::vector<messageType> messagesToShow) { shownMessages = messagesToShow; };


        /// @brief Sets a custom function to call in case of a message occurence
        /// @param mType Message type
        /// @param trap  Linked trap
        static void setTrapCallbackForMessage(messageType mType, std::function<void(void)> trap) 
        { 
            if(traps.find(mType) == traps.end())
                traps.insert(
                    *(new std::pair<messageType, std::function<void(void)>>(
                        mType,
                        trap
                    ))
                );
            else
                throw new std::invalid_argument("Cannot add new trap for type, it is already defined");           
        };

    private:

        /// @brief Name of the sender (your current system block name)
        std::string sender;

        /// @brief Traps to call in the event of the corresponding message showing up
        static std::map<messageType, std::function<void(void)>> traps;

        /// @brief The messaging stream shared by all instances of the messager
        static std::ostream* unifiedMessageStream;

        /// @brief The separator printed between the type and the message 
        static std::string messageSeparator;

        /// @brief List of messages to show
        static std::vector<messageType> shownMessages;

        static void callTrap(messageType mType)
        {
            try
            {
                auto trapCaller = traps.at(mType);
                trapCaller();
            }
            catch (std::exception){} 
        }

        static bool isShownMessage(messageType mType)
        {
            for(auto showType = shownMessages.begin(); showType != shownMessages.end(); ++showType)
            {
                if(mType == *showType || *showType == _ALL)
                    return true;
            }
            return false;
        }

        void printModuleName();
};

template <typename ERR>
void SystemMessager::SYNATX_ERROR(std::string msg, unsigned long line)
{
    if(isShownMessage(_SYNTAX_ERR))
    {
        printModuleName();
        *unifiedMessageStream << "\033[1;31mERROR\033[0m @line " << line;
        *unifiedMessageStream << " " + messageSeparator + " ";
        *unifiedMessageStream << msg << std::endl;
    }

    callTrap(_SYNTAX_ERR);
    throw new ERR(msg);
}

template <typename ERR>
void SystemMessager::ERROR(std::string msg)
{
    if(isShownMessage(_ERROR))
    {
        printModuleName();
        *unifiedMessageStream << "\033[1;31mERROR\033[0m";
        *unifiedMessageStream << " " + messageSeparator + " ";
        *unifiedMessageStream << msg << std::endl;
    }

    callTrap(_ERROR);
    throw new ERR(msg);
}


#endif /*SYS_MSG_H_*/