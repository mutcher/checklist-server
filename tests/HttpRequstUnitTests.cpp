#include <gtest/gtest.h>
#include <network/HttpRequest.h>

TEST(HttpRequestUnitTests, test_is_match_pattern_no_parameters) {
  HttpRequest request("GET", "/", "");
  EXPECT_TRUE(request.is_uri_match_to_pattern(""));
  EXPECT_TRUE(request.is_uri_match_to_pattern("/"));
  EXPECT_FALSE(request.is_uri_match_to_pattern("/a"));
}

TEST(HttpRequestUnitTests, test_is_match_pattern_has_parameters) {
  HttpRequest request("GET", "/list/4/blah", "");
  EXPECT_TRUE(request.is_uri_match_to_pattern("/list/:id/blah"));
  EXPECT_TRUE(request.is_uri_match_to_pattern("/list/:id/blah/"));
  EXPECT_TRUE(
      request.is_uri_match_to_pattern("///list/:id_super_long////blah/"));
  EXPECT_FALSE(
      request.is_uri_match_to_pattern("/iotem//list/:id_super_long////blah/"));
  EXPECT_FALSE(
      request.is_uri_match_to_pattern("///list/:id_super_long////blah/some"));
  EXPECT_FALSE(request.is_uri_match_to_pattern("/list/:param/"));
}
