#include "../gate.h"

#define CREATE_PRIMGATE(name, inNames, outNames)                \
    class name : public GATE {                                  \
        public:                                                 \
            name(std::string n) : GATE(n, inNames, outNames) {};   \
            void updateState();                                 \
    };

#define PRIMGATE_STD_IN2 {"A", "B"}
#define PRIMGATE_STD_OUT {"S"}

CREATE_PRIMGATE(AND, PRIMGATE_STD_IN2, PRIMGATE_STD_OUT);