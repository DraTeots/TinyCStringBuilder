
#include "catch.hpp"
#include "CStringBuilder.hpp"


using namespace tcsb;

SCENARIO( "Floating point CStringBuilder fpconv_dtoa" , "[CStringBuilder]" ) {
    GIVEN( "A vector with some items" ) {
        const size_t bufferSize = 100;
        char buffer[bufferSize];
        CStringBuilder sb(buffer, bufferSize);

        WHEN("There is usual double") {
            size_t len = sb.addf(3.14);

            THEN("We get it stringified") {
                REQUIRE(sb.cStr() == std::string("3.14"));
                REQUIRE(len == 4);
            }
        }

        WHEN("There is very small double") {
            size_t len = sb.addf(0.0000003);

            THEN("We get it with negative exponent") {
                REQUIRE(sb.cStr() == std::string("3e-7"));
                REQUIRE(len == 4);
            }
        }

        WHEN("There is very big double") {
            size_t len = sb.addf(3000000000000);

            THEN("We get it with positive exponent") {
                REQUIRE(sb.cStr() == std::string("3e+12"));
                REQUIRE(len == 5);
            }
        }

        WHEN("There is infinite fraction") {
            size_t len = sb.addf(1/3.0);

            THEN("We get it with positive exponent") {
                REQUIRE(sb.cStr() == std::string("0.3333333333333333"));
                REQUIRE(len == 18);
            }
        }

        WHEN("There is infinite fraction2") {
            size_t len = sb.addf(1/3.0);

            THEN("We get it with positive exponent") {
                REQUIRE(sb.cStr() == std::string("0.3333333333333333"));
                REQUIRE(len == 18);
            }
        }
    }
}

