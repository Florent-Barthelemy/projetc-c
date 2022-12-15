#ifndef LEXER_H_
#define LEXER_H_

#include <string>
#include <fstream>
#include <list>
#include <utility>

#define LEX_SUCCESS -0x00
#define LEX_FILE_NOT_OPENED -0x01
#define MISSING_END_SEPARATOR -0x02

enum DELIM_MODE : uint8_t
{
	SAVE,
	DISCARD,
	TOGGLE_STRING,
	LINE_COMMENT,
	START_COMMENT,
	END_COMMENT
};

namespace std {
	struct lined_string
	{
		string cnt;
		unsigned long line;
	};
}

#define DELIM_MAP std::list<std::pair<std::string, DELIM_MODE>>
#define LEXED_LIST std::list<std::lined_string>
#define ADD_DELIM(var, tag, conf) var.push_back(std::pair<std::string, DELIM_MODE>(tag, conf));

/**
Lexes a file with configurable delimiters.
\param file: file to lex
\param delimiters: delimiters to use
\param lexListContainer: list receiving the lexed file
\return LEX_SUCCESS on success.
*/
int lex(std::fstream& file, DELIM_MAP& delimiters, LEXED_LIST& lexListContainer);

#endif