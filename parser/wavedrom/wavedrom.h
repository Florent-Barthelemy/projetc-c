#ifndef WAVEDROM_H_
#define WAVEDROM_H_

#include "../stimuli.h"
#include "json/jsonparsing.h"

JSONFIELD* wavedromGenerator();

#define WAVEDROM_JSON(varName) JSON varName = {{"signal", new UNSIZED_ARRAY_FIELD(wavedromGenerator)}};

void generateStimuliData(JSON& json, STIMULI_HANDLER& handler);

#endif