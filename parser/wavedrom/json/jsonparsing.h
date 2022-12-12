#ifndef JSONPARSING_H_
#define JSONPARSING_H_

#include "json.h"
#include <fstream>
#include "../../lexer.h"

typedef STRUCT_FIELD JSON;

struct DELIM
{
    std::string tag;
    DELIM_MODE mode;
};

const DELIM jsonDelims[] = {
    {"\"", TOGGLE_STRING},
    {" ", DISCARD},
    {"\n", DISCARD},
    {"\t", DISCARD},
    {",", SAVE},
    {":", SAVE},
    {"{", SAVE},
    {"}", SAVE},
    {"[", SAVE},
    {"]", SAVE}
};

void parseJSON(std::fstream&, JSON&);
void writeJSON(std::fstream&, JSON&);

#endif