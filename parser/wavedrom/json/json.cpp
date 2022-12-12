#include "json.h"

int stringToInt(std::string str)
{
    int retValue = 0;

    while(str.size() > 0)
    {
        if(str.front() < '0' || str.front() > '9')
            throw std::invalid_argument("String is not a number.");

        retValue *= 10;
        retValue += str.front() - '0';
        str = str.substr(1);
    }

    return retValue;
}

//VALUE_FIELD
VALUE_FIELD::VALUE_FIELD(JSONPARSER* parser) : value(parser) {}

void VALUE_FIELD::updateField(JSONPATH path, std::string val)
{
    if(path.size() > 0)
        throw std::domain_error("Unknown field.");

    value->fromString(val);
}

JSONPARSER* VALUE_FIELD::getField(JSONPATH path)
{
    if(path.size() > 0)
        throw std::domain_error("Unknown field.");

    return value;
}

std::vector<JSONFIELD*> VALUE_FIELD::getAsArray(JSONPATH path)
{
    if(path.size() > 0)
        throw std::domain_error("Unknown field.");

    std::vector<JSONFIELD*> vec;
    vec.push_back(this);
    return vec;
}

std::string VALUE_FIELD::toString()
{
    return value->toString();
}
//ARRAY_FIELD
ARRAY_FIELD::ARRAY_FIELD(std::vector<JSONFIELD*> fields) : fields(fields) {}

ARRAY_FIELD::ARRAY_FIELD(std::initializer_list<JSONFIELD*> fields) : fields(fields) {}

void ARRAY_FIELD::updateField(JSONPATH path, std::string val)
{
    if(path.size() <= 0)
        throw std::domain_error("Domain is not a value.");

    long index = stringToInt(path.front());

    if(index > fields.size())
        throw std::out_of_range("JSON out of range access issued.");

    path.pop_front();

    fields[index]->updateField(path, val);
}

JSONPARSER* ARRAY_FIELD::getField(JSONPATH path)
{
    if(path.size() <= 0)
        throw std::domain_error("Domain is not a value.");

    long index = stringToInt(path.front());

    if(index > fields.size())
        throw std::out_of_range("JSON out of range access issued.");

    path.pop_front();

    return fields[index]->getField(path);
}

std::vector<JSONFIELD*> ARRAY_FIELD::getAsArray(JSONPATH path)
{
    if(path.size() > 0)
        throw std::domain_error("Unknown field.");

    return fields;
}

std::string ARRAY_FIELD::toString()
{
    std::string buf = "[\n";

    for(auto it = fields.begin(); it != fields.end(); ++it)
        buf += (*it)->toString() + ",\n";

    buf = buf.substr(0, buf.size() - 2); //remove last ','
    buf += "\n]";

    return buf;
}

//STRUCT_FIELD
STRUCT_FIELD::STRUCT_FIELD(std::map<std::string, JSONFIELD*> fields) : fields(fields) {}

STRUCT_FIELD::STRUCT_FIELD(std::initializer_list<std::pair<const std::string, JSONFIELD*>> fields) : fields(fields) {}

void STRUCT_FIELD::updateField(JSONPATH path, std::string val)
{
    if(path.size() <= 0)
        throw std::domain_error("Domain is not a value.");

    std::map<std::string, JSONFIELD*>::iterator fieldFinder = fields.find(path.front());

    if(fieldFinder == fields.end())
        throw std::invalid_argument("Field not found.");

    path.pop_front();

    fieldFinder->second->updateField(path, val);
}

JSONPARSER* STRUCT_FIELD::getField(JSONPATH path)
{
    if(path.size() <= 0)
        throw std::domain_error("Domain is not a value.");

    std::map<std::string, JSONFIELD*>::iterator fieldFinder = fields.find(path.front());

    if(fieldFinder == fields.end())
        throw std::invalid_argument("Field not found.");

    path.pop_front();

    return fieldFinder->second->getField(path);
}

std::vector<JSONFIELD*> STRUCT_FIELD::getAsArray(JSONPATH path)
{
    if(path.size() == 0)
        throw std::domain_error("Cannot get struct as array.");

    std::map<std::string, JSONFIELD*>::iterator fieldFinder = fields.find(path.front());

    if(fieldFinder == fields.end())
        throw std::invalid_argument("Field not found.");

    path.pop_front();

    return fieldFinder->second->getAsArray(path);
}

std::string STRUCT_FIELD::toString()
{
    std::string buf = "{";

    for(auto it = fields.begin(); it != fields.end(); ++it)
        buf += it->first + ":" + it->second->toString() + ",";

    buf = buf.substr(0, buf.size() - 1);
    buf += "}";

    return buf;
}

//UNSIZED_ARRAY_FIELD
UNSIZED_ARRAY_FIELD::UNSIZED_ARRAY_FIELD(JSONFIELD* (*generator)()) : create(generator) {}

void UNSIZED_ARRAY_FIELD::updateField(JSONPATH path, std::string val)
{
    if(path.size() <= 0)
        throw std::domain_error("Domain is not a value.");

    long index = stringToInt(path.front());
    path.pop_front();

    if(index >= fields.size())
        if(index == fields.size())
        {
            fields.push_back(create());
            fields.back()->updateField(path, val);
        }
        else
            throw std::out_of_range("JSON out of range access issued.");
    else
        fields[index]->updateField(path, val);
}

JSONPARSER* UNSIZED_ARRAY_FIELD::getField(JSONPATH path)
{
    if(path.size() <= 0)
        throw std::domain_error("Domain is not a value.");

    long index = stringToInt(path.front());

    if(index > fields.size())
        throw std::out_of_range("JSON out of range access issued.");

    path.pop_front();

    return fields[index]->getField(path);
}

std::vector<JSONFIELD*> UNSIZED_ARRAY_FIELD::getAsArray(JSONPATH path)
{
    if(path.size() > 0)
        throw std::domain_error("Unknown field.");

    return fields;
}

std::string UNSIZED_ARRAY_FIELD::toString()
{
    std::string buf = "[\n";

    for(auto it = fields.begin(); it != fields.end(); ++it)
        buf += (*it)->toString() + ",\n";

    buf = buf.substr(0, buf.size() - 2); //remove last ','
    buf += "\n]";

    return buf;
}