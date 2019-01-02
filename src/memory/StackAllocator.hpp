#pragma once

#include "../Standard.hpp"

#include "List.hpp"
#include "Allocations.hpp"
#include <type_traits>

namespace lw
{

	template<class T = byte, class Allocator = std::allocator<T>>
	class StackAllocator
	{
		class Marker
		{
		public:
			T* m_value;
			size_t m_destructorHandle;
			Marker(T* value, size_t destructorHandle)
				: m_value(value), m_destructorHandle(destructorHandle)
			{
			}
		};

		class ElementDestructor
		{
		private:
			const void* m_data;
			void (*destructor)(const void*);
		public:
			template<typename U>
			explicit ElementDestructor(const U& data) noexcept :
				m_data(std::addressof(data)), destructor(MEM::execDestructor<U>)
			{}

			void operator()() noexcept
			{
				destructor(m_data);
			}
		};

		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using size_type = size_t;
		using difference_type = std::ptrdiff_t;
		
	private:
		static constexpr size_t DEFAULT_STACK_SIZE = 1024;
		T* m_data = nullptr;
		T* m_head = nullptr;
		size_t m_size = 0;

		Allocator* m_allocator = nullptr;
		bool m_shouldDestroyAllocator = false;

		List<ElementDestructor> m_destructors;


	public:
		StackAllocator(const StackAllocator& other) = delete; //Copy Constructor
		StackAllocator(StackAllocator&& other) = delete; //Move Constructor
		StackAllocator& operator=(const StackAllocator& other) = delete; //Copy Assignment
		StackAllocator& operator=(StackAllocator&& other) = delete; //Move Assignment

		explicit StackAllocator(size_t stackSize = DEFAULT_STACK_SIZE, Allocator* allocator = nullptr)
			: m_size(stackSize), m_allocator(allocator)
		{
			if (!m_allocator)
			{
				m_allocator = new Allocator();
				m_shouldDestroyAllocator = true;
			}
			else
			{
				m_shouldDestroyAllocator = false;
			}
			m_data = m_allocator->allocate(m_size);
			m_head = m_data;
		}

		~StackAllocator()
		{
			if (m_head != m_data)
			{
				debugBreak();
			}

			if (m_data && m_allocator)
			{
				m_allocator->deallocate(m_data, m_size);
			}
			if (m_shouldDestroyAllocator)
			{
				delete m_allocator;
			}
			m_data = nullptr;
			m_head = nullptr;
		}

		

		template<class U, class... Args>
		U* allocateObjects(size_t objCount = 1, Args&&... args)
		{
			T* allocPtr = (T*)lw::nextMultiple(alignof(U), (uintptr_t)m_head);
			T* newHeadPtr = (T*)((uintptr_t)allocPtr) + objCount * sizeof(U);
			if (newHeadPtr <= m_data + m_size)
			{
				U* returnPtr = reinterpret_cast<U*>(allocPtr);
				m_head = newHeadPtr;
				for (size_t i = 0; i < objCount; i++)
				{
					U* obj = allocPtr + i;
					new(obj) U(std::forward<Args>(args)...); //only initializes (doesn't allocate)
					addDestructor(obj);
				}
				return returnPtr;
			}
			else
			{
				throw AllocatorOutOfMemoryException();
			}
		}

		template<class U, class... Args>
		U* allocateObject(Args&&... args)
		{
			return allocateObjects<U>(1, std::forward<Args>(args)...);
		}

		void* allocate(size_t memTypeCount, size_t alignment = alignof(T))
		{
			T* allocPtr = lw::nextMultiple(alignment, (uintptr_t)m_head);
			T* newHeadPtr = allocPtr + memTypeCount;
			if (newHeadPtr <= m_data + m_size)
			{
				m_head = newHeadPtr;
				return allocPtr;
			}
			else
			{
				throw AllocatorOutOfMemoryException();
			}
		}

		Marker getMarker()
		{
			return Marker(m_head, m_destructors.size());
		}

		void freeToMarker(Marker marker)
		{
			m_head = marker.m_value;
			while (m_destructors.size() > marker.m_destructorHandle)
			{
				m_destructors.back()();
				m_destructors.pop();
			}
		}
		
		void freeAllObjects()
		{
			m_head = m_data;
			while (m_destructors.size() > 0)
			{
				m_destructors.back()();
				m_destructors.pop();
			}
		}


	private:
		template<class U>
		inline std::enable_if_t<std::is_trivially_destructible<U>::value> addDestructor(U* obj)
		{
			//do nothing
		}

		template<class U>
		inline std::enable_if_t<!std::is_trivially_destructible<U>::value> addDestructor(U* obj)
		{
			m_destructors.push(ElementDestructor(*obj));
		}
	};
	
}