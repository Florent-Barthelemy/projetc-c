#ifndef PRIMLIB_H_
#define PRIMLIB_H_

#include "../module.h"

#define CREATE_PRIMGATE(name, inNames, outNames)                    \
    class name : public MODULE {                                    \
        public:                                                     \
            name(std::string n) : MODULE(n, inNames, outNames) {};  \
            void updateState();                                     \
    };

#define PRIMGATE_STD_IN2 {"IN1", "IN2"}
#define PRIMGATE_STD_OUT {"OUT1"}

CREATE_PRIMGATE(AND, PRIMGATE_STD_IN2, PRIMGATE_STD_OUT);
CREATE_PRIMGATE(OR, PRIMGATE_STD_IN2, PRIMGATE_STD_OUT);

#endif