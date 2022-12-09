#include "../parser/wavedrom/json/json.h"
#include <iostream>

using namespace std;

void json_parser_tb()
{
    try
    {
        STRUCT_FIELD json2 = {
            { "signal", new ARRAY_FIELD(
                {
                    new VALUE_FIELD(new JSONCONTAINER<int>(0)),
                    new VALUE_FIELD(new JSONCONTAINER<int>(2)),
                    new VALUE_FIELD(new JSONCONTAINER<int>(4))
                }
            )},
            { "time", new VALUE_FIELD(new JSONCONTAINER<std::string>("1ns")) },
            { "buff", new UNSIZED_ARRAY_FIELD(fieldGenerator<int, 0>) }
        };

        cout << json2.getField({"0", "signal"})->toString() << endl;
        cout << json2.getField({"1", "signal"})->toString() << endl;
        cout << json2.getField({"2", "signal"})->toString() << endl;

        cout << json2.getField({"time"})->toString() << endl;

        json2.updateField({"0", "buff"}, "1000");
        cout << json2.getField({"0", "buff"})->toString() << endl;

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}