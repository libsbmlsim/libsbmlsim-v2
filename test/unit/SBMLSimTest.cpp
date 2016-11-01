#include <gtest/gtest.h>
#include "sbmlsim/SBMLSim.h"

namespace {

class SBMLSimTest : public ::testing::Test{};

TEST_F(SBMLSimTest, say) {
  //SBMLSim *sim = new SBMLSim();
  EXPECT_EQ(1, 1);
  //delete sim;
}

} // namespace
