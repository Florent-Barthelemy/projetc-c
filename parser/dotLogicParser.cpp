#include "dotLogicParser.h"


DotLogicParser::DotLogicParser(DotLogicParserConfig conf)
{
    builder = new ObjectBuilder(conf.objBuilderConfig);
    logicLinker = new LinkedModuleBuilder(conf.moduleLinkerConfig);

    //delimiters param config for lexer
	ADD_DELIM(delims, " ", DISCARD);
	ADD_DELIM(delims, "{", SAVE);
	ADD_DELIM(delims, "}", SAVE);
	ADD_DELIM(delims, "\t", DISCARD);
	ADD_DELIM(delims, "[", SAVE);
	ADD_DELIM(delims, ".", SAVE);
	ADD_DELIM(delims, "]", SAVE);
	ADD_DELIM(delims, "=", SAVE);
	ADD_DELIM(delims, ";", SAVE);
	ADD_DELIM(delims, "\n", DISCARD);
	ADD_DELIM(delims, "\"", TOGGLE_STRING);
	ADD_DELIM(delims, "->", SAVE);
	ADD_DELIM(delims, "(", SAVE);
	ADD_DELIM(delims, ")", SAVE);

    /* =========== ObjectBuilder Setup ===========*/
 	//delimiters & keywords param config for ObjectBuilder
	builder->associateToken(token::DIGRAPH, "digraph");
	builder->associateToken(token::SPACE, " ");
	builder->associateToken(token::GRAPH_BLOCK_BEGIN, "{");
	builder->associateToken(token::GRAPH_BLOCK_END, "}");
	builder->associateToken(token::ELEMENT_PARAMS_BEGIN, "[");
	builder->associateToken(token::ELEMENT_PARAMS_END, "]");
	builder->associateToken(token::EQUAL_SIGN, "=");
	builder->associateToken(token::END_LINE_OF_CODE, ";");
	builder->associateToken(token::LINKAGE, "->");
	builder->associateToken(token::QUOTES, "\"");
	builder->associateToken(token::INPUT_PORT_SPECIFIER_START, "(");
	builder->associateToken(token::INPUT_PORT_SPECIFIER_END, ")");
	builder->associateToken(token::OBJECT_SPECIFIER, ".");
	builder->associateToken(token::USING, "using");
	builder->associateToken(token::MODULE, "module");
	
	builder->associateTypeToken(typeToken::INPUT, "INPUT");
	builder->associateTypeToken(typeToken::OUTPUT, "OUTPUT");

	builder->associateElementFeild(elementFeildInitializer::LABEL, "label");
	builder->associateElementFeild(elementFeildInitializer::__DOT_COMPATIBLE_CLOCK_LINK, "clk");
	builder->associateElementFeild(elementFeildInitializer::__DOT_COMPATIBLE_SEL_LINK, "sel");
	
	builder->associateTypeToken(typeToken::__DOT_COMPATIBLE_MUX2, "MUX2");
	builder->associateTypeToken(typeToken::__DOT_COMPATIBLE_DFF, "DFF");
	builder->associateTypeToken(typeToken::__DOT_COMPATIBLE_DLATCH, "DLATCH");

    /* =========== Registering Types creators ===========*/
    SIM_NODE::registerType("INV", invCreator);
	SIM_NODE::registerType("AND", andCreator);
	SIM_NODE::registerType("NAND", nandCreator);
	SIM_NODE::registerType("OR", orCreator);
	SIM_NODE::registerType("NOR", norCreator);
	SIM_NODE::registerType("XOR", xorCreator);
	SIM_NODE::registerType("NXOR", nxorCreator);
	SIM_NODE::registerType("INPUT", buffCreator);
	SIM_NODE::registerType("DFF", dffCreator);
	SIM_NODE::registerType("DLATCH", dlatchCreator);
	SIM_NODE::registerType("MUX2", muxCreator);
}

map<linkedModuleName, Module*>* DotLogicParser::dot2simLogic(std::string mainFileName, std::string compName)
{
    
	fstream tb0(mainFileName, ios_base::in);
    LEXED_LIST lexedList;

	auto code = lex(tb0, delims, lexedList);
	if(code != LEX_SUCCESS)
		messager->ERROR<ios_base::failure>("Lexer error, probable bad syntax");


    objectBuilderOutput circuitsList = builder->buildCircuit(&lexedList);
    

    //adding the compilation to finished compilations list.
    auto compCheck = finishedCompilations.insert(*(new pair<compilationName, dotLogicParserOutput>(
        compName,
        logicLinker->buildLinkedModule(&circuitsList)
    )));

    if(compCheck.second == false)
        messager->ERROR<runtime_error>("Cannot add compilation '" + compName + "' to history, a previous compilation already exists undser that name.");


    return compCheck.first->second;
}

DotLogicParser::~DotLogicParser()
{
    delete builder;
    delete logicLinker;
    delete messager;
}