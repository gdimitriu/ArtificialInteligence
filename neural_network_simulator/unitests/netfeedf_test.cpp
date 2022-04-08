/*
 * netfeedf_test.cpp
 *
 *  Created on: Apr 7, 2022
 *      Author: Gabriel Dimitriu
 */


#include <gtest/gtest.h>
#include "../netfeedf.h"

TEST(NETFEEDFTestSuite,ALLOCATE_DEALOCATE) {
	net_feedf net0(0,2,0,0,2);
	net0.create();
	net0.init_net();
	net0.free_net();
	net_feedf net1(1,10,5,0,2);
	net1.create();
	net1.init_net();
	net1.free_net();
	net_feedf net2(2,10,5,3,2);
	net2.create();
	net2.init_net();
	net2.free_net();
}

TEST(NETFEEDFTestSuite,ZEROES_FORWARD) {
	matrix inputs(1,1,10);
	net_feedf net(0,10,0,0,10);
	net.create();
	net.init_net();
	net.init_inp(inputs);
	net.forward();
	matrixf outputs = net.get_outputs();
	for (int i = 0 ;i < 10; i++) {
		EXPECT_FLOAT_EQ(0.0,outputs(0,0,i))<<"Should be zero";
	}
}
