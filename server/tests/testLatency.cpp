// Test file for latency things

/*
  Things to test:
  IDs match with the message buffer
  the locks get locked
  messages are pushed to correct buffers
  ID gets set


 */


#include "testLatency.h"


using namespace testing;
namespace testSpace{
bool gameObjectsSet;
// Tests that the Latency Adds the message
TEST(LatencyTest, TestPushMessage) {
  
  
  Latency l(NULL, 8);
  l.receiveMessage(3, ":D");

  EXPECT_EQ(1, l.sendBuffer->size());
}

  

// Tests that Foo does Xyz.
TEST_F(LatencyTest, DoesXyz) {
  // Exercises the Xyz feature of Foo.
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
} // end namespace

