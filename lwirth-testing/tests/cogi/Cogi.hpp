#ifndef LWIRTH_COGI_HPP
#define LWIRTH_COGI_HPP

#include "CogiSim.h"
#include "Graphics/CogiRen.h"

void cogiTest()
{
    CogiSim::inst().start();
	CogiRen::inst().start();
}


#endif //LWIRTH_COGI_HPP