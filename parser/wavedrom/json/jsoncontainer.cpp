#include "jsoncontainer.h"
#include "json.h"

//std::string
template<>
std::string JSONCONTAINER<std::string>::toString()
{
    return "\"" + this->data + "\"";
}

template<>
void JSONCONTAINER<std::string>::fromString(std::string str)
{
    this->data = str;
}

//std::int
template<>
std::string JSONCONTAINER<int>::toString()
{
    return std::to_string(this->data);
}

template<>
void JSONCONTAINER<int>::fromString(std::string str)
{
    this->data = stringToInt(str);
}