/*
 * matrixf_test.cpp
 *
 *  Created on: Apr 5, 2022
 *      Author: Gabriel Dimitriu
 */

#include "../matrixf.h"
#include "../matrix.h"

#include <gtest/gtest.h>

TEST(MATRIXFTestSuite,CONSTRUCTOR_DIMENSIONS) {
	matrixf matrix_test(2,3,4);
	EXPECT_EQ(2,matrix_test.d0())<<"Getter for dimension should be 2";
	EXPECT_EQ(3,matrix_test.d1())<<"Getter for dimension should be 3";
	EXPECT_EQ(4,matrix_test.d2())<<"Getter for dimension should be 4";
}

TEST(MATRIXFTestSuite,OPERATOR_GET_SET) {
	matrixf matrix_test(2,3,4);
	matrix_test(1,1,1) = 10.10;
	EXPECT_FLOAT_EQ(10.10,matrix_test(1,1,1))<<"Value in matrix should be 10.10";
	matrix_test(1,1,3) = 20.21;
	EXPECT_FLOAT_EQ(20.21,matrix_test(1,1,3))<<"Value in matrix should be 20.21";
}

TEST(MATRIXFTestSuite,OPERATOR_ASSIGN) {
	matrixf matrix_test1(2,3,4);
	matrix_test1(1,1,1) = 10.11;
	matrix_test1(1,1,3) = 20.22;
	matrixf matrix_test2(1,1,1);
	matrix_test2 = matrix_test1;
	matrix_test1(1,1,1) = 30.11;
	EXPECT_FLOAT_EQ(30.11,matrix_test1(1,1,1))<<"Value in matrix should be 30.11";
	EXPECT_FLOAT_EQ(10.11,matrix_test2(1,1,1))<<"Value in matrix should be 10.11";
	EXPECT_FLOAT_EQ(20.22,matrix_test2(1,1,3))<<"Value in matrix should be 20.22";
}

TEST(MATRIXFTestSuite,OPERATOR_ASSIGN_MATRIX) {
	matrix matrix_test1(2,3,4);
	matrix_test1(1,1,1) = 10;
	matrix_test1(1,1,3) = 20;
	matrixf matrix_test2(1,1,1);
	matrix_test2 = matrix_test1;
	matrix_test1(1,1,1) = 30;
	EXPECT_EQ(30,matrix_test1(1,1,1))<<"Value in matrix should be 30";
	EXPECT_FLOAT_EQ(10,matrix_test2(1,1,1))<<"Value in matrix should be 10";
	EXPECT_FLOAT_EQ(20,matrix_test2(1,1,3))<<"Value in matrix should be 20";
}

TEST(MATRIXFTestSuite,OPERATOR_ASSIGN_CONSTRUCTOR) {
	matrixf matrix_test1;
	matrixf matrix_test3;
	matrix_test1.init(2,3,4);
	matrix_test1(1,1,1) = 10.11;
	matrix_test1(1,1,3) = 20.22;
	matrixf matrix_test2(1,1,1);
	matrix_test3 = matrix_test2 = matrix_test1;
	matrix_test1(1,1,1) = 30.11;
	EXPECT_FLOAT_EQ(30.11,matrix_test1(1,1,1))<<"Value in matrix should be 30.11";
	EXPECT_FLOAT_EQ(10.11,matrix_test2(1,1,1))<<"Value in matrix should be 10.11";
	EXPECT_FLOAT_EQ(20.22,matrix_test2(1,1,3))<<"Value in matrix should be 20.22";
	EXPECT_FLOAT_EQ(10.11,matrix_test3(1,1,1))<<"Value in matrix should be 10.11";
	EXPECT_FLOAT_EQ(20.22,matrix_test3(1,1,3))<<"Value in matrix should be 20.22";
}

TEST(MATRIXFTestSuite,OPERATOR_OPERATIONS) {
	matrixf matrix_test1;
	matrixf matrix_test3;
	matrix_test1.init(2,3,4);
	matrix_test1(1,1,1) = 2;
	matrix_test1(1,1,3) = 3;
	matrixf matrix_test2(1,1,1);
	matrix_test2 = matrix_test1;
	matrix_test1(1,1,1) = 4;
	matrix_test3.init(3,3,3);
	matrix_test3(2,2,2) = matrix_test1(1,1,1) * matrix_test2(1,1,1);
	EXPECT_FLOAT_EQ(4,matrix_test1(1,1,1))<<"Value in matrix should be 4";
	EXPECT_FLOAT_EQ(3,matrix_test1(1,1,3))<<"Value in matrix should be 3";
	EXPECT_FLOAT_EQ(2,matrix_test2(1,1,1))<<"Value in matrix should be 2";
	EXPECT_FLOAT_EQ(3,matrix_test2(1,1,3))<<"Value in matrix should be 3";
	EXPECT_FLOAT_EQ(8,matrix_test3(2,2,2))<<"Value in matrix should be 8";
}

TEST(MATRIXFTestSuite,OPERATOR_EXCEPTIONS) {
	matrixf matrix_test1(3,3,3);
	EXPECT_FLOAT_EQ(0.0,matrix_test1(3,3,3))<<"Should be 0.0";
}
