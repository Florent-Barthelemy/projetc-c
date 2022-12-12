#include "PRIMLIB.h"

SIM_NODE* invCreator(INIT_PARAM params)
{
    return new INV(params.nodeName);
}

SIM_NODE* andCreator(INIT_PARAM params)
{
    return new AND(params.nodeName);
}

SIM_NODE* nandCreator(INIT_PARAM params)
{
    return new NAND(params.nodeName);
}

SIM_NODE* orCreator(INIT_PARAM params)
{
    return new OR(params.nodeName);
}

SIM_NODE* norCreator(INIT_PARAM params)
{
    return new NOR(params.nodeName);
}

SIM_NODE* xorCreator(INIT_PARAM params)
{
    return new XOR(params.nodeName);
}

SIM_NODE* nxorCreator(INIT_PARAM params)
{
    return new NXOR(params.nodeName);
}

SIM_NODE* buffCreator(INIT_PARAM params)
{
    return new BUFF(params.nodeName);
}

SIM_NODE* dffCreator(INIT_PARAM params)
{
    return new DFF(params.nodeName);
}

SIM_NODE* dlatchCreator(INIT_PARAM params)
{
    return new DLATCH(params.nodeName);
}