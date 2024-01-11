#include <shared_mutex.hpp>
#include <gtest/gtest.h>

#include <shared_mutex>

TEST(stdcpp_shared_mutex, lockShard_duplicate_lock)
{
    stdcpp::shared_mutex mtx;
    mtx.lock_shared();
    ASSERT_TRUE(mtx.try_lock_shared());
    mtx.unlock_shared();
    mtx.unlock_shared();
}

TEST(stdcpp_shared_mutex, blockExclusive_duplicate_lock)
{
    stdcpp::shared_mutex mtx;
    mtx.lock();
    ASSERT_FALSE(mtx.try_lock());
    mtx.unlock();

    mtx.lock_shared();
    ASSERT_FALSE(mtx.try_lock());
    mtx.unlock_shared();

    ASSERT_TRUE(mtx.try_lock());
    mtx.unlock();
}

TEST(stdcpp_shared_mutex, lockWithUniqueLock_LockingCS_ExpectNotReentrant)
{
    stdcpp::shared_mutex mtx;
    std::unique_lock<stdcpp::shared_mutex> lk(mtx);
    ASSERT_FALSE(mtx.try_lock());
    ASSERT_FALSE(mtx.try_lock_shared());
    lk.unlock();
    ASSERT_TRUE(mtx.try_lock());
    ASSERT_FALSE(mtx.try_lock_shared());
    mtx.unlock();
}

TEST(stdcpp_shared_mutex, shared_lock)
{
    stdcpp::shared_mutex mtx;
    std::shared_lock<stdcpp::shared_mutex> lk(mtx);
    ASSERT_FALSE(mtx.try_lock());
    ASSERT_TRUE(mtx.try_lock_shared());
    lk.unlock();
    ASSERT_FALSE(mtx.try_lock());
    ASSERT_TRUE(mtx.try_lock_shared());
    mtx.unlock_shared();
}
