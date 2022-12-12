#include "builder.h"

void configDOTBuilder(ObjectBuilder& builder, DELIM_MAP& delims)
{
    builder.associateToken(token::DIGRAPH, "digraph");
	builder.associateToken(token::SPACE, " ");
	builder.associateToken(token::GRAPH_BLOCK_BEGIN, "{");
	builder.associateToken(token::GRAPH_BLOCK_END, "}");
	builder.associateToken(token::ELEMENT_PARAMS_BEGIN, "[");
	builder.associateToken(token::ELEMENT_PARAMS_END, "]");
	builder.associateToken(token::EQUAL_SIGN, "=");
	builder.associateToken(token::END_LINE_OF_CODE, ";");
	builder.associateToken(token::LINKAGE, "->");
	builder.associateToken(token::QUOTES, "\"");
	builder.associateToken(token::INPUT_PORT_SPECIFIER_START, "(");
	builder.associateToken(token::INPUT_PORT_SPECIFIER_END, ")");
	builder.associateToken(token::OBJECT_SPECIFIER, ".");
	builder.associateToken(token::USING, "using");
	builder.associateToken(token::MODULE, "module");
	
	builder.associateTypeToken(typeToken::INPUT, "input");
	builder.associateTypeToken(typeToken::OUTPUT, "output");

	builder.associateElementFeild(elementFeildInitializer::LABEL, "label");

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
}