#include "wavedrom.h"
#include "../../utils/systemMessages.h"

SystemMessager wavedromLogger("Wavedrom");

JSONFIELD* wavedromGenerator()
{
    return new STRUCT_FIELD({
        { "name", new VALUE_FIELD(new JSONCONTAINER<std::string>("")) },
        { "wave", new VALUE_FIELD(new JSONCONTAINER<std::string>("")) }
    });
}

void generateStimuliData(JSON& json, STIMULI_HANDLER& handler)
{
    std::vector<JSONFIELD*> signals = json.getAsArray({"signal"});

    for(std::vector<JSONFIELD*>::iterator sigIT = signals.begin(); sigIT != signals.end(); ++sigIT)
    {
        long timestamp = 0;
        std::vector<TIMED_SIG> newStimulis;

        JSONCONTAINER<std::string>* nameContainer = dynamic_cast<JSONCONTAINER<std::string>*>((*sigIT)->getField({"name"}));
        if(nameContainer == NULL)
            wavedromLogger.ERROR<std::domain_error>("JSON container is not a string.");

        JSONCONTAINER<std::string>* valueContainer = dynamic_cast<JSONCONTAINER<std::string>*>((*sigIT)->getField({"wave"}));
        if(valueContainer == NULL)
            wavedromLogger.ERROR<std::domain_error>("JSON container is not a string.");

        std::string waveStims = valueContainer->getValue();

        for(int i = 0; i < waveStims.size(); i++)
        {
            switch (waveStims[i])
            {
            case '0':
            case 'l':
            case 'L':
                newStimulis.push_back({timestamp, L});
                break;

            case '1':
            case 'h':
            case 'H':
                newStimulis.push_back({timestamp, H});
                break;
                
            case 'n':
            case 'N':
            {
                newStimulis.push_back({timestamp, L});
                int iP;
                for(iP = 1; iP + i < waveStims.size() && waveStims[iP + i] == '.'; iP++)
                    newStimulis.push_back({timestamp + iP, !newStimulis.back().state});
                i += iP - 1;
                break;
            }

            case 'p':
            case 'P':
            {
                newStimulis.push_back({timestamp, H});
                int iP;
                for(iP = 1; iP + i < waveStims.size() && waveStims[iP + i] == '.'; iP++)
                    newStimulis.push_back({timestamp + iP, !newStimulis.back().state});
                i += iP - 1;
                break;
            }

            case 'x':
            case 'X':
                newStimulis.push_back({timestamp, X});
                break;

            case 'z':
            case 'Z':
                newStimulis.push_back({timestamp, Z});
                break;

            case '.':
                break;

            default:
                wavedromLogger.ERROR<std::invalid_argument>("Wave token\'" + waveStims[i] + (std::string) "\' for signal \""  + nameContainer->getValue() + "\" is unknown."); //why is the cast needed? good question!
                break;
            }

            timestamp++;
        }

        handler.addStimuli(nameContainer->getValue(), newStimulis);
    }
}

JSON generateWavedromSigs(ACQUISITION_HANDLER& handler)
{
    WAVEDROM_JSON(json);
    std::map<std::string, std::vector<TIMED_SIG>> sigs;
    handler.saveAcquisition(sigs);

    int iJson = 0;
    std::string strJson;
    long currentTimestamp;

    for(std::map<std::string, std::vector<TIMED_SIG>>::iterator sigIT = sigs.begin(); sigIT != sigs.end(); ++sigIT, ++iJson)
    {
        strJson = ""; currentTimestamp = 0;
        for(std::vector<TIMED_SIG>::iterator timedSigIt = sigIT->second.begin(); timedSigIt != sigIT->second.end(); ++timedSigIt)
        {
            while(currentTimestamp < timedSigIt->timestamp)
            {
                strJson += ".";
                currentTimestamp++;
            }
            
            switch (timedSigIt->state)
            {
            case X:
                strJson += "X";
                break;
            case Z:
                strJson += "Z";
                break;
            case L:
                strJson += "0";
                break;
            case H:
                strJson += "1";
                break;
            
            default:
                wavedromLogger.WARNING("Failed to decode state '" + timedSigIt->state + (std::string) "' to wavdrom.");
                break;
            }

            currentTimestamp++;
        }

        json.updateField({"signal", std::to_string(iJson), "name"}, sigIT->first);
        json.updateField({"signal", std::to_string(iJson), "wave"}, strJson);
    }

    return json;
}