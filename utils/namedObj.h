#ifndef NAMED_OBJ_H_
#define NAMED_OBJ_H_

#include <string>

class NAMEDOBJ
{
private:
    std::string name;

public:
    NAMEDOBJ() = delete;

    /**
    Contructor to name a object.
    \param name: name of the object.
    */
    NAMEDOBJ(std::string name);

    /**
    Get the associated name of this object.
    \return the name of this object.
    */
    inline std::string getName() const { return this->name; }
};

#endif