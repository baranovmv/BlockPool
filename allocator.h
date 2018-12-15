#pragma once

#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <mutex> // Use RTOS's critical sections instead.

// The pool agregates all the storage inside. The allocation method doesn't block a caller
// in case of lack of free blocks. If one needs the caller to wait until a free block appears,
// we have to use counting semaphore instead of critical sections (mutex).
template<size_t SIZE, size_t LEN>
class BlockPool
{
public:     
    class Block
    {
    public:
        uint8_t *data()
        {
            return buf_;
        }

        void free()
        {
            allocator_->free(*this);
        }

        size_t size() const
        {
            return SIZE;
        }

        friend BlockPool;
    private:
        // All blocks initialization is located inside BlockPool constructor.
        // Block mustn't be initiated outside of the pool.
        explicit Block()
        {}

        BlockPool *allocator_;
        bool occupied_;
        uint8_t buf_[SIZE];
    };

    void free(Block &b)
    {
        std::lock_guard<std::mutex> guard(storage_mutex_);
        if(b.occupied_) {
            b.occupied_ = false;
            free_cntr_++;
        }
    }

    // Non-blocking allocation.
    Block* allocate()
    {
        size_t i = 0;

        std::lock_guard<std::mutex> guard(storage_mutex_);
        if(!free_cntr_){
            return NULL;
        }
        for(; i < LEN && storage_[i].occupied_; ++i){}
        assert(i < LEN && free_cntr_);

        storage_[i].occupied_ = true;
        --free_cntr_;
        return &storage_[i];
    }

    BlockPool()
    {
        free_cntr_ = LEN;
        for(size_t i = 0; i < LEN; ++i){
            storage_[i].allocator_ = this;
            storage_[i].occupied_ = false;
            memset(storage_[i].buf_, 0, SIZE);
        }
    }

private:

    BlockPool(const BlockPool&);
    void operator=(const BlockPool&);

    std::mutex storage_mutex_;
    Block storage_[LEN];
    size_t free_cntr_;
};
