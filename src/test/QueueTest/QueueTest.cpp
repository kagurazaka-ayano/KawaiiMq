/**
 * @file QueueTest.cpp
 * @author ayano
 * @date 1/23/24
 * @brief
*/

#include "Queue.hpp"
#include "IMessage.h"
#include "gtest/gtest.h"
#include <thread>
#include <string>
#include <chrono>

TEST(QueueTest, PushAndWait) {
    auto queue = messaging::Queue<messaging::StringMessage>("test");
    std::string out;
    std::stringstream str;

    for(int i = 0; i < 10; ++i){
        str << i;
        queue.push(messaging::StringMessage(str.str()));
        out = queue.wait()->getContent();
        EXPECT_EQ(out, str.str());
        str.str("");
    }
}

void pusher(messaging::Queue<messaging::StringMessage>& queue_in) {
    for (int i = 0; i < 10; ++i) {
        std::stringstream str;
        str << i;
        queue_in.push(str.str());
        str.str("");
    }
}

void fetcher(messaging::Queue<messaging::StringMessage>& queue_in) {
    while(!queue_in.empty()) {
        for (int i = 0; i < 10; ++i) {
            std::stringstream str;
            str << i;
            auto ret = queue_in.wait()->getContent();
            EXPECT_EQ(ret, str.str());
            str.str("");
        }
    }
}

TEST(QueueTest, Multithread) {
    auto queue = messaging::Queue<messaging::StringMessage>("test");
    auto th1 = std::thread(pusher, std::ref(queue));
    auto th2 = std::thread(fetcher, std::ref(queue));
    th1.join();
    th2.join();
}
