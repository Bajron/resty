
#include "qt_test.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "RegExBuilder.h"

class RegExBuilderShould :public ::testing::Test {
protected:
  resty::RegExBuilder uut;

  void expectPattern(const QString& from, const QString& expectedPattern) {
    auto returned = uut.fromPattern(from);
    EXPECT_TRUE(returned.isValid());
    EXPECT_EQ(expectedPattern, returned.pattern());
  }
};

TEST_F(RegExBuilderShould, Compile) {
  
}

TEST_F(RegExBuilderShould, LeaveSimpleTextAsIs) {
  expectPattern("/simple/link", "/simple/link");
}

TEST_F(RegExBuilderShould, ChangeFieldInBracesIntoNamedGroup) {
  expectPattern("/simple/{name}", R"(/simple/(?<name>[^/?]+))");
  expectPattern("/simple/{name}/{name2}", R"(/simple/(?<name>[^/?]+)/(?<name2>[^/?]+))");
}

TEST_F(RegExBuilderShould, ChangeFieldInBracesIntoNamedGroupWithCustomRegex) {
  expectPattern("/something/{name:[a-z]}", R"(/something/(?<name>[a-z]))");
}

