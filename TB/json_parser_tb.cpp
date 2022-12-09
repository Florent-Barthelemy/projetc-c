#include "../parser/wavedrom/json/jsonparsing.h"
#include <iostream>

using namespace std;

JSONFIELD* waveGenerator()
{
    return new STRUCT_FIELD({
        { "name", new VALUE_FIELD(new JSONCONTAINER<std::string>("")) },
        { "wave", new VALUE_FIELD(new JSONCONTAINER<std::string>("")) }
    });
}

void json_parser_tb()
{
    try
    {
        JSON json = {{
            "signal", new UNSIZED_ARRAY_FIELD(waveGenerator)
        }};

        fstream testFile("files\\json_test.json", ios_base::in);

        parseJSON(testFile, json);

        printf(json.toString().c_str());
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}