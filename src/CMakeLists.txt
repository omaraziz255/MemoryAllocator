add_library(MemoryAllocator SHARED allocator.c)
target_link_libraries(MemoryAllocator ${LIBS})