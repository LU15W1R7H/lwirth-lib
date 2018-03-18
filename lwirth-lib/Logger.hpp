#pragma once

#include "Standard.hpp"

#include <iostream>

namespace lw
{
	enum MessageType { INFO_M, WARNING_M, ERROR_M };

	class Logger
	{
	public:

		template<typename T>
		static std::ostream& print(T t, MessageType type = INFO_M)
		{
			switch (type)
			{
			case INFO_M:
				return std::cout << "INFO: " << t;
			case WARNING_M:
				return std::cout << "WARNING: " << t;
			case ERROR_M:
				return std::cout << "ERROR: " << t;
			}
		}

		template<typename T>
		static std::ostream& println(T t, MessageType type = INFO_M)
		{
			switch (type)
			{
			case INFO_M:
				return std::cout << "INFO: " << t << std::endl;
			case WARNING_M:
				return std::cout << "WARNING: " << t << std::endl;
			case ERROR_M:
				return std::cout << "ERROR: " << t << std::endl;
			}
		}
	};
}



