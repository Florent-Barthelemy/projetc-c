#ifndef JSON_H_
#define JSON_H_

#include <string>
#include <vector>
#include <map>
#include "jsoncontainer.h"
#include <exception>
#include <stdexcept>

typedef std::vector<std::string> JSONPATH;

class JSONFIELD
{
public:
    virtual void updateField(JSONPATH, std::string) = 0;
    virtual JSONPARSER* getField(JSONPATH) = 0;
    virtual std::vector<JSONFIELD*> getAsArray(JSONPATH) = 0;
};

class VALUE_FIELD : public JSONFIELD
{
public:
    JSONPARSER* value;

    VALUE_FIELD(JSONPARSER* parser);

    void updateField(JSONPATH path, std::string val);

    JSONPARSER* getField(JSONPATH path);

    std::vector<JSONFIELD*> getAsArray(JSONPATH path);
};

class ARRAY_FIELD : public JSONFIELD
{
public:
    std::vector<JSONFIELD*> fields;

    ARRAY_FIELD(std::vector<JSONFIELD*> fields);

    ARRAY_FIELD(std::initializer_list<JSONFIELD*> fields);

    void updateField(JSONPATH path, std::string val);

    JSONPARSER* getField(JSONPATH path);

    std::vector<JSONFIELD*> getAsArray(JSONPATH path);
};

class STRUCT_FIELD : public JSONFIELD
{
public:
    std::map<std::string, JSONFIELD*> fields;

    STRUCT_FIELD(std::map<std::string, JSONFIELD*> fields);

    STRUCT_FIELD(std::initializer_list<std::pair<const std::string, JSONFIELD*>> fields);

    void updateField(JSONPATH path, std::string val);

    JSONPARSER* getField(JSONPATH path);

    std::vector<JSONFIELD*> getAsArray(JSONPATH path);
};

#endif