#define BOOST_TEST_MODULE MASTER_TEST_SUITE_NAME
#include <boost/test/included/unit_test.hpp>

#include "test_servers.hpp"

BOOST_FIXTURE_TEST_CASE( make_sure_unsupported_requests_are_answered_correct, test::small_temperature_service_with_response<> )
{
    BOOST_CHECK( check_error_response( { 0x65 }, 0x65, 0x0000, 0x06 ) );
}
