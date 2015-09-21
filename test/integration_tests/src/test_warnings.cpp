/*
  Copyright (c) 2014-2015 DataStax

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE cassandra
#endif

#include <boost/test/unit_test.hpp>
#include <boost/test/debug.hpp>

#include "cassandra.h"
#include "test_utils.hpp"

struct WarningsTests : public test_utils::SingleSessionTest {
  WarningsTests() : SingleSessionTest(1, 0) { }
};

BOOST_AUTO_TEST_SUITE(warnings)

BOOST_AUTO_TEST_CASE(aggregate_without_partition_key)
{
  CassVersion version = test_utils::get_version();
  if ((version.major >= 2 && version.minor >= 2) || version.major >= 3) {
    WarningsTests tester;
    test_utils::CassStatementPtr statement(cass_statement_new("SELECT sum(gossip_generation) FROM system.local", 0));
    test_utils::CassLog::reset("Server-side warning: Aggregation query used without partition key");
    test_utils::CassFuturePtr future(cass_session_execute(tester.session, statement.get()));
    BOOST_CHECK(cass_future_error_code(future.get()) == CASS_OK);
    BOOST_CHECK(test_utils::CassLog::message_count() > 0);
  } else {
    boost::unit_test::unit_test_log_t::instance().set_threshold_level(boost::unit_test::log_messages);
    BOOST_TEST_MESSAGE("Unsupported Test for Cassandra v" << version.to_string() << ": Skipping warnings/aggregate_without_partition_key");
    BOOST_REQUIRE(true);
  }
}

BOOST_AUTO_TEST_SUITE_END()

