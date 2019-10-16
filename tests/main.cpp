#include <cstdio>
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

}

void test_raw_allocator()
{
    
}