#ifndef ARENA_HPP
#define ARENA_HPP

#include <iostream>

class ArenaAllocator{
	public:
		inline explicit ArenaAllocator(size_t byte)
			: m_size(byte)
		{
			m_buffer = static_cast<std::byte*>(malloc(m_size));
			m_offset = m_buffer;
		};

		inline ArenaAllocator(const ArenaAllocator& other) = delete;

		inline ArenaAllocator operator=(const ArenaAllocator& other) = delete;

		template<typename T>
		inline T* alloc(){
			void* offset = m_offset;
			m_offset += sizeof(T);
			return static_cast<T*>(offset);
		}

		inline ~ArenaAllocator(){
			free(m_buffer);
		}

	private:
		size_t m_size;
		std::byte* m_buffer;
		std::byte* m_offset;
};

#endif
