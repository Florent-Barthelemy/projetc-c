#ifndef JSONCONTAINER_H_
#define JSONCONTAINER_H_

#include <string>

class JSONPARSER
{
public:
    virtual std::string toString() = 0;
    virtual void fromString(std::string) = 0;
};

//for implementation

template <typename TYPE>
class JSONCONTAINER : public JSONPARSER
{
private:
    TYPE data;

public:
    JSONCONTAINER(TYPE d) : data(d) {}
    std::string toString();
    void fromString(std::string);
    inline TYPE getValue() { return this->data; }
    inline void setValue(TYPE& a) { this->data = a; }
};

#endif