#include "stdafx.hpp"
#include "Root.hpp"

namespace lw
{
	void init()
	{
		Root::get().init();
	}

	void terminate()
	{
		Root::get().terminate();
	}

	Root& Root::get()
	{
		static Root singleton;

		return singleton;
	}

	void Root::init()
	{
	}

	void Root::terminate()
	{
	}

}