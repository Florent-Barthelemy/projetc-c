#include "../parser/wavedrom/json/jsonparsing.h"
#include <iostream>
#include "../parser/wavedrom/wavedrom.h"

using namespace std;

void json_parser_tb()
{
    try
    {
        WAVEDROM_JSON(json);

        STIMULI_HANDLER stims;

        fstream testFile("files\\json_test.json", ios_base::in);

        parseJSON(testFile, json);

        generateStimuliData(json, stims);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}