#ifndef MODULE_H_
#define MODULE_H_

#include  "../gate/sim_node.h"
#include "../gate/logic.h"
#include "../gate/PRIMLIB/PRIMLIB.h"
#include "../utils/namedObj.h"
#include <string>

using namespace std;

typedef string eltName;
typedef map<eltName, SIM_NODE*> moduleContent;

struct moduleInstanceParams
{
    string moduleName;
    vector<string> moduleInputNames;
    vector<string> moduleOutputNames;
    moduleContent modContent;
};


class Module : public SIM_NODE
{
    public:
        Module(moduleInstanceParams params) : 
        SIM_NODE(params.moduleName, params.moduleInputNames, params.moduleOutputNames) 
        {
            modContent = params.modContent;
            moduleInputNames = params.moduleInputNames;
            moduleOutputNames = params.moduleOutputNames;

            //create an input interface based on NODE_CONNs
            for(auto it = moduleInputNames.begin(); it != moduleInputNames.end(); it++)
            {
                moduleInputConns.insert(
                    *(new pair<string, NODE_CONN<LOGICSTATE>>(
                        *it,
                        *(new NODE_CONN<LOGICSTATE>())
                    ))
                );
            }
        }
    
       
        void updateState()
        {
            for(vector<string>::iterator inputIt = moduleInputNames.begin(); inputIt != moduleInputNames.end(); inputIt++)
            {
                //input forwarding
                auto gate = this->moduleInputConns.find(*inputIt);
                gate->second.state = this->getInConn(*inputIt)->state;

                for(list<NODE<LOGICSTATE>*>::iterator gateOutput = gate->second.outputs.begin(); gateOutput != gate->second.outputs.end(); ++gateOutput)
                {
                    static_cast<SIM_NODE*>(*gateOutput)->updateGate();
                }
            }

            for(vector<string>::iterator outputIt = moduleOutputNames.begin(); outputIt != moduleOutputNames.end(); outputIt++)
            {
                //outputs copy
                this->getOutConn(*outputIt)->state = this->internalModuleOutputs.find(*outputIt)->second->state;
            }


        }

       
        const moduleContent* getContent() const { return &modContent; }
        const vector<string>* getModuleInputNames() const { return &moduleInputNames; }
        const vector<string>* getModuleOutputNames() const { return &moduleOutputNames; }
        map<string, NODE_CONN<LOGICSTATE>>* getModuleInputConns()  { return &moduleInputConns; }
        map<string, NODE_CONN<LOGICSTATE>*>* getInternalModuleOutputs() { return &internalModuleOutputs; }
        

        ~Module()
        {
            for(moduleContent::iterator it = modContent.begin(); it != modContent.end(); ++it)
            {
                delete it->second;
            }

            for(map<string, NODE_CONN<LOGICSTATE>>::iterator it = moduleInputConns.begin(); it != moduleInputConns.end(); ++it)
            {
                delete &it;
            }

        }

    private:
        moduleContent modContent;
        map<string, NODE_CONN<LOGICSTATE>> moduleInputConns;
        map<string, NODE_CONN<LOGICSTATE>*> internalModuleOutputs; 
        vector<string> moduleInputNames;
        vector<string> moduleOutputNames;
        
        //destroy gates before node conns

};



#endif



