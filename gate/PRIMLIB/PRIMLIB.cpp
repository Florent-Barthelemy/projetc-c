#include "PRIMLIB.h"

//combinatorial 2i/1s gates

void INV::updateState()
{
    getOutConn("OUT1")->state = !getInConn("IN1")->state;
}

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
    if(getInConn("Clk")->state == H && this->prevClkState == L)
        this->currState = getInConn("D")->state;

    this->prevClkState = getInConn("Clk")->state;

    getOutConn("Q")->state = this->currState;
}

void DLATCH::updateState()
{
    if(getInConn("Clk")->state == H )
        this->currState = getInConn("D")->state;
        
    getOutConn("Q")->state = this->currState;
}