#ifndef __TEST_LATENCY_H
#define __TEST_LATENCY_H

#include "gtest/gtest.h"
#include "gtest/gtest_prod.h"


namespace testSpace{

// The fixture for testing class Foo.
class LatencyTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  LatencyTest() {
    // You can do set-up work for each test here.
  }

  virtual ~LatencyTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    
    // Code here will be called immediately after the constructor (right
    // before each test).
    buffer = new Latency(NULL, 8);
    
    
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
  Latency *buffer;
  


};
}  // namespace

#endif
