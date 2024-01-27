/**
 * @file ManagerTest.cpp
 * @author ayano
 * @date 1/26/24
 * @brief
*/

#include "kawaiiMQ.h"
#include "gtest/gtest.h"
#include <future>

TEST(ManagerTest, SingletonMiltiThreadTest) {
    auto result1 = std::async(std::launch::async, [](){
        return messaging::MessageQueueManager<messaging::IntMessage>::Instance();
    });
    auto result2 = std::async(std::launch::async, [](){
        return messaging::MessageQueueManager<messaging::IntMessage>::Instance();
    });
    EXPECT_EQ(result1.get(), result2.get());
}

TEST(ManagerTest, RelateAndUnrelate) {
    auto topic1 = messaging::Topic("topic1");
    auto topic2 = messaging::Topic("topic2");
    auto queue11 = messaging::Queue<messaging::IntMessage>("test11");
    auto queue12 = messaging::Queue<messaging::IntMessage>("test12");
    auto queue21 = messaging::Queue<messaging::IntMessage>("test21");
    auto queue22 = messaging::Queue<messaging::IntMessage>("test22");

    auto manager = messaging::MessageQueueManager<messaging::IntMessage>::Instance();
    manager->relate(topic1, queue11);
    manager->relate(topic1, queue12);
    manager->relate(topic2, queue21);
    manager->relate(topic2, queue22);
    EXPECT_EQ(manager->getAllRelatedQueue(topic1).size(), 2);
    EXPECT_EQ(manager->getAllRelatedQueue(topic2).size(), 2);
    manager->unrelate(topic1, queue11);
    EXPECT_EQ(manager->getAllRelatedQueue(topic1).size(), 1);
    EXPECT_EQ(manager->getAllRelatedQueue(topic2).size(), 2);
    manager->unrelate(topic1, queue12);
    EXPECT_EQ(manager->getAllRelatedQueue(topic1).size(), 0);
    EXPECT_EQ(manager->getAllRelatedQueue(topic2).size(), 2);
    manager->unrelate(topic2, queue21);
    EXPECT_EQ(manager->getAllRelatedQueue(topic1).size(), 0);
    EXPECT_EQ(manager->getAllRelatedQueue(topic2).size(), 1);
    manager->unrelate(topic2, queue22);
    EXPECT_EQ(manager->getAllRelatedQueue(topic1).size(), 0);
    EXPECT_EQ(manager->getAllRelatedQueue(topic2).size(), 0);
}

TEST(ManagerTest, RelateAndUnrelateMultiThread) {
    auto topic1 = messaging::Topic("topic1");
    auto topic2 = messaging::Topic("topic2");
    auto queue11 = messaging::Queue<messaging::IntMessage>("test11");
    auto queue12 = messaging::Queue<messaging::IntMessage>("test12");
    auto queue21 = messaging::Queue<messaging::IntMessage>("test21");
    auto queue22 = messaging::Queue<messaging::IntMessage>("test22");

    auto manager = messaging::MessageQueueManager<messaging::IntMessage>::Instance();
    auto result1 = std::async(std::launch::async, [&](){
        manager->relate(topic1, queue11);
        manager->relate(topic1, queue12);
        manager->relate(topic2, queue21);
        manager->relate(topic2, queue22);
    });
    auto result2 = std::async(std::launch::async, [&](){
        manager->unrelate(topic1, queue11);
        manager->unrelate(topic1, queue12);
        manager->unrelate(topic2, queue21);
        manager->unrelate(topic2, queue22);
    });
    result1.get();
    result2.get();
    EXPECT_EQ(manager->getAllRelatedQueue(topic1).size(), 0);
    EXPECT_EQ(manager->getAllRelatedQueue(topic2).size(), 0);
}
