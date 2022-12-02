#include "../gate/PRIMLIB/PRIMLIB.h"
#include <iostream>

using namespace std;

void gate_tb()
{
    ADD adder("myAdder");
    ADD adder2("add2");
    ADD adder3("add3");

    adder2.connIn("A", adder.getOutConn("S"));
    adder2.connIn("B", adder.getOutConn("S"));
    adder3.connIn("A", adder.getOutConn("S"));
    adder3.connIn("B", adder.getOutConn("S"));

    NODE_CONN<LOGICSTATE> testConn1 = {X, {}};
    NODE_CONN<LOGICSTATE> testConn2 = {X, {}};

    adder.connIn("A", &testConn1);
    adder.connIn("B", &testConn2);

    testConn1.state = L;
    testConn2.state = L;

    adder.updateGate();

    cout << "|A|B||S1|S2|" << endl;
    cout << "|" << testConn1.state << "|" << testConn2.state << "||";
    cout << adder2.getOutConn("S")->state << " |" << adder3.getOutConn("S")->state << " |";
    cout << endl;

    testConn1.state = L;
    testConn2.state = H;

    adder.updateGate();

    cout << "|" << testConn1.state << "|" << testConn2.state << "||";
    cout << adder2.getOutConn("S")->state << " |" << adder3.getOutConn("S")->state << " |";
    cout << endl;

    testConn1.state = H;
    testConn2.state = L;

    adder.updateGate();

    cout << "|" << testConn1.state << "|" << testConn2.state << "||";
    cout << adder2.getOutConn("S")->state << " |" << adder3.getOutConn("S")->state << " |";
    cout << endl;

    testConn1.state = H;
    testConn2.state = H;

    adder.updateGate();

    cout << "|" << testConn1.state << "|" << testConn2.state << "||";
    cout << adder2.getOutConn("S")->state << " |" << adder3.getOutConn("S")->state << " |";
    cout << endl;
}