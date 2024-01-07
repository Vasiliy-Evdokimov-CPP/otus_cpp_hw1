/*
#define BOOST_TEST_MODULE test_version

#include "lib.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_version)

BOOST_AUTO_TEST_CASE(test_valid_version)
{
    BOOST_CHECK(version() != 100);
}

BOOST_AUTO_TEST_SUITE_END()
*/

#include "lib.h"

#include <iostream>

int main (int, char **) 
{
    if (version() == 100)
        std::cout << "FAIL" << std::endl;
    
    return 0;
}

/*
#include <gtest/gtest.h>

#include "lib.h"

TEST(Version, Valid) 
{    
    int ver = version();
    ASSERT_LT(ver, 1000);
}

int main(int argc, char** argv) 
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
*/