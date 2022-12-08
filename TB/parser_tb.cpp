#include "../parser/lexer.h"
#include "../parser/objectBuilder.h"
#include "../parser/linkedModuleBuilder.h"
#include "../gate/PRIMLIB/PRIMLIB.h"
#include <iostream>


using namespace std;
int parser_tb()
{
	LEXED_LIST lexedList;
	DELIM_MAP delims;

	//instanciation of the ObjectBuilder isVerbose = true
	ObjectBuilder objBuilder(true);

	LinkedModuleBuilderConfig lmBuilderConf;
	lmBuilderConf.isVerbose = true;

	LinkedModuleBuilder lmBuilder(lmBuilderConf);
	
	//delimiters param config for lexer
	ADD_DELIM(delims, " ", DISCARD);
	ADD_DELIM(delims, "{", SAVE);
	ADD_DELIM(delims, "}", SAVE);
	ADD_DELIM(delims, "\t", DISCARD);
	ADD_DELIM(delims, "[", SAVE);
	ADD_DELIM(delims, "]", SAVE);
	ADD_DELIM(delims, "=", SAVE);
	ADD_DELIM(delims, ";", SAVE);
	ADD_DELIM(delims, "\n", DISCARD);
	ADD_DELIM(delims, "\"", TOGGLE_STRING); //SAVE is necessary !
	ADD_DELIM(delims, "->", SAVE);
	ADD_DELIM(delims, "(", SAVE);
	ADD_DELIM(delims, ")", SAVE);


	

	//specifying HDL description location, then open
    string filename = "dot_test_file.dot";
	fstream tb0(filename, ios_base::in);

	//lexing code
	lex(tb0, delims, lexedList);
 
    /* =========== ObjectBuilder Test ===========*/


 	//delimiters & keywords param config for ObjectBuilder
	objBuilder.associateToken(token::DIGRAPH, "digraph");
	objBuilder.associateToken(token::SPACE, " ");
	objBuilder.associateToken(token::GRAPH_BLOCK_BEGIN, "{");
	objBuilder.associateToken(token::GRAPH_BLOCK_END, "}");
	objBuilder.associateToken(token::ELEMENT_PARAMS_BEGIN, "[");
	objBuilder.associateToken(token::ELEMENT_PARAMS_END, "]");
	objBuilder.associateToken(token::EQUAL_SIGN, "=");
	objBuilder.associateToken(token::END_LINE_OF_CODE, ";");
	objBuilder.associateToken(token::LINKAGE, "->");
	objBuilder.associateToken(token::QUOTES, "\"");
	objBuilder.associateToken(token::INPUT_PORT_SPECIFIER_START, "(");
	objBuilder.associateToken(token::INPUT_PORT_SPECIFIER_END, ")");

	objBuilder.associateElementFeild(elementFeildInitializer::LABEL, "label");

	SIM_NODE::registerType("INV", invCreator);
	SIM_NODE::registerType("AND", andCreator);
	SIM_NODE::registerType("NAND", nandCreator);
	SIM_NODE::registerType("OR", orCreator);
	SIM_NODE::registerType("NOR", norCreator);
	SIM_NODE::registerType("XOR", xorCreator);
	SIM_NODE::registerType("NXOR", nxorCreator);
	SIM_NODE::registerType("INPUT", buffCreator);
	
	objectBuilderOutput circuitsList;

    //passing lexedList to the ObjectBuilder
	circuitsList = objBuilder.buildCircuit(&lexedList);



	Module* builtSystemModule = lmBuilder.buildLinkedModule(&circuitsList);
	
	

	return 0;


}