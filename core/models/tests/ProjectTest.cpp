#include <gtest/gtest.h>

#include <models/Project.h>

TEST(ProjectTest, Noop) {
  tlp::Project project;
  EXPECT_EQ(project.toJson(), "");
}
