#include "PRIMLIB.h"

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