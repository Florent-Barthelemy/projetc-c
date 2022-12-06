#ifndef LOGIC_H_
#define LOGIC_H_

#include <ostream>

#define ASSERT_LOGIC(a, b)			\
	if (a == X || b == X)			\
		return X;					\
	else if (a == Z || b == Z)		\
		return Z; 

/**
This enum represents a simulation state:
 - H for logical 1
 - L for logical 0
 - X for unknown
 - Z fpr high-Z

Associated operators are also implemented:
 '+': logical 'OR'
 '*': logical 'AND'
 '^': logical 'XOR'
 '!': logical inverter
*/
enum LOGICSTATE
{
	H,
	L,
	X,
	Z
};

//logic operators
LOGICSTATE operator+(LOGICSTATE o1, LOGICSTATE o2);
LOGICSTATE operator*(LOGICSTATE o1, LOGICSTATE o2);
LOGICSTATE operator^(LOGICSTATE o1, LOGICSTATE o2);
LOGICSTATE operator!(LOGICSTATE o1);
LOGICSTATE operator~(LOGICSTATE o1);

//removed operators
LOGICSTATE operator++(LOGICSTATE& o1, int i) = delete;
LOGICSTATE operator--(LOGICSTATE& o1, int i) = delete;

//display operators
std::ostream& operator<<(std::ostream& stream, const LOGICSTATE state);

#endif