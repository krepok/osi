#include <gtest/gtest.h>
#include "marker.h"

#include <algorithm>
#include <sstream>

TEST(MarkerTests, ResetAllWorks)
{
    hStop = { CreateEvent(nullptr, TRUE, TRUE, nullptr) };  // сразу в signaled
    hTerm = { CreateEvent(nullptr, TRUE, TRUE, nullptr) };
    numOfThreads = 5;

    resetAll();

    EXPECT_EQ(numOfThreads, 0);
    EXPECT_EQ(WaitForSingleObject(hStop[0], INFINITY), WAIT_TIMEOUT);
    EXPECT_EQ(WaitForSingleObject(hTerm[0], INFINITY), WAIT_TIMEOUT);

    CloseHandle(hStop[0]);
    CloseHandle(hTerm[0]);
    hStop.clear();
    hTerm.clear();
}

// Проверяем printArray: элементы через пробел, в конце перевод строки
TEST(MarkerTests, PrintArrayPrintsElements)
{
    arr = { 1, 0, 2 };

    std::ostringstream out;
    auto oldOut = std::cout.rdbuf(out.rdbuf());

    printArray();

    std::cout.rdbuf(oldOut);
    EXPECT_EQ(out.str(), "1 0 2 \n");
}

TEST(MarkerTests, ReadMarkersCountRetriesOnInvalid)
{
    std::istringstream in("abc\n0\n100\n3\n");
    std::ostringstream out;
    auto oldIn = std::cin.rdbuf(in.rdbuf());
    auto oldOut = std::cout.rdbuf(out.rdbuf());

    int result = readMarkersCount();

    std::cin.rdbuf(oldIn);
    std::cout.rdbuf(oldOut);
    EXPECT_EQ(result, 3);
}

TEST(MarkerTests, ReadMarkerIdValid)
{
    std::vector<bool> alive = { true, true, true };
    std::istringstream in("2\n");
    std::ostringstream out;
    auto oldIn = std::cin.rdbuf(in.rdbuf());
    auto oldOut = std::cout.rdbuf(out.rdbuf());

    int result = readMarkerId(3, alive);

    std::cin.rdbuf(oldIn);
    std::cout.rdbuf(oldOut);
    EXPECT_EQ(result, 2);
}

TEST(MarkerTests, ReadMarkerIdSkipsInvalidAndDead)
{
    std::vector<bool> alive = { true, false, true };
    std::istringstream in("abc\n0\n4\n2\n3\n");
    std::ostringstream out;
    auto oldIn = std::cin.rdbuf(in.rdbuf());
    auto oldOut = std::cout.rdbuf(out.rdbuf());

    int result = readMarkerId(3, alive);

    std::cin.rdbuf(oldIn);
    std::cout.rdbuf(oldOut);
    EXPECT_EQ(result, 3);
}