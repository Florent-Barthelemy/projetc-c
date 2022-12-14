#include "jsonparsing.h"
#include "../../../utils/systemMessages.h"

SystemMessager jsonLogger("JSON");

enum STATE
{
    NEW_FIELD,
    END_FIELD,

    FIELD_NAME,
    FIELD_SEPARATOR,
    FIELD_VALUE
};

enum FIELD_TYPES
{
    STRUCT,
    ARRAY
};

void parseJSON(std::fstream& stream, JSON& json)
{
    LEXED_LIST llist;
    DELIM_MAP delims;

    for(int i = 0; i < (sizeof(jsonDelims) / sizeof(DELIM)); i++)
        ADD_DELIM(delims, jsonDelims[i].tag, jsonDelims[i].mode);

    switch(lex(stream, delims, llist))
    {
        case MISSING_END_SEPARATOR:
            return;
        case LEX_FILE_NOT_OPENED:
            return;
        case LEX_SUCCESS:
            break;
        default:
            return;
    }

    JSONPATH currentPath;
    STATE currentState;
    STATE nextState = NEW_FIELD;
    std::list<FIELD_TYPES> typeFifo;
    std::vector<int> arrayIndex;

    do
    {
        currentState = nextState;

        switch (currentState)
        {
        case NEW_FIELD:
            if(llist.front().cnt == "]" || llist.front().cnt == "}")
            {
                jsonLogger.WARNING("Expecting opening token but token '" + llist.front().cnt + "' line " + std::to_string(llist.front().line) + " found. Please consider removing previous separator token.");
                nextState = END_FIELD;
                goto end_field; //work around
                break;
            }
            if(llist.front().cnt != "{")
                jsonLogger.SYNATX_ERROR<std::invalid_argument>("'{' token expected.", llist.front().line);
            typeFifo.push_back(STRUCT);
            nextState = FIELD_NAME;
            break;

        case FIELD_NAME:
            currentPath.push_back(llist.front().cnt);
            nextState = FIELD_SEPARATOR;
            break;

        case FIELD_SEPARATOR:
            if(llist.front().cnt != ":")
                jsonLogger.SYNATX_ERROR<std::invalid_argument>("':' token expected.", llist.front().line);
            nextState = FIELD_VALUE;
            break;

        case FIELD_VALUE:
            if(llist.front().cnt == "[") //ARRAY
            {
                typeFifo.push_back(ARRAY);
                arrayIndex.push_back(0);
                currentPath.push_back("0");
                nextState = NEW_FIELD;
            }
            else if(llist.front().cnt == "{")
            {
                typeFifo.push_back(STRUCT);
                nextState = NEW_FIELD;
            }
            else
            {
                json.updateField(currentPath, llist.front().cnt);
                nextState = END_FIELD;
            }
            
            break;

        case END_FIELD: //stay in state to discard all END_FIELD tags
            end_field:
            if(typeFifo.size() == 0)
                jsonLogger.SYNATX_ERROR<std::invalid_argument>("Opening token expected before closing token.", llist.front().line);
            if(typeFifo.back() == ARRAY)
                if(llist.front().cnt == "]")
                {
                    typeFifo.pop_back();
                    arrayIndex.pop_back();
                }
                else if(llist.front().cnt == ",")
                {
                    nextState = NEW_FIELD;
                    arrayIndex.back()++;
                    currentPath.back() = std::to_string(arrayIndex.back());
                    break; //break early, do not reduce current path
                }
                else
                    jsonLogger.SYNATX_ERROR<std::invalid_argument>("End of array field token expected.", llist.front().line);
            else if(typeFifo.back() == STRUCT)
                if(llist.front().cnt == "}")
                    typeFifo.pop_back();
                else if(llist.front().cnt == ",")
                    nextState = FIELD_NAME;
                else
                    jsonLogger.SYNATX_ERROR<std::invalid_argument>("End of struct field token expected.", llist.front().line);
            else
                jsonLogger.ERROR<std::domain_error>("Internal error.");
            currentPath.pop_back();

            break;

        default:
            break;
        }

        llist.pop_front();
    } while (llist.size() > 0);

    if(currentState != END_FIELD || currentPath.size() > 0)
        jsonLogger.SYNATX_ERROR<std::invalid_argument>("End of file arrived before all fields are closed.", llist.front().line);
}

void writeJSON(std::fstream& stream, JSON& json)
{
    stream << json.toString();
}