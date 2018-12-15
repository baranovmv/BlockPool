
#include <CppUTest/CommandLineTestRunner.h>

int main(int argc, char **argv) {
    return CommandLineTestRunner::RunAllTests(argc, argv);
}


// #include "allocator.h"

// #include <iostream>
// #include <string.h>

// typedef BlockPool<1522, 8> EthPool;
// EthPool pool;

// int main()
// {
//     EthPool::Block *block = pool.allocate();
//     memset(block->data(), 0, block->size());
//     char* p = (char*)block->data();
//     return 0;
// }