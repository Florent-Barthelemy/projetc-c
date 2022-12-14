#include "../parser/lexer.h"
#include "../parser/objectBuilder.h"
#include "../parser/linkedModuleBuilder.h"
#include "../gate/PRIMLIB/PRIMLIB.h"
#include <iostream>
#include "../utils/systemMessages.h"
#include "../utils/traps.h"

using namespace std;

int parser_tb()
{
	

	LEXED_LIST lexedList;
	DELIM_MAP delims;

	//instanciation of a messager for the test bench
	SystemMessager messager("parser_tb");

	SystemMessager::setShownMessages({_WARN, _ERROR, _UNIMP, _INFO, _ALL});
	
	SystemMessager::setTrapCallbackForMessage(_ERROR, TRAPS::__TRAP__ERROR);
	SystemMessager::setTrapCallbackForMessage(_SYNTAX_ERR, TRAPS::__TRAP__ERROR);
	SystemMessager::setTrapCallbackForMessage(_WARN, TRAPS::__TRAP__WARN);
	SystemMessager::setTrapCallbackForMessage(_UNIMP, TRAPS::__TRAP__UNIMP);




	//instanciation of the ObjectBuilder isVerbose = true
	auto objBuilderConfig = *(new objectBuilderConfig());
	objBuilderConfig.isDotSyntaxCompatible = true;
	objBuilderConfig.isVerbose = false;

	ObjectBuilder objBuilder(objBuilderConfig);

	LinkedModuleBuilderConfig lmBuilderConf;
	lmBuilderConf.isVerbose = true;

	LinkedModuleBuilder lmBuilder(lmBuilderConf);
	
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


	

	//specifying HDL description location, then open
    string filename = "dot_test_file.dot";
	fstream tb0(filename, ios_base::in);

	//lexing code
	auto code = lex(tb0, delims, lexedList);
	if(code != LEX_SUCCESS)
		messager.ERROR<ios_base::failure>("Lexer error, probable bad syntax");
	
 
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
	objBuilder.associateToken(token::OBJECT_SPECIFIER, ".");
	objBuilder.associateToken(token::USING, "using");
	objBuilder.associateToken(token::MODULE, "module");
	
	objBuilder.associateTypeToken(typeToken::INPUT, "INPUT");
	objBuilder.associateTypeToken(typeToken::OUTPUT, "OUTPUT");

	objBuilder.associateElementFeild(elementFeildInitializer::LABEL, "label");
	objBuilder.associateElementFeild(elementFeildInitializer::__DOT_COMPATIBLE_CLOCK_LINK, "clk");
	objBuilder.associateElementFeild(elementFeildInitializer::__DOT_COMPATIBLE_SEL_LINK, "sel");
	
	objBuilder.associateTypeToken(typeToken::__DOT_COMPATIBLE_MUX2, "MUX2");
	objBuilder.associateTypeToken(typeToken::__DOT_COMPATIBLE_DFF, "DFF");
	objBuilder.associateTypeToken(typeToken::__DOT_COMPATIBLE_DLATCH, "DLATCH");

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

	SIM_NODE::setMaxDelta(100);

	objectBuilderOutput circuitsList;

    //passing lexedList to the ObjectBuilder
	circuitsList = objBuilder.buildCircuit(&lexedList);



	map<string, Module*>* builtSystemModule = lmBuilder.buildLinkedModule(&circuitsList);

		
    NODE_CONN<LOGICSTATE> A = {X, {}};
    NODE_CONN<LOGICSTATE> B = {X, {}};

	NODE_CONN<LOGICSTATE> CLK = {X, {}};

	map<string, Module*>::iterator it = builtSystemModule->find("NAND_MODULE");
	if(it == builtSystemModule->end())
		messager.ERROR<ios::failure>("Cicruit do not exist");
	
	
	it->second->connIn("A", &A);
	it->second->connIn("B", &B);
	it->second->connIn("CLOCK", &CLK);


	//Check if the module is properly connected :
	it->second->checkConnectivity_IO();

	A.state = L;
	CLK.state = L;

	cout << "OUTPUT : " << it->second->getOutConn("S")->state << endl;
	it->second->updateGate();

	A.state = H;
	CLK.state = H;


	it->second->updateGate();

	
	cout << "OUTPUT : " << it->second->getOutConn("S")->state << endl;

	A.state = L;
	CLK.state = L;


	it->second->updateGate();

	
	cout << "OUTPUT : " << it->second->getOutConn("S")->state << endl;

	return 0;


}