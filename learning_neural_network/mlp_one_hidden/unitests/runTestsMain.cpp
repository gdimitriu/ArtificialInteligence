/*
 * runTestsMain.cpp
 *
 *  Created on: February 2, 2024
 *      Author: Gabriel Dimitriu
 */

#include <gtest/gtest.h>

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
