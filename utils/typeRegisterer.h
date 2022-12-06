#ifndef TYPE_REGISTERER_H_
#define TYPE_REGISTERER_H_

#include <map>
#include <exception>

template<class BASE_CLASS, typename INIT_PARAM, typename ID>
class TYPE_REG
{
private:
    using INIT_FNC = BASE_CLASS* (*)(INIT_PARAM);
    static std::map<ID, INIT_FNC> initialiserRegister;

public:
    static BASE_CLASS* createNewObject(ID identifier, INIT_PARAM) throw(...);
    static bool registerType(ID identifier, INIT_FNC fnc);
    static bool unregisterType(ID identifier);
};

template<class BASE_CLASS, typename INIT_PARAM, typename ID>
std::map<ID, BASE_CLASS* (*)(INIT_PARAM)> TYPE_REG<BASE_CLASS, INIT_PARAM, ID>::initialiserRegister;

template<class BASE_CLASS, typename INIT_PARAM, typename ID>
BASE_CLASS* TYPE_REG<BASE_CLASS, INIT_PARAM, ID>::createNewObject(ID identifier, INIT_PARAM param) throw(...)
{
    std::map<ID, INIT_FNC>::iterator objCreator = initialiserRegister.find(identifier);

    if(objCreator == initialiserRegister.end())
        throw new std::exception("Type is not registered.");

    BASE_CLASS* newObj = objCreator->second(param);

    if(newObj == NULL)
        throw new std::exception("Initialisor failed to create object.");

        return newObj;
}

template<class BASE_CLASS, typename INIT_PARAM, typename ID>
bool TYPE_REG<BASE_CLASS, INIT_PARAM, ID>::registerType(ID identifier, INIT_FNC fnc)
{
    std::pair<std::map<ID, INIT_FNC>::iterator, bool> iResult = initialiserRegister.insert({identifier, fnc});
    return iResult.second;
}

template<class BASE_CLASS, typename INIT_PARAM, typename ID>
bool TYPE_REG<BASE_CLASS, INIT_PARAM, ID>::unregisterType(ID identifier)
{
    return (initialiserRegister.erase(identifier) == 0) ? false : true;
}

#endif