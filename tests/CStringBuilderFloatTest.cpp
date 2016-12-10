
#include "catch.hpp"
#include "CStringBuilder.hpp"


using namespace tcsb;

SCENARIO( "Floating point CStringBuilder fpconv_dtoa" , "[CStringBuilder]" ) {
    GIVEN( "A vector with some items" ) {
        const size_t bufferSize = 100;
        char buffer[bufferSize];
        CStringBuilder sb(buffer, bufferSize);

        WHEN("There is usual double") {
            int len = sb.fpconv_dtoa(3.14, buffer);

            THEN("We get it stringified") {
                REQUIRE(sb.cStr() == std::string("3.14"));
                REQUIRE(len == 4);
            }
        }

        WHEN("There is very small double") {
            int len = sb.fpconv_dtoa(0.0000003, buffer);

            THEN("We get it with negative exponent") {
                REQUIRE(sb.cStr() == std::string("3e-7"));
                REQUIRE(len == 4);
            }
        }

        WHEN("There is very big double") {
            int len = sb.fpconv_dtoa(3000000000000, buffer);

            THEN("We get it with positive exponent") {
                REQUIRE(sb.cStr() == std::string("3e+12"));
                REQUIRE(len == 5);
            }
        }

        WHEN("There is very big double") {
            int len = sb.fpconv_dtoa(1/3.0, buffer);

            THEN("We get it with positive exponent") {
                REQUIRE(sb.cStr() == std::string("3e+12"));
                REQUIRE(len == 5);
            }
        }
    }
}

