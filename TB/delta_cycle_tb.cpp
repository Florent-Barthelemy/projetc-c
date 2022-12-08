#include "../gate/PRIMLIB/PRIMLIB.h"
#include <iostream>

using namespace std;

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

    try
    {
        NODE_CONN<LOGICSTATE> input;
        NODE_CONN<LOGICSTATE> clk;

        SIM_NODE::setMaxDelta(10);

        DFF ff("FF");
        DLATCH latch("LATCH");

        ff.connIn("D", &input);
        latch.connIn("D", &input);

        ff.connIn("Clk", &clk);
        latch.connIn("Clk", &clk);

        input.state = X; clk.state = X;
        ff.updateGate(); latch.updateGate();
        cout << ff.getOutConn("Q")->state << "|" << latch.getOutConn("Q")->state << endl;

        input.state = L; clk.state = L;
        ff.updateGate(); latch.updateGate();
        cout << ff.getOutConn("Q")->state << "|" << latch.getOutConn("Q")->state << endl;

        input.state = L; clk.state = H;
        ff.updateGate(); latch.updateGate();
        cout << ff.getOutConn("Q")->state << "|" << latch.getOutConn("Q")->state << endl;

        input.state = H; clk.state = H;
        ff.updateGate(); latch.updateGate();
        cout << ff.getOutConn("Q")->state << "|" << latch.getOutConn("Q")->state << endl;

        input.state = H; clk.state = L;
        ff.updateGate(); latch.updateGate();
        cout << ff.getOutConn("Q")->state << "|" << latch.getOutConn("Q")->state << endl;

        input.state = L; clk.state = L;
        ff.updateGate(); latch.updateGate();
        cout << ff.getOutConn("Q")->state << "|" << latch.getOutConn("Q")->state << endl;
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }

    std::cout << "TEST FINISHED" << std::endl;
}