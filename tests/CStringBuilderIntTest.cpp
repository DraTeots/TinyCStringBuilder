
#include "catch.hpp"
#include "CStringBuilder.hpp"


using namespace tcsb;

SCENARIO( "Create and use simple CStringBuilder", "[CStringBuilder]" ) {
    GIVEN( "A vector with some items" ) {
        const size_t bufferSize = 100;
        char buffer[bufferSize];

        CStringBuilder sb(buffer, bufferSize);

        REQUIRE(sb.size() == 0);
        REQUIRE(sb.bufferSize());

        WHEN("Add another string") {
            sb.append("ha!");

            THEN("the size and capacity change") {
                REQUIRE(sb.size() == 3);
            }

            THEN("ha! should be there and 0 terminated") {
                REQUIRE(sb.cStr() == std::string("ha!"));
                REQUIRE(sb.cStr()[3] == '\0');
            }
        }

        WHEN("Append by 0 digit int") {
            sb.append(0);

            THEN("it should be properly displayed") {
                REQUIRE(sb.cStr() == std::string("0"));
                REQUIRE(sb.cStr()[1] == '\0');
            }
        }

        WHEN("Add many digits int is passed") {
            sb.append(-1234);

            THEN("it should be properly displayed") {

                REQUIRE(sb.cStr()[0] == '-');
                REQUIRE(sb.cStr()[1] == '1');
                REQUIRE(sb.cStr()[2] == '2');
                REQUIRE(sb.cStr()[3] == '3');
                REQUIRE(sb.cStr()[4] == '4');
                REQUIRE(sb.cStr()[5] == '\0');
            }
        }

        WHEN("Add int8_t") {
            sb.append((int8_t) -3);

            THEN("it should be properly displayed") {
                REQUIRE(sb.cStr() == std::string("-3"));
            }
        }
        WHEN("Add uint8_t") {
            sb.append((uint8_t) 250);

            THEN("it should be properly displayed") {
                REQUIRE(sb.cStr() == std::string("250"));
            }
        }
        WHEN("Add int16_t") {
            sb.append((int16_t) -30000);

            THEN("it should be properly displayed") {
                REQUIRE(sb.cStr() == std::string("-30000"));
            }
        }
        WHEN("Add uint16_t") {
            sb.append((uint16_t) 60000);

            THEN("it should be properly displayed") {
                REQUIRE(sb.cStr() == std::string("60000"));
            }
        }
        WHEN("Add int32_t") {
            sb.append((int32_t) -2147483600);

            THEN("it should be properly displayed") {
                REQUIRE(sb.cStr() == std::string("-2147483600"));
            }
        }
        WHEN("Add uint32_t") {
            sb.append((uint32_t) 4294967295U);

            THEN("it should be properly displayed") {
                REQUIRE(sb.cStr() == std::string("4294967295"));
            }
        }

        WHEN("Add int64_t") {
            sb.append((int64_t) -9223372036854775800);

            THEN("it should be properly displayed") {
                REQUIRE(sb.cStr() == std::string("-9223372036854775800"));
            }
        }

        WHEN("Add uint64_t") {
            uint64_t value = 18446744073709551614U;
            sb.append(value);

            THEN("it should be properly displayed") {
                REQUIRE(sb.cStr() == std::string("18446744073709551614"));
            }
        }

        WHEN("Add string and value") {
            uint64_t value = 18446744073709551614U;
            sb.addValue("haha!", value);

            THEN("it should be properly displayed") {
                REQUIRE(sb.cStr() == std::string("haha! 18446744073709551614"));
            }
        }
    }
}

SCENARIO( "Insufficient buffer", "[CStringBuilder]" ) {

    GIVEN("A vector with some items") {
        const size_t bufferSize = 5;
        char buffer[bufferSize];
        CStringBuilder sb(buffer, bufferSize);

        WHEN("Add long string") {
            sb.append("123456789");

            THEN("It should be truncated") {
                REQUIRE(sb.cStr() == std::string("1234"));
                REQUIRE(sb.cStr()[4] == '\0');

            }
        }
        AND_WHEN("We add integer to it"){
            THEN("It should not fall over the edges") {
                REQUIRE_NOTHROW(sb.addValue(55));
            }
        }


        WHEN("Add long integer") {
            sb.append(123456789);

            THEN("It should be truncated") {
                //TODO REQUIRE(sb.cStr() == std::string("1234"));
                REQUIRE(sb.cStr()[4] == '\0');
            }
        }
    }
}

