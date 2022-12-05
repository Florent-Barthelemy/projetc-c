#include "PRIMLIB.h"

void AND::updateState()
{
    LOGICSTATE newState = H;

    for(auto it = this->inputs.begin(); it != this->inputs.end(); it++)
        newState = newState * it->second->state;

    for(auto it = this->outputs.begin(); it != this->outputs.end(); it++)
        it->second.state = newState;
}

void OR::updateState()
{
    LOGICSTATE newState = L;

    for(auto it = this->inputs.begin(); it != this->inputs.end(); it++)
        newState = newState + it->second->state;

    for(auto it = this->outputs.begin(); it != this->outputs.end(); it++)
        it->second.state = newState;
}