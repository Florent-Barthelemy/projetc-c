#include "lexer.h"
#include "objectBuilder.h"
#include <iostream>

using namespace std;
int main()
{
	list<string> lexedList;
	list<circuit> circuitsList;
	DELIM_MAP delims;

	//instanciation of the ObjectBuilder isVerbose = true
	ObjectBuilder objBuilder(true);
	
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
	ADD_DELIM(delims, "\"", SAVE); //SAVE is necessary !
	ADD_DELIM(delims, "->", SAVE);


	

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

	objBuilder.associateElementFeild(elementFeildInitializer::LABEL, "label");



    //passing lexedList to the ObjectBuilder
	circuitsList = objBuilder.buildCircuit(&lexedList);
	


	return 0;


}