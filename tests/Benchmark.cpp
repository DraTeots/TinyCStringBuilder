#include "catch.hpp"
#include <cstdlib>
#include <chrono>
#include <iostream>
#include "CStringBuilder.hpp"

using namespace tcsb;

uint32_t HAL_GetTick() {
    return std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
}


TEST_CASE("CSctingBuilder vs. snprintf", "[Benchmark]") {
    int x;
    uint64_t count;
    char buffer[100];

    // -------- REVERSE BENCHMARK 1-----------
    uint32_t start = HAL_GetTick();
    uint64_t countDown = 100000;

    while (countDown--) {
        x = std::rand();
        count++;
        if (!x)
        {
            INFO(buffer);
        }
    }
    uint32_t stop = HAL_GetTick();
    sprintf(buffer, "rand %u", stop - start);
    //std::cout<<buffer<<std::endl;


    // -------- REVERSE BENCHMARK 2 -----------
    start = HAL_GetTick();
    countDown = 100000;

    while (countDown--) {
        x = std::rand();
        snprintf(buffer, 100, "simple insertion of x=%i", x);
        count++;
        if (!x)
        {
            INFO(buffer);
        }
    }
    stop = HAL_GetTick();
    sprintf(buffer, "snprintf %u", stop - start);
    INFO(buffer);


    // -------- REVERSE BENCHMARK 3 -----------
    start = HAL_GetTick();
    countDown = 10000000;

    while (countDown--) {
        x = std::rand();
        CStringBuilder sb(buffer, 100);
        sb.add("simple insertion of x=");
        sb.add((uint32_t)x);
        count++;
        if (!x)
        {
            INFO(buffer);
        }
    }
    stop = HAL_GetTick();
    sprintf(buffer, "CStringBuilder %u", stop - start);
    INFO(buffer);
    REQUIRE(true);
}