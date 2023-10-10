/*
 * numbersAi_test.cpp
 *
 *  Created on: Apr 26, 2022
 *      Author: Gabriel Dimitriu
 */

#include <gtest/gtest.h>
#include "../include/netfeedf.h"
#include "../include/netfeedl.h"
#include "../include/netfeedft.h"
#include "../include/netfeedftp.h"

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



TEST(NUMBERSAITestSuite,TRAINNING_TC_0_NUMBERS) {
	net_feedf net(0,49,0,0,4);
	net.create();
	net.init_net();
	net.set_nr_it(10000);
	net.set_temp(1);
	net.setEta(0.35);
	net.setDesiredMse(0.014);
	net.setMomentum(0.9);
	net.initweights(-0.5, 0.2);
	net.load_trainning_suite_text_file("./numbers_until_3_trainning_data.txt");
	matrixf &inputs = net.get_trainning_inp(0);
	matrixf &expected = net.get_trainning_tp(0);
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
TEST(NUMBERSAITestSuite,TRAINNING_TC_1_NUMBERS) {
	net_feedf net(1,49,12,0,4);
	net.create();
	net.init_net();
	net.set_nr_it(10000);
	net.set_temp(1);
	net.setEta(0.35);
	net.setDesiredMse(0.012);
	net.setMomentum(0.9);
	net.initweights(-0.5, 0.2);
	net.load_trainning_suite_text_file("./numbers_until_3_trainning_data.txt");
	matrixf &inputs = net.get_trainning_inp(0);
	matrixf &expected = net.get_trainning_tp(0);
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

TEST(NUMBERSAITestSuite,TRAINNING_LC_1_NUMBERS) {
	net_feedl net(1,49,25,0,4,3,1,0,0);
	net.create();
	net.init_net();
	net.set_nr_it(10000);
	net.set_temp(1);
	net.setEta(0.35);
	net.setDesiredMse(0.012);
	net.setMomentum(0.9);
	net.initweights(-0.5, 0.2);
	net.load_trainning_suite_text_file("./numbers_until_3_trainning_data.txt");
	matrixf &inputs = net.get_trainning_inp(0);
	matrixf &expected = net.get_trainning_tp(0);
	net.train();
	EXPECT_EQ(ON,net.trained());
//	net.load_test_suite_text_file("./numbers_until_3_trainning_data.txt");
//	EXPECT_EQ(13,net.get_nr_in_test_suite())<<"Wrong numbers in test suite";
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

TEST(NUMBERSAITestSuite,TRAINNING_ET_2_NUMBERS) {
	net_feedft net(1,49,25,0,4,3,1,0,0,2,0);
	net.create();
	net.init_net();
	net.set_nr_it(100000);
	net.set_temp(1);
	net.setEta(0.35);
	net.setDesiredMse(0.014);
	net.setMomentum(0.9);
	net.initweights(-0.5, 0.2);
	net.load_trainning_suite_text_file("./numbers_until_3_trainning_data.txt");
	matrixf &inputs = net.get_trainning_inp(0);
	matrixf &expected = net.get_trainning_tp(0);
	net.train();
	EXPECT_EQ(ON,net.trained());
//	net.load_test_suite_text_file("./numbers_until_3_trainning_data.txt");
//	EXPECT_EQ(13,net.get_nr_in_test_suite())<<"Wrong numbers in test suite";
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

TEST(NUMBERSAITestSuite,TRAINNING_ET_4_NUMBERS) {
	net_feedft net(1,49,25,0,4,3,1,0,0,4,0);
	net.create();
	net.init_net();
	net.set_nr_it(100000);
	net.set_temp(1);
	net.setEta(0.35);
	net.setDesiredMse(0.014);
	net.setMomentum(0.9);
	net.initweights(-0.5, 0.2);
	net.load_trainning_suite_text_file("./numbers_until_3_trainning_data.txt");
	matrixf &inputs = net.get_trainning_inp(0);
	matrixf &expected = net.get_trainning_tp(0);
	net.train();
	EXPECT_EQ(ON,net.trained());
//	net.load_test_suite_text_file("./numbers_until_3_trainning_data.txt");
//	EXPECT_EQ(13,net.get_nr_in_test_suite())<<"Wrong numbers in test suite";
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

TEST(NUMBERSAITestSuite,TRAINNING_ETP_2_NUMBERS) {
	net_feedftp net(1,49,25,0,4,3,1,0,0,2,0);
	net.create();
	net.init_net();
	net.set_nr_it(100000);
	net.set_temp(1);
	net.setEta(0.35);
	net.setDesiredMse(0.014);
	net.setMomentum(0.9);
	net.initweights(-0.5, 0.2);
	net.load_trainning_suite_text_file("./numbers_until_3_trainning_data.txt");
	matrixf &inputs = net.get_trainning_inp(0);
	matrixf &expected = net.get_trainning_tp(0);
	net.train();
	EXPECT_EQ(ON,net.trained());
//	net.load_test_suite_text_file("./numbers_until_3_trainning_data.txt");
//	EXPECT_EQ(13,net.get_nr_in_test_suite())<<"Wrong numbers in test suite";
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
