#ifndef GRAPH_H_
#define GRAPH_H_

#include <map>
#include <string>
#include <list>
#include <exception>
#include <vector>
#include <stdexcept>

template <typename LOGICTYPE>
class NODE;

template <typename LOGICTYPE>
struct NODE_CONN
{
    LOGICTYPE state;
    std::list<NODE<LOGICTYPE>*> outputs;
};

/**
Directed graph vertices/nodes/points.
Edges/links/lines are connected via the "input" and "outputs". Their value is registered in a NODE_CONN.
*/
template <typename LOGICTYPE>
class NODE
{
protected:
    std::map<std::string, NODE_CONN<LOGICTYPE>*> inputs;
    std::map<std::string, NODE_CONN<LOGICTYPE>> outputs;

public:
    NODE() = delete;
    NODE(NODE&) = delete;

    /**
    Node constructor.
    \param inputNames: vector of input port names
    \param outputNames: vector of output port names
    */
    NODE(std::vector<std::string>& inputNames, std::vector<std::string>& outputNames);
    
    ~NODE();

    /**
    Connect input net to NODE_CONN.
    \param portName: name of the port to connect.
    \param conn: NODE_CONN to connect.
    \throw std::invalid_argument when port does not exist.
    */
    void connIn(std::string portName, NODE_CONN<LOGICTYPE>* conn);

    /**
    Gets input port pointer.
    \param portName: name of the port to get.
    \return NODE_CONN driving this net.
    \throw std::invalid_argument when port does not exist.
    */
    NODE_CONN<LOGICTYPE>* getInConn(std::string portName);

    /// @brief returns a (const) map of the node inputs for
    /// improved flexibility on top level connectivity checks 
    /// @return map<nodeName, connector*>
    /// @author Florent B.
    inline const std::map<std::string, NODE_CONN<LOGICTYPE>*> getInputs() const { return this->inputs; }

    /**
    Gets output port pointer.
    \param portName: name of the port to get.
    \return NODE_CONN driving this net.
    \throw std::invalid_argument when port does not exist.
    */
    NODE_CONN<LOGICTYPE>* getOutConn(std::string portName);
};

template <typename LOGICTYPE>
NODE<LOGICTYPE>::NODE(std::vector<std::string>& inputNames, std::vector<std::string>& outputNames)
{
    for(auto inIT = inputNames.begin(); inIT != inputNames.end(); inIT++) //creates input ports
        this->inputs.insert({*inIT, NULL});

    for(auto outIT = outputNames.begin(); outIT != outputNames.end(); outIT++) //creates output ports
        this->outputs.insert({*outIT, {}});
}

template <typename LOGICTYPE>
NODE<LOGICTYPE>::~NODE()
{
    for(typename std::map<std::string, NODE_CONN<LOGICTYPE>*>::iterator inIT = this->inputs.begin(); inIT != this->inputs.end(); ++inIT)
    {
        if(inIT->second != NULL) //if port is connected
            for(typename std::list<NODE<LOGICTYPE>*>::iterator remIT = inIT->second->outputs.begin(); remIT != inIT->second->outputs.end();)
            {
                if(*remIT == this) //port is connected
                    remIT = inIT->second->outputs.erase(remIT);
                else
                    ++remIT;
            }
    }

    for(typename std::map<std::string, NODE_CONN<LOGICTYPE>>::iterator outIT = this->outputs.begin(); outIT != this->outputs.end(); ++outIT)
    {
        for(typename std::list<NODE<LOGICTYPE>*>::iterator nodeIT = outIT->second.outputs.begin(); nodeIT != outIT->second.outputs.end(); ++nodeIT)
        {
            for(typename std::map<std::string, NODE_CONN<LOGICTYPE>*>::iterator mapIT = (*nodeIT)->inputs.begin(); mapIT != (*nodeIT)->inputs.end(); ++mapIT)
            {
                if(mapIT->second == &outIT->second) //disconnect all connected nodes
                    mapIT->second = NULL;
            }
        }
    }

    this->outputs.clear();
}

template <typename LOGICTYPE>
void NODE<LOGICTYPE>::connIn(std::string portName, NODE_CONN<LOGICTYPE>* conn)
{
    typename std::map<std::string, NODE_CONN<LOGICTYPE>*>::iterator connSearch = this->inputs.find(portName);

    if(connSearch != this->inputs.end())
    {
        connSearch->second = conn;
        conn->outputs.push_back(this);
    }
    else
        throw std::invalid_argument("Input port name does not exist.");
}

template <typename LOGICTYPE>
NODE_CONN<LOGICTYPE>* NODE<LOGICTYPE>::getInConn(std::string portName)
{
    typename std::map<std::string, NODE_CONN<LOGICTYPE>*>::iterator connSearch = this->inputs.find(portName);

    if(connSearch == this->inputs.end())
        throw std::invalid_argument("Inputs port name does not exist.");
    else
        return connSearch->second;
}

template <typename LOGICTYPE>
NODE_CONN<LOGICTYPE>* NODE<LOGICTYPE>::getOutConn(std::string portName)
{
    typename std::map<std::string, NODE_CONN<LOGICTYPE>>::iterator connSearch = this->outputs.find(portName);

    if(connSearch == this->outputs.end())
        throw std::invalid_argument("Output port name does not exist.");
    else
        return &connSearch->second;
}

#endif