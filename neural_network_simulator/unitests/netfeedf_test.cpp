/*
 * netfeedf_test.cpp
 *
 *  Created on: Apr 7, 2022
 *      Author: Gabriel Dimitriu
 */


#include <gtest/gtest.h>
#include "../netfeedf.h"


static int max(matrixf &out) {
	int index = 0;
	float maxim = out(0, 0, 0);
	for (int i = 1; i < out.d2(); i++) {
		if (maxim < out(0, 0, i)) {
			maxim = out(0, 0, i);
			index = i;
		}
	}
	return index;
}

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
	net.set_temp(2);
	net.forward();
	matrixf outputs = net.get_outputs();
	for (int i = 0 ;i < 10; i++) {
		EXPECT_FLOAT_EQ(0.5,outputs(0,0,i))<<"Should be 0.5";
	}
}

TEST(NETFEEDFTestSuite,FILE_SAVE) {
	matrix inputs(1,1,10);
	net_feedf net(0,10,0,0,10);
	net.put_name("TestNet");
	net.create();
	net.init_net();
	net.init_inp(inputs);
	net.set_temp(2);
	EXPECT_STREQ("TestNet",net.namenet())<<"Should be TestNet";
	net.save("./test.dat");
	net.put_name("TestNet2");
	EXPECT_STREQ("TestNet2",net.namenet())<<"Should be TestNet2";
	net.load_inf("./test.dat");
	EXPECT_STREQ("TestNet",net.namenet())<<"Should be TestNet";
}

TEST(NETFEEDFTestSuite,FILE_LOAD) {
	matrix inputs(1,1,10);
	net_feedf net;
	net.load_inf("./test.dat");
	net.create();
	net.init_net();
	net.init_inp(inputs);
	net.set_temp(2);
	net.forward();
	EXPECT_STREQ("TestNet",net.namenet())<<"Should be TestNet";
	matrixf outputs = net.get_outputs();
	for (int i = 0 ;i < 10; i++) {
		EXPECT_FLOAT_EQ(0.5,outputs(0,0,i))<<"Should be 0.5";
	}
}

TEST(NETFEEDFTestSuite,FILE_LOAD_TRAINNING) {
	net_feedf net;
	net.create();
	net.init_net();
	net.load_trainning_text_File("./test_trainning_load.txt");
	matrixf &inputs = net.get_inp();
	matrixf &expected = net.get_tp();
	EXPECT_EQ(1,inputs.d0())<<"dim0 should 1";
	EXPECT_EQ(1,inputs.d1())<<"dim1 should 1";
	EXPECT_EQ(6,inputs.d2())<<"dim0 should 6";
	EXPECT_FLOAT_EQ(0.5,inputs(0,0,0))<<"Input should be 0.5";
	EXPECT_FLOAT_EQ(2.5,inputs(0,0,1))<<"Input should be 2.5";
	EXPECT_FLOAT_EQ(6.5,inputs(0,0,2))<<"Input should be 6.5";
	EXPECT_FLOAT_EQ(10,inputs(0,0,3))<<"Input should be 10";
	EXPECT_FLOAT_EQ(20,inputs(0,0,4))<<"Input should be 20";
	EXPECT_FLOAT_EQ(30,inputs(0,0,5))<<"Input should be 30";
	EXPECT_EQ(1,expected.d0())<<"dim0 should 1";
	EXPECT_EQ(1,expected.d1())<<"dim1 should 1";
	EXPECT_EQ(6,expected.d2())<<"dim0 should 6";
	EXPECT_FLOAT_EQ(0.5,expected(0,0,0))<<"Expected should be 0.5";
	EXPECT_FLOAT_EQ(2.5,expected(0,0,1))<<"Expected should be 2.5";
	EXPECT_FLOAT_EQ(6.5,expected(0,0,2))<<"Expected should be 6.5";
	EXPECT_FLOAT_EQ(10,expected(0,0,3))<<"Expected should be 10";
	EXPECT_FLOAT_EQ(20,expected(0,0,4))<<"Expected should be 20";
	EXPECT_FLOAT_EQ(30,expected(0,0,5))<<"Expected should be 30";
}

