#include "jsoncontainer.h"

//std::string
std::string JSONCONTAINER<std::string>::toString()
{
    return this->data;
}

void JSONCONTAINER<std::string>::fromString(std::string str)
{
    this->data = str;
}

//std::int
std::string JSONCONTAINER<int>::toString()
{
    return std::to_string(this->data);
}

void JSONCONTAINER<int>::fromString(std::string str)
{
    this->data = 1; //TODO: implement
}