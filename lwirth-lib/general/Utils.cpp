#include "../stdafx.hpp"
#include "Utils.hpp"

#include <thread>
#include "Exceptions.hpp"

namespace lw
{
	void sleep(size_t milliseconds)
    {
	    throw NotImplementedException("lw::sleep is not implemented");
        //#TODO: implement lw::sleep (needs pThread)
	    //std::this_thread::sleep_for()
    }
}


