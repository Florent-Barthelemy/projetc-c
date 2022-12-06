#include "../gate/PRIMLIB/PRIMLIB.h"
#include <iostream>

using namespace std;

void gate_tb()
{
    //NEEDS TO BE BEFORE GATE INSTANCIATION!
    //NODE_CONN HERE ARE DESTROYED AFTER THE DESTRUCTION OF THE MODULES!
    NODE_CONN<LOGICSTATE> testConn1 = {X, {}};
    NODE_CONN<LOGICSTATE> testConn2 = {X, {}};

    AND adder2("add2");
    AND adder3("add3");
    OR adder("myAdder");

    adder2.connIn("IN1", adder.getOutConn("OUT1"));
    adder2.connIn("IN2", adder.getOutConn("OUT1"));
    adder3.connIn("IN1", adder.getOutConn("OUT1"));
    adder3.connIn("IN2", adder.getOutConn("OUT1"));

    adder.connIn("IN1", &testConn1);
    adder.connIn("IN2", &testConn2);

    testConn1.state = L;
    testConn2.state = L;

    adder.updateGate();

    cout << "|A|B||S1|S2|" << endl;
    cout << "|" << testConn1.state << "|" << testConn2.state << "||";
    cout << adder2.getOutConn("OUT1")->state << " |" << adder3.getOutConn("OUT1")->state << " |";
    cout << endl;

    testConn1.state = L;
    testConn2.state = H;

    adder.updateGate();

    cout << "|" << testConn1.state << "|" << testConn2.state << "||";
    cout << adder2.getOutConn("OUT1")->state << " |" << adder3.getOutConn("OUT1")->state << " |";
    cout << endl;

    testConn1.state = H;
    testConn2.state = L;

    adder.updateGate();

    cout << "|" << testConn1.state << "|" << testConn2.state << "||";
    cout << adder2.getOutConn("OUT1")->state << " |" << adder3.getOutConn("OUT1")->state << " |";
    cout << endl;

    testConn1.state = H;
    testConn2.state = H;

    adder.updateGate();

    cout << "|" << testConn1.state << "|" << testConn2.state << "||";
    cout << adder2.getOutConn("OUT1")->state << " |" << adder3.getOutConn("OUT1")->state << " |";
    cout << endl;
}