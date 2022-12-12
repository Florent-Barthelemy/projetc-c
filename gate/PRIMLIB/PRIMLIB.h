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
CREATE_PRIMGATE(BUFF, PRIMGATE_STD_IN1, PRIMGATE_STD_OUT);

#define PRIMGATE_STD_IN2 {"IN1", "IN2"}

//2 input gates
CREATE_PRIMGATE(AND, PRIMGATE_STD_IN2, PRIMGATE_STD_OUT);
CREATE_PRIMGATE(NAND, PRIMGATE_STD_IN2, PRIMGATE_STD_OUT);
CREATE_PRIMGATE(OR, PRIMGATE_STD_IN2, PRIMGATE_STD_OUT);
CREATE_PRIMGATE(NOR, PRIMGATE_STD_IN2, PRIMGATE_STD_OUT);
CREATE_PRIMGATE(XOR, PRIMGATE_STD_IN2, PRIMGATE_STD_OUT);
CREATE_PRIMGATE(NXOR, PRIMGATE_STD_IN2, PRIMGATE_STD_OUT);


SIM_NODE* invCreator(INIT_PARAM params);
SIM_NODE* andCreator(INIT_PARAM params);
SIM_NODE* nandCreator(INIT_PARAM params);
SIM_NODE* orCreator(INIT_PARAM params);
SIM_NODE* norCreator(INIT_PARAM params);
SIM_NODE* xorCreator(INIT_PARAM params);
SIM_NODE* nxorCreator(INIT_PARAM params);
SIM_NODE* buffCreator(INIT_PARAM params);

//memories
#define PRIMGATE_FF_IN {"IN1", "CLK"}
#define PRIMGATE_FF_OUT {"OUT1"}

class DFF : public SIM_NODE {
    private:
        LOGICSTATE currState = X;
        LOGICSTATE prevClkState = L;

    public:
        DFF(std::string n) : SIM_NODE(n, PRIMGATE_FF_IN, PRIMGATE_FF_OUT) {};
        void updateState();
};

class DLATCH : public SIM_NODE {
    private:
        LOGICSTATE currState = X;

    public:
        DLATCH(std::string n) : SIM_NODE(n, PRIMGATE_FF_IN, PRIMGATE_FF_OUT) {};
        void updateState();
};

SIM_NODE* dffCreator(INIT_PARAM params);
SIM_NODE* dlatchCreator(INIT_PARAM params);


#endif