#pragma once

#include "../Standard.hpp"

#include <string>

#define MAKE_EXC(DIM, M)						\
class DIM : public lw::Exception				\
{											\
private:									\
	std::string msg = M; 					\
public:										\
	DIM()										\
	{										\
											\
	}										\
											\
	DIM(const char* m)						\
	{										\
		msg.append(": ");					\
		msg.append(m);						\
	}										\
											\
											\
	const char* what() const throw()		\
	{										\
		return msg.c_str();					\
	}										\
};										

#include <string>



namespace lw
{
	class Exception : std::exception
	{
	
	public:

		virtual const char* what() const throw() = 0;
	};

	MAKE_EXC(AllocatorOutOfMemoryException, "Allocator has run out of memory");
	MAKE_EXC(MalformedPointerException, "Pointer is not valid in this context");
	MAKE_EXC(IllSizedContainer, "Container has illegal size in this context");

	MAKE_EXC(AlreadyInitializedException, "Object was already initialized");
	MAKE_EXC(AlreadyCreatedException, "Object was already created.");
	MAKE_EXC(AlreadyAllocatedException, "Object was already allocated.");
	MAKE_EXC(NotCreatedException, "Object was not created.");
	MAKE_EXC(NotInitializedException, "Object was not initialized.");
	MAKE_EXC(NotAllocatedException, "Object was not allocated.");
	MAKE_EXC(NotDestroyedException, "Object was not destroyed.");
	MAKE_EXC(NotFreedException, "Object was not freed.");
	MAKE_EXC(IllegalStateException, "Object is in an illegal state.");
	MAKE_EXC(NotReadyException, "Object is not ready to be used.");

	MAKE_EXC(IllegalIndexException, "Out of bounds.");
	MAKE_EXC(MathException, "Mathmatical error.");

	MAKE_EXC(VulkanException, "A Vulkan error was encountered.");
}


