#include "traps.h"

namespace TRAPS
{
    void __TRAP__WARN(){}
    void __TRAP__INFO(){}
    void __TRAP__ERROR(){ std::cout << "catched error.\n";}
    void __TRAP__SYNTAX_ERR(){}
    void __TRAP__UNIMP(){}
    void __TRAP__DEBG(){}
    void __TRAP__ALL(){}
}