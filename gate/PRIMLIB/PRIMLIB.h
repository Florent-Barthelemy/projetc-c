#ifndef PRIMLIB_H_
#define PRIMLIB_H_

#include "../sim_node.h"

#define CREATE_PRIMGATE(name, inNames, outNames)                    \
    class name : public SIM_NODE {                                    \
        public:                                                     \
            name(std::string n) : SIM_NODE(n, inNames, outNames) {};  \
            void updateState();                                     \
    };

#define PRIMGATE_STD_OUT {"OUT1"}

#define PRIMGATE_STD_IN1 {"IN1"}

//1 input gates
CREATE_PRIMGATE(INV, PRIMGATE_STD_IN1, PRIMGATE_STD_OUT);

#define PRIMGATE_STD_IN2 {"IN1", "IN2"}

//2 input gates
CREATE_PRIMGATE(AND, PRIMGATE_STD_IN2, PRIMGATE_STD_OUT);
CREATE_PRIMGATE(NAND, PRIMGATE_STD_IN2, PRIMGATE_STD_OUT);
CREATE_PRIMGATE(OR, PRIMGATE_STD_IN2, PRIMGATE_STD_OUT);
CREATE_PRIMGATE(NOR, PRIMGATE_STD_IN2, PRIMGATE_STD_OUT);
CREATE_PRIMGATE(XOR, PRIMGATE_STD_IN2, PRIMGATE_STD_OUT);
CREATE_PRIMGATE(NXOR, PRIMGATE_STD_IN2, PRIMGATE_STD_OUT);

#endif