TEST(NETFEEDFTestSuite,FILE_LOAD_SUITE_TRAINNING) {
	net_feedf net;
	net.create();
	net.init_net();
	net.load_trainning_suite_text_file("./test_suite_load.txt");
	matrixf &inputs = net.get_trainning_inp(0);
	matrixf &expected = net.get_trainning_tp(0);
	EXPECT_EQ(1,inputs.d0())<<"dim0 should 1";
	EXPECT_EQ(1,inputs.d1())<<"dim1 should 1";
	EXPECT_EQ(6,inputs.d2())<<"dim0 should 6";
	EXPECT_FLOAT_EQ(0.5,inputs(0,0,0))<<"Input should be 0.5";
	EXPECT_FLOAT_EQ(2.5,inputs(0,0,1))<<"Input should be 2.5";
	EXPECT_FLOAT_EQ(6.5,inputs(0,0,2))<<"Input should be 6.5";
	EXPECT_FLOAT_EQ(10,inputs(0,0,3))<<"Input should be 10";
	EXPECT_FLOAT_EQ(20,inputs(0,0,4))<<"Input should be 20";
	EXPECT_FLOAT_EQ(30,inputs(0,0,5))<<"Input should be 30";
	EXPECT_EQ(1,expected.d0())<<"dim0 should 1";
	EXPECT_EQ(1,expected.d1())<<"dim1 should 1";
	EXPECT_EQ(6,expected.d2())<<"dim0 should 6";
	EXPECT_FLOAT_EQ(0.5,expected(0,0,0))<<"Expected should be 0.5";
	EXPECT_FLOAT_EQ(2.5,expected(0,0,1))<<"Expected should be 2.5";
	EXPECT_FLOAT_EQ(6.5,expected(0,0,2))<<"Expected should be 6.5";
	EXPECT_FLOAT_EQ(10,expected(0,0,3))<<"Expected should be 10";
	EXPECT_FLOAT_EQ(20,expected(0,0,4))<<"Expected should be 20";
	EXPECT_FLOAT_EQ(30,expected(0,0,5))<<"Expected should be 30";
	inputs = net.get_trainning_inp(1);
	expected = net.get_trainning_tp(1);
	EXPECT_EQ(1,inputs.d0())<<"dim0 should 1";
	EXPECT_EQ(1,inputs.d1())<<"dim1 should 1";
	EXPECT_EQ(6,inputs.d2())<<"dim0 should 6";
	EXPECT_FLOAT_EQ(0.5,inputs(0,0,0))<<"Input should be 0.5";
	EXPECT_FLOAT_EQ(2.5,inputs(0,0,1))<<"Input should be 2.5";
	EXPECT_FLOAT_EQ(6.5,inputs(0,0,2))<<"Input should be 6.5";
	EXPECT_FLOAT_EQ(10,inputs(0,0,3))<<"Input should be 10";
	EXPECT_FLOAT_EQ(20,inputs(0,0,4))<<"Input should be 20";
	EXPECT_FLOAT_EQ(40,inputs(0,0,5))<<"Input should be 40";
	EXPECT_EQ(1,expected.d0())<<"dim0 should 1";
	EXPECT_EQ(1,expected.d1())<<"dim1 should 1";
	EXPECT_EQ(6,expected.d2())<<"dim0 should 6";
	EXPECT_FLOAT_EQ(0.5,expected(0,0,0))<<"Expected should be 0.5";
	EXPECT_FLOAT_EQ(2.5,expected(0,0,1))<<"Expected should be 2.5";
	EXPECT_FLOAT_EQ(6.5,expected(0,0,2))<<"Expected should be 6.5";
	EXPECT_FLOAT_EQ(10,expected(0,0,3))<<"Expected should be 10";
	EXPECT_FLOAT_EQ(20,expected(0,0,4))<<"Expected should be 20";
	EXPECT_FLOAT_EQ(40,expected(0,0,5))<<"Expected should be 40";
	inputs = net.get_trainning_inp(2);
	expected = net.get_trainning_tp(2);
	EXPECT_EQ(1,inputs.d0())<<"dim0 should 1";
	EXPECT_EQ(1,inputs.d1())<<"dim1 should 1";
	EXPECT_EQ(6,inputs.d2())<<"dim0 should 6";
	EXPECT_FLOAT_EQ(0.5,inputs(0,0,0))<<"Input should be 0.5";
	EXPECT_FLOAT_EQ(2.5,inputs(0,0,1))<<"Input should be 2.5";
	EXPECT_FLOAT_EQ(6.5,inputs(0,0,2))<<"Input should be 6.5";
	EXPECT_FLOAT_EQ(10,inputs(0,0,3))<<"Input should be 10";
	EXPECT_FLOAT_EQ(20,inputs(0,0,4))<<"Input should be 20";
	EXPECT_FLOAT_EQ(50,inputs(0,0,5))<<"Input should be 50";
	EXPECT_EQ(1,expected.d0())<<"dim0 should 1";
	EXPECT_EQ(1,expected.d1())<<"dim1 should 1";
	EXPECT_EQ(6,expected.d2())<<"dim0 should 6";
	EXPECT_FLOAT_EQ(0.5,expected(0,0,0))<<"Expected should be 0.5";
	EXPECT_FLOAT_EQ(2.5,expected(0,0,1))<<"Expected should be 2.5";
	EXPECT_FLOAT_EQ(6.5,expected(0,0,2))<<"Expected should be 6.5";
	EXPECT_FLOAT_EQ(10,expected(0,0,3))<<"Expected should be 10";
	EXPECT_FLOAT_EQ(20,expected(0,0,4))<<"Expected should be 20";
	EXPECT_FLOAT_EQ(50,expected(0,0,5))<<"Expected should be 50";
	net.del_trainning_suite();
}

