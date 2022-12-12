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
    if(getInConn("CLK")->state == H && this->prevClkState == L)
        this->currState = getInConn("IN1")->state;

    this->prevClkState = getInConn("IN1")->state;

    getOutConn("OUT1")->state = this->currState;
}

void DLATCH::updateState()
{
    if(getInConn("CLK")->state == H )
        this->currState = getInConn("IN1")->state;
        
    getOutConn("OUT1")->state = this->currState;
}

