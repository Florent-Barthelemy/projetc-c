#ifndef TRAPS_H_
#define TRAPS_H_

#include <iostream>

namespace TRAPS
{
    void __TRAP__WARN();
    void __TRAP__INFO();
    void __TRAP__ERROR();
    void __TRAP__SYNTAX_ERR();
    void __TRAP__UNIMP();
    void __TRAP__DEBG();
    void __TRAP__ALL();
}


#endif /*TRAPS_H_*/