#include <gtest/gtest.h>
#include <network/HttpUriPatternProcessor.h>

TEST(HttpUriPatternProcessorUnitTests, test_is_match_pattern_no_parameters) {
  HttpUriPatternProcessor uri_processor;
  uri_processor.process_uri("/blah-blah", "/blah-blah");
  EXPECT_TRUE(uri_processor.is_uri_matched());
  EXPECT_TRUE(uri_processor.get_parameters().empty());
}

TEST(HttpUriPatternProcessorUnitTests, test_not_match_pattern_no_parameters) {
  HttpUriPatternProcessor uri_processor;
  uri_processor.process_uri("/blah-blah", "/blah-blah-1");
  EXPECT_FALSE(uri_processor.is_uri_matched());
  EXPECT_TRUE(uri_processor.get_parameters().empty());
}

TEST(HttpUriPatternProcessorUnitTests,
     test_is_match_pattern_has_one_parameter) {
  HttpUriPatternProcessor uri_processor;
  uri_processor.process_uri("/blah-blah/la-la-la", "/blah-blah/:name");
  EXPECT_TRUE(uri_processor.is_uri_matched());
  ASSERT_FALSE(uri_processor.get_parameters().empty());

  EXPECT_EQ(uri_processor.get_parameters().at(":name"), "la-la-la");
}
