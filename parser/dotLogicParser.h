#ifndef DOT_LOGIC_PARSER_H_
#define DOT_LOGIC_PARSER_H_

#include "objectBuilder.h"
#include "linkedModuleBuilder.h"
#include "lexer.h"



struct DotLogicParserConfig{
    objectBuilderConfig objBuilderConfig;
    LinkedModuleBuilderConfig moduleLinkerConfig;
};

typedef map<linkedModuleName, Module*>* dotLogicParserOutput;
typedef string compilationName;

class DotLogicParser
{
    public:
        DotLogicParser(DotLogicParserConfig conf);


        map<linkedModuleName, Module*>* dot2simLogic(std::string mainDotFileName, std::string compName);

        ~DotLogicParser();


    private:
        DotLogicParserConfig config;
        
        SystemMessager* messager = new SystemMessager("dotLogicParser"); 

        //map of delimiters for use in lexer
        DELIM_MAP delims;

        /// @brief Parsing FSM & intermediate form builder
        ObjectBuilder* builder;

        /// @brief Creates intances of logic elements linked together in modules
        LinkedModuleBuilder* logicLinker;

        map<compilationName, dotLogicParserOutput> finishedCompilations;

};

#endif /*DOT_LOGIC_PARSER_H_*/