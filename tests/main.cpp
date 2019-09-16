#include <stdio.h>
#include <string>
#include <cstring>
#include <istream>
#include <sstream>

#include <memory/pool_allocator.hpp>
#include <memory/typed_pool_allocator.hpp>

/*
NOTE: For this to work now, you have to uncomment 
#define __PA_DEBUG_OUTPUT_
in the pool_allocator hpp files
*/

typedef struct test_struct
{
    int a = 999;
    std::string str = "abc";
    char d = 'd';
    float c = 123;
    char f = 'f';

    std::string to_string()
    {
        std::stringstream stream;
        stream << "a: " << a << std::endl
               << "str: " << str << std::endl
               << "c: " << c << std::endl;

        return stream.str();
    }
} test_struct_t;

void test_typed_allocator();
void test_raw_allocator();

// TODO: Convert to catch2 test
int main(int argc, char **argv)
{
    test_typed_allocator();
    test_raw_allocator();
    return 0;
}

void test_typed_allocator()
{
    printf("\n\nTESTING TYPED ALLOCATOR\n");

    printf("Construct allocator\n");
    allocators::typed_pool_allocator<test_struct> alloc(100);

    std::vector<test_struct *> ptrs;

    printf("Check the in-place construction:\n");
    auto ptr = alloc.allocate();
    std::cout << ptr->to_string();
    alloc.free(ptr);

    for (int i = 0; i < 1000000; i++)
    {
        auto ptr = alloc.allocate();
        ptr->a = 1000;
        ptr->str = "foo";
        ptr->c = 23.234;
        ptrs.push_back(ptr);
    }

    for (auto ptr : ptrs)
    {
        alloc.free(ptr);
    }

    printf("All should be freed, no more allocs...\n");

    ptrs.clear();

    for (int i = 0; i < 1000000; i++)
    {
        auto ptr = alloc.allocate();
        ptr->a = 1000;
        ptr->str = "foo";
        ptr->c = 23.234;
    }

    printf("Done allocating.\n");

    printf("Freeing all...\n");
    alloc.free_pool();

    printf("end\n");
}

void test_raw_allocator()
{
    printf("\n\nTESTING RAW ALLOCATOR\n");

    auto size = sizeof(test_struct_t);
    auto align = alignof(test_struct_t);

    printf("sizeof(test_struct_t): %d\n", size);
    printf("alignof(test_struct_t): %d\n", align);

    printf("Construct allocator\n");
    allocators::pool_allocator alloc;

    printf("All Memblock Info:");
    printf("%s", alloc.all_memblock_info());

    printf("Init Allocator:\n");
    alloc.init_aligned(sizeof(test_struct_t) * 100, sizeof(test_struct_t), alignof(test_struct_t));

    printf("Capacity: %d\n", alloc.capacity());
    printf("Size: %d\n", alloc.size());

    printf("All Memblock Info:");
    printf("%s", alloc.all_memblock_info());

    printf("Capacity: %d\n", alloc.capacity());
    printf("Size: %d\n", alloc.size());

    std::vector<void *> ptrs;

    for (int i = 0; i < 1000000; i++)
    {
        ptrs.push_back(alloc.allocate());
        memset(ptrs.back(), 1, size);
    }

    for (auto ptr : ptrs)
    {
        alloc.free_element(ptr);
    }

    printf("All should be freed, no more allocs...\n");

    ptrs.clear();

    for (int i = 0; i < 1000000; i++)
    {
        ptrs.push_back(alloc.allocate());
        memset(ptrs.back(), 1, size);
    }

    printf("Done allocating.\n");

    printf("Freeing all...\n");
    alloc.free_pool();

    printf("end\n");
}