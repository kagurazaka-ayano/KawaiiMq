/**
 * @file QueueTest.cpp
 * @author ayano
 * @date 1/23/24
 * @brief
*/

#include "Queue.h"
#include "Message.h"
#include "gtest/gtest.h"
#include <thread>
#include <string>
#include <chrono>

namespace KawaiiMQ {
    TEST(QueueTest, PushIncreasesSize) {
        Queue queue("test");
        auto m = makeMessage(0);
        queue.push(m);
        ASSERT_EQ(queue.size(), 1);
    }

    TEST(QueueTest, WaitDecreasesSize) {
        Queue queue("test");
        auto m = makeMessage(0);
        queue.push(m);
        queue.wait();
        ASSERT_EQ(queue.size(), 0);
    }

    TEST(QueueTest, WaitReturnsPushedMessage) {
        Queue queue("test");
        auto m = makeMessage(0);
        queue.push(m);
        ASSERT_EQ(queue.wait(), m);
    }

    TEST(QueueTest, EmptyReturnsTrueWhenQueueIsEmpty) {
        Queue queue("test");
        ASSERT_TRUE(queue.empty());
    }

    TEST(QueueTest, EmptyReturnsFalseWhenQueueIsNotEmpty) {
        Queue queue("test");
        auto m = makeMessage(0);
        queue.push(m);
        ASSERT_FALSE(queue.empty());
    }

    TEST(QueueTest, SingleThreadHighVolumePushAndMultipleThreadsWaiting) {
        Queue queue("test");
        std::thread t1([&]() {
            for (int i = 0; i < 1000; ++i) {
                queue.push(makeMessage(i));
            }
        });

        std::thread t2([&]() { for (int i = 0; i < 500; ++i) queue.wait(); });
        std::thread t3([&]() { for (int i = 0; i < 500; ++i) queue.wait(); });

        t1.join();
        t2.join();
        t3.join();

        ASSERT_EQ(queue.size(), 0);
    }

    TEST(QueueTest, MultipleThreadsPushingAndSingleThreadWaiting) {
        Queue queue("test");

        std::thread t1([&]() { for (int i = 0; i < 500; ++i) queue.push(makeMessage(i)); });
        std::thread t2([&]() { for (int i = 0; i < 500; ++i) queue.push(makeMessage(i)); });

        std::thread t3([&]() {
            for (int i = 0; i < 1000; ++i) {
                queue.wait();
            }
        });

        t1.join();
        t2.join();
        t3.join();

        ASSERT_EQ(queue.size(), 0);
    }
}

