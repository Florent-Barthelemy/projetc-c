#ifndef GRAPH_H_
#define GRAPH_H_

#include <map>
#include <string>
#include <list>
#include <exception>
#include <vector>

template <typename LOGICTYPE>
class NODE;

template <typename LOGICTYPE>
struct NODE_CONN
{
    LOGICTYPE state;
    std::list<NODE<LOGICTYPE>*> outputs;
};

template <typename LOGICTYPE>
class NODE
{
protected:
    std::map<std::string, NODE_CONN<LOGICTYPE>*> inputs;
    std::map<std::string, NODE_CONN<LOGICTYPE>> outputs;

public:
    NODE() = delete;
    NODE(std::vector<std::string>& inputNames, std::vector<std::string>& outputNames);
    ~NODE();

    void connIn(std::string portName, NODE_CONN<LOGICTYPE>* conn) throw(...);

    NODE_CONN<LOGICTYPE>* getInConn(std::string portName) throw(...);

    NODE_CONN<LOGICTYPE>* getOutConn(std::string portName) throw(...);
};

template <typename LOGICTYPE>
NODE<LOGICTYPE>::NODE(std::vector<std::string>& inputNames, std::vector<std::string>& outputNames)
{
    for(auto inIT = inputNames.begin(); inIT != inputNames.end(); inIT++)
        this->inputs.insert({*inIT, NULL});

    for(auto outIT = outputNames.begin(); outIT != outputNames.end(); outIT++)
        this->outputs.insert({*outIT, {}});
}

template <typename LOGICTYPE>
NODE<LOGICTYPE>::~NODE()
{
    for(std::map<std::string, NODE_CONN<LOGICTYPE>*>::iterator inIT = this->inputs.begin(); inIT != this->inputs.end(); ++inIT)
    {
        if(inIT->second != NULL) //is connected
            for(std::list<NODE<LOGICTYPE>*>::iterator remIT = inIT->second->outputs.begin(); remIT != inIT->second->outputs.end();)
            {
                if(*remIT == this)
                    remIT = inIT->second->outputs.erase(remIT);
                else
                    ++remIT;
            }
    }

    for(std::map<std::string, NODE_CONN<LOGICTYPE>>::iterator outIT = this->outputs.begin(); outIT != this->outputs.end(); ++outIT)
    {
        for(std::list<NODE<LOGICTYPE>*>::iterator nodeIT = outIT->second.outputs.begin(); nodeIT != outIT->second.outputs.end(); ++nodeIT)
        {
            for(std::map<std::string, NODE_CONN<LOGICTYPE>*>::iterator mapIT = (*nodeIT)->inputs.begin(); mapIT != (*nodeIT)->inputs.end(); ++mapIT)
            {
                if(mapIT->second == &outIT->second)
                    mapIT->second = NULL;
            }
        }
    }

    this->outputs.clear();
}

template <typename LOGICTYPE>
void NODE<LOGICTYPE>::connIn(std::string portName, NODE_CONN<LOGICTYPE>* conn) throw(...)
{
    std::map<std::string, NODE_CONN<LOGICTYPE>*>::iterator connSearch = this->inputs.find(portName);

    if(connSearch != this->inputs.end())
    {
        connSearch->second = conn;
        conn->outputs.push_back(this);
    }
    else
        throw new std::exception("Input port name does not exist.");
}

template <typename LOGICTYPE>
NODE_CONN<LOGICTYPE>* NODE<LOGICTYPE>::getInConn(std::string portName) throw(...)
{
    std::map<std::string, NODE_CONN<LOGICTYPE>*>::iterator connSearch = this->inputs.find(portName);

    if(connSearch == this->inputs.end())
        throw new std::exception("Inputs port name does not exist.");
    else
        return connSearch->second;
}

template <typename LOGICTYPE>
NODE_CONN<LOGICTYPE>* NODE<LOGICTYPE>::getOutConn(std::string portName) throw(...)
{
    std::map<std::string, NODE_CONN<LOGICTYPE>>::iterator connSearch = this->outputs.find(portName);

    if(connSearch == this->outputs.end())
        throw new std::exception("Output port name does not exist.");
    else
        return &connSearch->second;
}

#endif