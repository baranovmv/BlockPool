#include <CppUTest/TestHarness.h>

#include "allocator.h"

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

TEST_GROUP(Pool) {
    static const size_t BLOCKS_NUM = 8;
    typedef BlockPool<1522, BLOCKS_NUM> EthPool;
    EthPool pool;

    void setup() {
    }

    void teardown() {
    }
};

// Test that the pool won't allocate more than available blocks.
TEST(Pool, max){
    for(size_t i = 0; i < BLOCKS_NUM; ++i){
        EthPool::Block *b = pool.allocate();
        CHECK(b);
    }
    EthPool::Block *b = pool.allocate();
    CHECK_FALSE(b);
}


TEST(Pool, free){
    EthPool::Block *b[BLOCKS_NUM];
    for(size_t i = 0; i < BLOCKS_NUM; ++i){
        b[i] = pool.allocate();
        CHECK(b[i]);
    }
    EthPool::Block *null_block = pool.allocate();
    CHECK_FALSE(null_block);
    b[0]->free();
    EthPool::Block *not_null_block = pool.allocate();
    CHECK(not_null_block);
    CHECK_EQUAL(not_null_block, b[0]);
}
