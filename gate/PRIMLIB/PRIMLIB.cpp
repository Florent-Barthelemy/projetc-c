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



SIM_NODE* invCreator(INIT_PARAM params)
{
    return new INV(params.nodeName);
}

SIM_NODE* andCreator(INIT_PARAM params)
{
    return new AND(params.nodeName);
}

SIM_NODE* nandCreator(INIT_PARAM params)
{
    return new NAND(params.nodeName);
}

SIM_NODE* orCreator(INIT_PARAM params)
{
    return new OR(params.nodeName);
}

SIM_NODE* norCreator(INIT_PARAM params)
{
    return new NOR(params.nodeName);
}

SIM_NODE* xorCreator(INIT_PARAM params)
{
    return new XOR(params.nodeName);
}

SIM_NODE* nxorCreator(INIT_PARAM params)
{
    return new NXOR(params.nodeName);
}



