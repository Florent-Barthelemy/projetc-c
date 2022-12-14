#include "PRIMLIB.h"

//combinatorial 1i/1s gates

void BUFF::updateState()
{
    getOutConn("OUT1")->state = getInConn("IN1")->state;
}

void INV::updateState()
{
    getOutConn("OUT1")->state = !getInConn("IN1")->state;
}

//combinatorial 2i/1s gates

void AND::updateState()
{
    getOutConn("OUT1")->state = getInConn("IN1")->state * getInConn("IN2")->state;
}

void NAND::updateState()
{
    getOutConn("OUT1")->state = !(getInConn("IN1")->state * getInConn("IN2")->state);
}

void OR::updateState()
{
    getOutConn("OUT1")->state = getInConn("IN1")->state + getInConn("IN2")->state;
}

void NOR::updateState()
{
    getOutConn("OUT1")->state = !(getInConn("IN1")->state + getInConn("IN2")->state);
}

void XOR::updateState()
{
    getOutConn("OUT1")->state = getInConn("IN1")->state ^ getInConn("IN2")->state;
}

void NXOR::updateState()
{
    getOutConn("OUT1")->state = !(getInConn("IN1")->state ^ getInConn("IN2")->state);
}

//1 bit memories

void DFF::updateState()
{
    NODE_CONN<LOGICSTATE>* clkConnPtr = getInConn("CLK");
    
    if(clkConnPtr == 0x00)
    {
        getOutConn("OUT1")->state = X;
        return;
    }
    else
    {
        if(clkConnPtr->state == H && this->prevClkState == L)
            this->currState = getInConn("IN1")->state;

        this->prevClkState = clkConnPtr->state;

        getOutConn("OUT1")->state = this->currState;
    }
}

void DLATCH::updateState()
{
    if(getInConn("CLK")->state == H )
        this->currState = getInConn("IN1")->state;
        
    getOutConn("OUT1")->state = this->currState;
}

void MUX::updateState()
{
    NODE_CONN<LOGICSTATE>* selConnPtr = getInConn("SEL");
    
    /// quick fix for existing but 
    /// unconnected SEL signal inducing a seg fault
    if(selConnPtr == 0x00)
    {
        getOutConn("OUT1")->state = X;
        return;
    }
        
    switch (selConnPtr->state)
    {
    case L:
        getOutConn("OUT1")->state = getInConn("IN1")->state;
        break;
    case H:
        getOutConn("OUT1")->state = getInConn("IN2")->state;
        break;

    default:
        getOutConn("OUT1")->state = X;
        break;
    }
}