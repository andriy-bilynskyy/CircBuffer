#include "gtest/gtest.h"
#include "CCircBuffer.h"
#include <stdint.h>
#include <string.h>

class CircBufferTest : public ::testing::Test {
};

TEST_F(CircBufferTest, RW)
{
    CCircBuffer<uint8_t> testBuffer(10, false);
    uint8_t wrData[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    uint8_t rdData[20];

    ASSERT_EQ(testBuffer.size(), 10);
    ASSERT_EQ(testBuffer.free(), 10);
    ASSERT_EQ(testBuffer.used(), 0);
    ASSERT_EQ(testBuffer.put(wrData, 3), 3);
    ASSERT_EQ(testBuffer.free(), 7);
    ASSERT_EQ(testBuffer.used(), 3);
    ASSERT_EQ(testBuffer.get(rdData, sizeof(rdData)), 3);
    ASSERT_EQ(0, memcmp(rdData, wrData, 3));
    ASSERT_EQ(testBuffer.free(), 10);
    ASSERT_EQ(testBuffer.used(), 0);
    ASSERT_EQ(testBuffer.put(wrData, sizeof(wrData)), 10);
    ASSERT_EQ(testBuffer.free(), 0);
    ASSERT_EQ(testBuffer.used(), 10);
    ASSERT_EQ(testBuffer.get(rdData, 5), 5);
    ASSERT_EQ(0, memcmp(rdData, wrData, 5));
    ASSERT_EQ(testBuffer.free(), 5);
    ASSERT_EQ(testBuffer.used(), 5);
    ASSERT_EQ(testBuffer.put(wrData, 3), 3);
    ASSERT_EQ(testBuffer.get(rdData, 7), 7);
    uint8_t pattern1[] = {6,7,8,9,10,1,2};
    ASSERT_EQ(0, memcmp(rdData, pattern1, 7));
    ASSERT_EQ(testBuffer.used(), 1);
    ASSERT_EQ(testBuffer.put(wrData, 9), 9);
    ASSERT_EQ(testBuffer.used(), 10);
    ASSERT_EQ(testBuffer.get(rdData, sizeof(rdData)), 10);
    uint8_t pattern2[] = {3,1,2,3,4,5,6,7,8,9};
    ASSERT_EQ(0, memcmp(rdData, pattern2, 10));
    ASSERT_EQ(testBuffer.used(), 0);
}

TEST_F(CircBufferTest, Template)
{
    CCircBuffer<int> testBuffer(10, false);
    int wrData[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    int rdData[20];

    ASSERT_EQ(testBuffer.size(), 10);
    ASSERT_EQ(testBuffer.free(), 10);
    ASSERT_EQ(testBuffer.used(), 0);
    ASSERT_EQ(testBuffer.put(wrData, 3), 3);
    ASSERT_EQ(testBuffer.free(), 7);
    ASSERT_EQ(testBuffer.used(), 3);
    ASSERT_EQ(testBuffer.get(rdData, sizeof(rdData)/sizeof(int)), 3);
    ASSERT_EQ(0, memcmp(rdData, wrData, 3 * sizeof(int)));
    ASSERT_EQ(testBuffer.free(), 10);
    ASSERT_EQ(testBuffer.used(), 0);
    ASSERT_EQ(testBuffer.put(wrData, sizeof(wrData)/sizeof(int)), 10);
    ASSERT_EQ(testBuffer.free(), 0);
    ASSERT_EQ(testBuffer.used(), 10);
    ASSERT_EQ(testBuffer.get(rdData, 5), 5);
    ASSERT_EQ(0, memcmp(rdData, wrData, 5 * sizeof(int)));
    ASSERT_EQ(testBuffer.free(), 5);
    ASSERT_EQ(testBuffer.used(), 5);
    ASSERT_EQ(testBuffer.put(wrData, 3), 3);
    ASSERT_EQ(testBuffer.get(rdData, 7), 7);
    int pattern1[] = {6,7,8,9,10,1,2};
    ASSERT_EQ(0, memcmp(rdData, pattern1, 7 * sizeof(int)));
    ASSERT_EQ(testBuffer.used(), 1);
    ASSERT_EQ(testBuffer.put(wrData, 9), 9);
    ASSERT_EQ(testBuffer.used(), 10);
    ASSERT_EQ(testBuffer.get(rdData, sizeof(rdData)/sizeof(int)), 10);
    int pattern2[] = {3,1,2,3,4,5,6,7,8,9};
    ASSERT_EQ(0, memcmp(rdData, pattern2, 10 * sizeof(int)));
    ASSERT_EQ(testBuffer.used(), 0);
}

TEST_F(CircBufferTest, Data)
{
    CCircBuffer<int> testBuffer(10, false);
    int wrData[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    int rdData[20];

    ASSERT_EQ(testBuffer.put(wrData, 3), 3);
    ASSERT_EQ(testBuffer[5], 0);
    ASSERT_EQ(testBuffer[1], 2);
    ASSERT_EQ(testBuffer.used(), 3);
    ASSERT_EQ(testBuffer.get(rdData, 2), 2);
    ASSERT_EQ(testBuffer.used(), 1);
    ASSERT_EQ(testBuffer[0], 3);
    ASSERT_EQ(testBuffer.put(wrData, sizeof(wrData)/sizeof(int)), 9);
    ASSERT_EQ(testBuffer[9], 9);
    ASSERT_EQ(testBuffer[6], 6);
    ASSERT_EQ(testBuffer.checkAt(6, rdData, sizeof(rdData)/sizeof(int)), 4);
    int pattern1[] = {6,7,8,9};
    ASSERT_EQ(0, memcmp(rdData, pattern1, 4 * sizeof(int)));
    ASSERT_EQ(testBuffer.get(rdData, 4), 4);
    ASSERT_EQ(testBuffer.checkAt(6, rdData, sizeof(rdData) / sizeof(int)), 0);
    testBuffer.clear();
    ASSERT_EQ(testBuffer.used(), 0);
    ASSERT_EQ(testBuffer.checkAt(0, rdData, sizeof(rdData) / sizeof(int)), 0);
}

TEST_F(CircBufferTest, RWOW)
{
    CCircBuffer<uint8_t> testBuffer(10, true);
    uint8_t wrData[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    uint8_t rdData[20];

    ASSERT_EQ(testBuffer.size(), 10);
    ASSERT_EQ(testBuffer.free(), 10);
    ASSERT_EQ(testBuffer.used(), 0);
    ASSERT_EQ(testBuffer.put(wrData, 3), 3);
    ASSERT_EQ(testBuffer.free(), 7);
    ASSERT_EQ(testBuffer.used(), 3);
    ASSERT_EQ(testBuffer.get(rdData, sizeof(rdData)), 3);
    ASSERT_EQ(0, memcmp(rdData, wrData, 3));
    ASSERT_EQ(testBuffer.free(), 10);
    ASSERT_EQ(testBuffer.used(), 0);
    ASSERT_EQ(testBuffer.put(wrData, sizeof(wrData)), 15);
    ASSERT_EQ(testBuffer.free(), 0);
    ASSERT_EQ(testBuffer.used(), 10);
    ASSERT_EQ(testBuffer.get(rdData, 5), 5);
    uint8_t pattern1[] = {6,7,8,9,10};
    ASSERT_EQ(0, memcmp(rdData, pattern1, 5));
    ASSERT_EQ(testBuffer.free(), 5);
    ASSERT_EQ(testBuffer.used(), 5);
    ASSERT_EQ(testBuffer.put(wrData, 3), 3);
    ASSERT_EQ(testBuffer.get(rdData, 7), 7);
    uint8_t pattern2[] = {11,12,13,14,15,1,2};
    ASSERT_EQ(0, memcmp(rdData, pattern2, 7));
    ASSERT_EQ(testBuffer.used(), 1);
    ASSERT_EQ(testBuffer.put(wrData, 9), 9);
    ASSERT_EQ(testBuffer.used(), 10);
    ASSERT_EQ(testBuffer.get(rdData, sizeof(rdData)), 10);
    uint8_t pattern3[] = {3,1,2,3,4,5,6,7,8,9};
    ASSERT_EQ(0, memcmp(rdData, pattern3, 10));
    ASSERT_EQ(testBuffer.used(), 0);
}

TEST_F(CircBufferTest, TemplateOW)
{
    CCircBuffer<int> testBuffer(10, true);
    int wrData[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    int rdData[20];

    ASSERT_EQ(testBuffer.size(), 10);
    ASSERT_EQ(testBuffer.free(), 10);
    ASSERT_EQ(testBuffer.used(), 0);
    ASSERT_EQ(testBuffer.put(wrData, 3), 3);
    ASSERT_EQ(testBuffer.free(), 7);
    ASSERT_EQ(testBuffer.used(), 3);
    ASSERT_EQ(testBuffer.get(rdData, sizeof(rdData)/sizeof(int)), 3);
    ASSERT_EQ(0, memcmp(rdData, wrData, 3 * sizeof(int)));
    ASSERT_EQ(testBuffer.free(), 10);
    ASSERT_EQ(testBuffer.used(), 0);
    ASSERT_EQ(testBuffer.put(wrData, sizeof(wrData)/sizeof(int)), 15);
    ASSERT_EQ(testBuffer.free(), 0);
    ASSERT_EQ(testBuffer.used(), 10);
    ASSERT_EQ(testBuffer.get(rdData, 5), 5);
    int pattern1[] = {6,7,8,9,10};
    ASSERT_EQ(0, memcmp(rdData, pattern1, 5 * sizeof(int)));
    ASSERT_EQ(testBuffer.free(), 5);
    ASSERT_EQ(testBuffer.used(), 5);
    ASSERT_EQ(testBuffer.put(wrData, 3), 3);
    ASSERT_EQ(testBuffer.get(rdData, 7), 7);
    int pattern2[] = {11,12,13,14,15,1,2};
    ASSERT_EQ(0, memcmp(rdData, pattern2, 7 * sizeof(int)));
    ASSERT_EQ(testBuffer.used(), 1);
    ASSERT_EQ(testBuffer.put(wrData, 9), 9);
    ASSERT_EQ(testBuffer.used(), 10);
    ASSERT_EQ(testBuffer.get(rdData, sizeof(rdData) / sizeof(int)), 10);
    int pattern3[] = {3,1,2,3,4,5,6,7,8,9};
    ASSERT_EQ(0, memcmp(rdData, pattern3, 10 * sizeof(int)));
    ASSERT_EQ(testBuffer.used(), 0);
}

TEST_F(CircBufferTest, DataOW)
{
    CCircBuffer<int> testBuffer(10, true);
    int wrData[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    int rdData[20];

    ASSERT_EQ(testBuffer.put(wrData, 3), 3);
    ASSERT_EQ(testBuffer[5], 0);
    ASSERT_EQ(testBuffer[1], 2);
    ASSERT_EQ(testBuffer.used(), 3);
    ASSERT_EQ(testBuffer.get(rdData, 2), 2);
    ASSERT_EQ(testBuffer.used(), 1);
    ASSERT_EQ(testBuffer[0], 3);
    ASSERT_EQ(testBuffer.put(wrData, sizeof(wrData)/sizeof(int)), 15);
    ASSERT_EQ(testBuffer[9], 15);
    ASSERT_EQ(testBuffer[6], 12);
    ASSERT_EQ(testBuffer.checkAt(6, rdData, sizeof(rdData)/sizeof(int)), 4);
    int pattern1[] = {12,13,14,15};
    ASSERT_EQ(0, memcmp(rdData, pattern1, 4*sizeof(int)));
    ASSERT_EQ(testBuffer.get(rdData, 4), 4);
    ASSERT_EQ(testBuffer.checkAt(6, rdData, sizeof(rdData)/sizeof(int)), 0);
    testBuffer.clear();
    ASSERT_EQ(testBuffer.used(), 0);
    ASSERT_EQ(testBuffer.checkAt(0, rdData, sizeof(rdData)/sizeof(int)), 0);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



