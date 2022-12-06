#include "PRIMLIB.h"

void AND::updateState()
{
    getOutConn("OUT1")->state = getInConn("IN1")->state * getInConn("IN2")->state;
}

void OR::updateState()
{
    getOutConn("OUT1")->state = getInConn("IN1")->state + getInConn("IN2")->state;
}