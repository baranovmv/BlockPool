#include <CppUTest/TestHarness.h>
#include <chrono>
#include <thread>

#include "allocator.h"


TEST_GROUP(Pool)
{
    static const size_t BLOCKS_NUM = 8;
    typedef BlockPool<1522, BLOCKS_NUM> EthPool;
    static EthPool pool;

    void setup() {
    }

    void teardown() {
        pool.reset();
    }

    static void loadtask(const size_t iterations, const size_t blocknum)
    {
        EthPool::Block *b[blocknum];
        size_t allocated = 0;
        bool allocate_or_free = false;
        for(size_t i=0; i < iterations; ++i){
            if(allocated < blocknum &&  i % blocknum == 0 && !allocate_or_free){
                b[allocated] = pool.allocate();
                CHECK(b[allocated]);
                memset(b[allocated]->data(), blocknum, b[allocated]->size());
                ++allocated;
                if(allocated >= blocknum){
                    allocate_or_free = !allocate_or_free;
                }
            } else if(allocated > 0 && i % blocknum == 0 && allocate_or_free){
                --allocated;
                b[allocated]->free();
                if(!allocated){
                    allocate_or_free = !allocate_or_free;
                }
            } else if(allocated && b[allocated-1]->data()){
                for(size_t j=0; j < b[allocated-1]->size(); ++j){
                    CHECK_EQUAL(b[allocated-1]->data()[j], blocknum);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        for(size_t i=0; i < blocknum; ++i){
            if(b[i]->data()){
                b[i]->free();
            }
        }
    }

};
TEST_GROUP_CppUTestGroupPool::EthPool TEST_GROUP_CppUTestGroupPool::pool;

// Test that the pool won't allocate more than available blocks.
TEST(Pool, max)
{
    EthPool::Block *b[BLOCKS_NUM];
    for(size_t i = 0; i < BLOCKS_NUM; ++i){
        b[i] = pool.allocate();
        CHECK(b);
    }
    EthPool::Block *b_max = pool.allocate();
    CHECK_FALSE(b_max);
    for(size_t i=0; i < BLOCKS_NUM; ++i){
        b[i]->free();
    }
}


TEST(Pool, free)
{
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

    for(size_t i = 0; i < BLOCKS_NUM; ++i){
        b[i]->free();
    }
    not_null_block->free();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

TEST(Pool, loadtest)
{
    std::thread t1(loadtask, 1000, 3);
    std::thread t2(loadtask, 1000, 4);

    t1.join();
    t2.join();
}

