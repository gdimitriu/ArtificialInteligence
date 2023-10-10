/*
 * matrix_test.cpp
 *
 *  Created on: Apr 5, 2022
 *      Author: Gabriel Dimitriu
 */

#include "../include/matrix.h"

#include <gtest/gtest.h>

TEST(MATRIXTestSuite,CONSTRUCTOR_DIMENSIONS) {
	matrix matrix_test(2,3,4);
	EXPECT_EQ(2,matrix_test.d0())<<"Getter for dimension should be 2";
	EXPECT_EQ(3,matrix_test.d1())<<"Getter for dimension should be 3";
	EXPECT_EQ(4,matrix_test.d2())<<"Getter for dimension should be 4";
}

TEST(MATRIXTestSuite,OPERATOR_GET_SET) {
	matrix matrix_test(2,3,4);
	matrix_test(1,1,1) = 10;
	EXPECT_FLOAT_EQ(10,matrix_test(1,1,1))<<"Value in matrix should be 10";
	matrix_test(1,1,3) = 20;
	EXPECT_FLOAT_EQ(20,matrix_test(1,1,3))<<"Value in matrix should be 20";
}

TEST(MATRIXTestSuite,OPERATOR_ASSIGN) {
	matrix matrix_test1(2,3,4);
	matrix_test1(1,1,1) = 10.;
	matrix_test1(1,1,3) = 20;
	matrix matrix_test2(1,1,1);
	matrix_test2 = matrix_test1;
	matrix_test1(1,1,1) = 30;
	EXPECT_EQ(30,matrix_test1(1,1,1))<<"Value in matrix should be 30";
	EXPECT_EQ(10,matrix_test2(1,1,1))<<"Value in matrix should be 10";
	EXPECT_EQ(20,matrix_test2(1,1,3))<<"Value in matrix should be 20";
}
