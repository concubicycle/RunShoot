#include "pool_allocator_tests.h"
#include "typed_pool_allocator_tests.h"
#include "archetype_pool_tests.h"


int main() {
    test_pool_allocator_all();
    test_typed_pool_allocator_all();
    test_archetype_pool_all();

    return 0;
}

