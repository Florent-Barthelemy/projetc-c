#include "..\\parser\\lexer.h"
#include <iostream>

using namespace std;
int TB_lexer()
{
	LEXED_LIST lexedList;
	DELIM_MAP delims;

	//to discard
	ADD_DELIM(delims, " ", DISCARD);
	ADD_DELIM(delims, "{", SAVE);
	ADD_DELIM(delims, "}", SAVE);
	ADD_DELIM(delims, "\t", DISCARD);
	ADD_DELIM(delims, "[", SAVE);
	ADD_DELIM(delims, "]", SAVE);
	ADD_DELIM(delims, "=", SAVE);
	ADD_DELIM(delims, ";", SAVE);
	ADD_DELIM(delims, "\n", DISCARD);

	for (DELIM_MAP::iterator it = delims.begin(); it != delims.end(); it++)
	{
		cout << it->first << endl;
	}

	//test
	cout << "************DOT_TB0.dot******************" << endl;
	fstream tb0("..\\parser\\dot_test_file.dot", ios_base::in);

	lex(tb0, delims, lexedList);

	for (list<lined_string>::iterator it = lexedList.begin(); it != lexedList.end(); it++)
	{
		cout << it->cnt << endl;
		cout << "   @" << it->line << endl;
	}

	return 0;
}