TEST(NETFEEDFTestSuite,TRAINNING_TC_0_NUMBERS) {
	net_feedf net(0,49,0,0,4);
	net.create();
	net.init_net();
	net.set_nr_it(1000);
	net.set_temp(1);
	net.setEta(0.35);
	net.setDesiredMse(0.014);
	net.setMomentum(0.9);
	net.initweights(-0.5, 0.2);
	net.load_trainning_suite_text_file("./numbers_until_3_trainning_data.txt");
	matrixf &inputs = net.get_trainning_inp(0);
	matrixf &expected = net.get_trainning_tp(0);
	EXPECT_EQ(1,inputs.d0())<<"dim0 should 1";
	EXPECT_EQ(1,inputs.d1())<<"dim1 should 1";
	EXPECT_EQ(49,inputs.d2())<<"dim0 should 49";
	EXPECT_EQ(1,expected.d0())<<"dim0 should 1";
	EXPECT_EQ(1,expected.d1())<<"dim1 should 1";
	EXPECT_EQ(4,expected.d2())<<"dim0 should 4";
	inputs = net.get_trainning_inp(1);
	expected = net.get_trainning_tp(1);
	EXPECT_EQ(1,inputs.d0())<<"dim0 should 1";
	EXPECT_EQ(1,inputs.d1())<<"dim1 should 1";
	EXPECT_EQ(49,inputs.d2())<<"dim0 should 49";
	EXPECT_EQ(1,expected.d0())<<"dim0 should 1";
	EXPECT_EQ(1,expected.d1())<<"dim1 should 1";
	EXPECT_EQ(4,expected.d2())<<"dim0 should 4";
	net.train();
	EXPECT_EQ(ON,net.trained());
	//net.load_test_suite_text_file("./numbers_until_3_trainning_data.txt");
	//EXPECT_EQ(13,net.get_nr_in_test_suite())<<"Wrong numbers in test suite";
	net.load_test_suite_text_file("./numbers_until_3_test_data.txt");
	EXPECT_EQ(6,net.get_nr_in_test_suite())<<"Wrong numbers in test suite";
	matrixf &outputs = net.get_outputs();
	for(int i = 0 ;i < net.get_nr_in_test_suite(); i++) {
		inputs = net.get_test_inp(i);
		expected = net.get_test_tp(i);
		net.init_inp(inputs);
		net.forward();
		outputs = net.get_outputs();
		EXPECT_EQ(max(expected),max(outputs))<<"at index "<<i;
		if (max(expected) != max(outputs)) {
			outputs.printData();
		}
	}
	net.del_trainning_suite();
	net.del_test_suite();
}
TEST(NETFEEDFTestSuite,TRAINNING_TC_1_NUMBERS) {
	net_feedf net(1,49,12,0,4);
	net.create();
	net.init_net();
	net.set_nr_it(100000);
	net.set_temp(1);
	net.setEta(0.35);
	net.setDesiredMse(0.012);
	net.setMomentum(0.9);
	net.initweights(-0.5, 0.2);
	net.load_trainning_suite_text_file("./numbers_until_3_trainning_data.txt");
	matrixf &inputs = net.get_trainning_inp(0);
	matrixf &expected = net.get_trainning_tp(0);
	EXPECT_EQ(1,inputs.d0())<<"dim0 should 1";
	EXPECT_EQ(1,inputs.d1())<<"dim1 should 1";
	EXPECT_EQ(49,inputs.d2())<<"dim0 should 49";
	EXPECT_EQ(1,expected.d0())<<"dim0 should 1";
	EXPECT_EQ(1,expected.d1())<<"dim1 should 1";
	EXPECT_EQ(4,expected.d2())<<"dim0 should 4";
	inputs = net.get_trainning_inp(1);
	expected = net.get_trainning_tp(1);
	EXPECT_EQ(1,inputs.d0())<<"dim0 should 1";
	EXPECT_EQ(1,inputs.d1())<<"dim1 should 1";
	EXPECT_EQ(49,inputs.d2())<<"dim0 should 49";
	EXPECT_EQ(1,expected.d0())<<"dim0 should 1";
	EXPECT_EQ(1,expected.d1())<<"dim1 should 1";
	EXPECT_EQ(4,expected.d2())<<"dim0 should 4";
	net.train();
	EXPECT_EQ(ON,net.trained());
	//net.load_test_suite_text_file("./numbers_until_3_trainning_data.txt");
	//EXPECT_EQ(13,net.get_nr_in_test_suite())<<"Wrong numbers in test suite";
	net.load_test_suite_text_file("./numbers_until_3_test_data.txt");
	EXPECT_EQ(6,net.get_nr_in_test_suite())<<"Wrong numbers in test suite";
	matrixf &outputs = net.get_outputs();
	for(int i = 0 ;i < net.get_nr_in_test_suite(); i++) {
		inputs = net.get_test_inp(i);
		expected = net.get_test_tp(i);
		net.init_inp(inputs);
		net.forward();
		outputs = net.get_outputs();
		EXPECT_EQ(max(expected),max(outputs))<<"at index "<<i;
		if (max(expected) != max(outputs)) {
			outputs.printData();
		}
	}
	net.del_trainning_suite();
	net.del_test_suite();
}
