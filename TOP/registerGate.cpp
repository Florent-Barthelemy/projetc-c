#include "registerGate.h"

void registerGates()
{
    SIM_NODE::registerType("INV", invCreator);
	SIM_NODE::registerType("AND", andCreator);
	SIM_NODE::registerType("NAND", nandCreator);
	SIM_NODE::registerType("OR", orCreator);
	SIM_NODE::registerType("NOR", norCreator);
	SIM_NODE::registerType("XOR", xorCreator);
	SIM_NODE::registerType("NXOR", nxorCreator);
	SIM_NODE::registerType("INPUT", buffCreator);
	//SIM_NODE::registerType("DFF", dffCreator);
	//SIM_NODE::registerType("DLATCH", dlatchCreator);
}