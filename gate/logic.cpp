#include "logic.h"

//logic operators
LOGICSTATE operator+(LOGICSTATE o1, LOGICSTATE o2)
{
	ASSERT_LOGIC(o1, o2);

	if (o1 == H || o2 == H)
		return H;

	return L;
}

LOGICSTATE operator*(LOGICSTATE o1, LOGICSTATE o2)
{
	ASSERT_LOGIC(o1, o2);

	if (o1 == L || o2 == L)
		return L;

	return H;
}

LOGICSTATE operator^(LOGICSTATE o1, LOGICSTATE o2)
{
	ASSERT_LOGIC(o1, o2);

	if (o1 == L)
		return o2 == L ? L : H;
	else //o1 == L1
		return o2 == L ? H : L;
}

LOGICSTATE operator!(LOGICSTATE o1)
{
	switch (o1)
	{
	case X: return X;
	case Z: return Z;
	case L: return H;
	case H: return L;
	default: return X;
	}
}

LOGICSTATE operator~(LOGICSTATE o1)
{
	switch (o1)
	{
	case X: return X;
	case Z: return Z;
	case L: return H;
	case H: return L;
	default: return X;
	}
}

//display operators
std::ostream& operator<<(std::ostream& stream, const LOGICSTATE state)
{
	switch (state)
	{
	case X:
		stream << "X";
		break;
	case Z:
		stream << "Z";
		break;
	case L:
		stream << "0";
		break;
	case H:
		stream << "1";
		break;
	default:
		stream << "UNKNOWN_STATE";
		break;
	}

	return stream;
}