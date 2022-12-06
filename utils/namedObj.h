#ifndef NAMED_OBJ_H_
#define NAMED_OBJ_H_

#include <string>

class NAMEDOBJ
{
private:
    std::string name;

public:
    NAMEDOBJ() = delete;
    NAMEDOBJ(std::string name);

    inline std::string getName() const { return this->name; }
};

#endif