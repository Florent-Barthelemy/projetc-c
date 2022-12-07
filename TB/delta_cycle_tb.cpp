#include "../gate/PRIMLIB/PRIMLIB.h"
#include <iostream>

void delta_cycle_tb()
{
    try
    {
        NODE_CONN<LOGICSTATE> input;

        SIM_NODE::setMaxDelta(10);

        NOR or1("myNOR");

        or1.connIn("IN1", &input);
        or1.connIn("IN2", or1.getOutConn("OUT1"));

        input.state = LOGICSTATE::H;
        or1.updateGate();

        std::cout << or1.getOutConn("OUT1")->state << std::endl;

        input.state = LOGICSTATE::L;
        or1.updateGate();

        std::cout << or1.getOutConn("OUT1")->state << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }

    std::cout << "TEST FINISHED" << std::endl;
}