#ifndef SYS_MSG_H_
#define SYS_MSG_H_

#include <string>
#include <stdexcept>
#include <iostream>
#include <exception>

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
    
        /// @brief Sets the messager output stream
        /// @param os The messaging stream
        static void setMessagerStream(std::ostream* os) { unifiedMessageStream = os; }

        /// @brief Sets the separator printed between the type and the message 
        /// @param s Separator
        static void setMessageSeparator(std::string s) { messageSeparator = s; }

        

    private:
        /// @brief Name of the sender (your current system block name)
        std::string sender;

        /// @brief The messaging stream shared by all instances of the messager
        static std::ostream* unifiedMessageStream;

        /// @brief The separator printed between the type and the message 
        static std::string messageSeparator;

        void printModuleName();
};

template <typename ERR>
void SystemMessager::SYNATX_ERROR(std::string msg, unsigned long line)
{
    printModuleName();
    *unifiedMessageStream << "\033[1;31mERROR\033[0m @line " << line;
    *unifiedMessageStream << " " + messageSeparator + " ";
    *unifiedMessageStream << msg << std::endl;
    throw new ERR(msg);
}

template <typename ERR>
void SystemMessager::ERROR(std::string msg)
{
    printModuleName();
    *unifiedMessageStream << "\033[1;31mERROR\033[0m";
    *unifiedMessageStream << " " + messageSeparator + " ";
    *unifiedMessageStream << msg << std::endl;
    throw new ERR(msg);
}






#endif /*SYS_MSG_H_*/