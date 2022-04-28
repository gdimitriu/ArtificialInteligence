/*
 * netfeedf.cpp
 *
 *  Created on: Apr 4, 2022
 *      Author: Gabriel Dimitriu
 *  @Copyright
 *  Code for net taken from SNNS described in
 *  Retele neuronale artficiale by Gavril Toderean, Mircea Costeiu, Mircea Giurgiu
 *  Editura Albastra, Cluj-Napoca 1995
 */

#include "netfeedf.h"
#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <math.h>

extern int errno;
using namespace std;

net_feedf::net_feedf() {
	nr_hidd = 0;
	CREATE = OFF;
	TRAINED = OFF;
	tab_test = 0;
	nr_it = 0;
	typenet = '%';
	LastMse = 0.0;
	LastIndEta = 0.0;
	LastS = 0.0;
	EtaVar = 0;
	eta = 0.0;
	N = 0;
	M0 = 0;
	M1 = 0;
	L = 0;
	LOADED = OFF;
	nr_in_suite_test = 0;
	nr_in_suite_trainning = 0;
	DesiredMse = 0.0;
}

net_feedf::net_feedf(int nr_hidd, int L, int M0, int M1, int N) :
		net_feedf() {
	this->nr_hidd = nr_hidd;
	this->L = L;
	this->M0 = M0;
	this->M1 = M1;
	this->N = N;
}

net_feedf::~net_feedf() {
	free_net();
	if (tab_test != 0)
		delete tab_test;
}

void net_feedf::set_temp(float value) {
	temp = value;
}

float net_feedf::to(float value, float threshold) {
	float exponentiala = 1.0 + exp(-threshold * value);
	return 1.0 / exponentiala;
}

void net_feedf::forward(void) {
	int i, j;
	float val;
	switch (nr_hidd) {
	case 0: //nici un nivel ascuns
	{
		for (i = 0; i < N; i++) {
			val = 0;
			for (j = 0; j < L; j++)
				val += wohi(0, i, j) * inp(0, 0, j);
			out(0, 0, i) = to(val + bohi(0, 0, i), temp);
		}
		break;
	}
	case 1: //exista un singur nivel ascuns
	{
		//hid0 <--inp
		for (i = 0; i < M0; i++) {
			val = 0;
			for (j = 0; j < L; j++)
				val += whin(0, i, j) * inp(0, 0, j);
			hid0(0, 0, i) = to(val + bhin(0, 0, i), temp);
		}
		//out <-- hid0
		for (i = 0; i < N; i++) {
			val = 0;
			for (j = 0; j < M0; j++)
				val += wohi(0, i, j) * hid0(0, 0, j);
			out(0, 0, i) = to(val + bohi(0, 0, i), temp);
		}
		break;
	}
	case 2: //2 nivele ascunse
	{
		//hid0 <-- inp
		for (i = 0; i < M0; i++) {
			val = 0;
			for (j = 0; j < L; j++)
				val += whin(0, i, j) * inp(0, 0, j);
			hid0(0, 0, i) = to(val + bhin(0, 0, i), temp);
		}
		//hid1 <-- hid0
		for (i = 0; i < M1; i++) {
			val = 0;
			for (j = 0; j < M0; j++)
				val += wh10(0, i, j) * hid0(0, 0, j);
			hid1(0, 0, i) = to(val + bh10(0, 0, i), temp);
		}
		//out <-- hid1
		for (i = 0; i < N; i++) {
			val = 0;
			for (j = 0; j < M1; j++)
				val += wohi(0, i, j) * hid1(0, 0, j);
			out(0, 0, i) = to(val + bohi(0, 0, i), temp);
		}
		break;
	}
	default:
		fprintf(stderr, "Illegal hidden layers number !!");
	}
}

void net_feedf::backward() {
	int i, j;
	matrixf deltaohi(1, 1, N);
	matrixf oldwohi;
	oldwohi = wohi;

	float deltahi, netdelta, deltaw, deltab;

	switch (nr_hidd) {
	case 0: //nici un nivel ascuns
	{
		float delta;
		for (i = 0; i < N; i++) {
			delta = tp(0, 0, i) - out(0, 0, i);
			delta *= out(0, 0, i) * (1 - out(0, 0, i));
			for (j = 0; j < L; j++) {
				deltaw = eta * delta * inp(0, 0, j);
				wohi(0, i, j) += deltaw + mom * dwohi(0, i, j);
				dwohi(0, i, j) = deltaw;
			}
			deltab = eta * delta;
			bohi(0, 0, i) += deltab + mom * dbohi(0, 0, i);
			dbohi(0, 0, i) = deltab;
		}
		break;
	}
	case 1: // un singur nivel ascuns
	{
		//out <-- hid0
		for (i = 0; i < N; i++) {
			deltaohi(0, 0, i) = tp(0, 0, i) - out(0, 0, i);
			deltaohi(0, 0, i) *= out(0, 0, i) * (1 - out(0, 0, i));
			for (j = 0; j < M0; j++) {
				deltaw = eta * deltaohi(0, 0, i) * hid0(0, 0, j);
				wohi(0, i, j) += deltaw + mom * dwohi(0, i, j);
				dwohi(0, i, j) = deltaw;
			}
			deltab = eta * deltaohi(0, 0, i);
			bohi(0, 0, i) += deltab + mom * dbohi(0, 0, i);
			dbohi(0, 0, i) = deltab;
		}
		//hid0 <-- inp
		for (i = 0; i < M0; i++) {
			netdelta = 0;
			for (j = 0; j < N; j++)
				netdelta += deltaohi(0, 0, j) * oldwohi(0, j, i);
			deltahi = netdelta * hid0(0, 0, i) * (1 - hid0(0, 0, i));
			for (j = 0; j < L; j++) {
				deltaw = eta * deltahi * inp(0, 0, j);
				whin(0, i, j) += deltaw + mom * dwhin(0, i, j);
				dwhin(0, i, j) = deltaw;
			}
			deltab = eta * deltahi;
			bhin(0, 0, i) += deltab + mom * dbhin(0, 0, i);
			dbhin(0, 0, i) = deltab;
		}
		break;
	}
	case 2: //2 nivele ascunse
	{
		matrixf delta10(1, 1, M1);
		matrixf oldwh10;
		oldwh10 = wh10;
		//out <-- hid1
		for (i = 0; i < N; i++) {
			deltaohi(0, 0, i) = tp(0, 0, i) - out(0, 0, i);
			deltaohi(0, 0, i) *= out(0, 0, i) * (1 - out(0, 0, i));
			for (j = 0; j < M1; j++) {
				deltaw = eta * deltaohi(0, 0, i) * hid1(0, 0, j);
				wohi(0, i, j) += deltaw + mom * dwohi(0, i, j);
				dwohi(0, i, j) = deltaw;
			}
			deltab = eta * deltaohi(0, 0, i);
			bohi(0, 0, i) += deltab + mom * dbohi(0, 0, i);
			dbohi(0, 0, i) = deltab;
		}
		//hid1 <-- hid0
		for (i = 0; i < M1; i++) {
			netdelta = 0;
			for (j = 0; j < N; j++)
				netdelta += deltaohi(0, 0, j) * oldwohi(0, j, i);
			delta10(0, 0, i) = netdelta * hid1(0, 0, i) * (1 - hid1(0, 0, i));
			for (j = 0; j < M0; j++) {
				deltaw = eta * delta10(0, 0, i) * hid0(0, 0, j);
				wh10(0, i, j) += deltaw + mom * dwh10(0, i, j);
				dwh10(0, i, j) = deltaw;
			}
			deltab = eta * delta10(0, 0, i);
			bh10(0, 0, i) += deltab + mom * dbh10(0, 0, i);
			dbh10(0, 0, i) = deltab;
		}
		//hid0 <-- inp
		for (i = 0; i < M0; i++) {
			netdelta = 0;
			for (j = 0; j < M1; j++)
				netdelta += delta10(0, 0, j) * oldwh10(0, j, i);
			deltahi = netdelta * hid0(0, 0, i) * (1 - hid0(0, 0, i));
			for (j = 0; j < L; j++) {
				deltaw = eta * deltahi * inp(0, 0, j);
				whin(0, i, j) += deltaw + mom * dwhin(0, i, j);
				dwhin(0, i, j) = deltaw;
			}
			deltab = eta * deltahi;
			bhin(0, 0, i) += deltab + mom * dbhin(0, 0, i);
			dbhin(0, 0, i) = deltab;
		}
		break;
	}
	default:
		fprintf(stderr, "Illegal hidden layers number !!");
	}
}

void net_feedf::test() {

}

void net_feedf::train() {
	nr_it_used = 0;
	int index_in_test = 0;
	for(;nr_it_used < nr_it; nr_it_used++) {
		for(index_in_test = 0; index_in_test < nr_in_suite_trainning; index_in_test++) {
			init_inp(trainning_inp[index_in_test]);
			tp = trainning_tp[index_in_test];
			forward();
			backward();
		}
		LastMse = 0.0;
		for(index_in_test = 0; index_in_test < nr_in_suite_trainning; index_in_test++) {
			init_inp(trainning_inp[index_in_test]);
			tp = trainning_tp[index_in_test];
			forward();
			for (int i = 0; i < N; i++) {
				LastMse += pow(out(0,0,i) - tp(0,0,i),2);
			}
		}
		if (fabs(LastMse/(nr_in_suite_trainning) - DesiredMse) < 0.0001) {
			TRAINED = ON;
			break;
		}
	}
}

int net_feedf::max() {
	int index = 0;
	float maxim = out(0, 0, 0);
	for (int i = 1; i < N; i++) {
		if (maxim < out(0, 0, i)) {
			maxim = out(0, 0, i);
			index = i;
		}
	}
	return index;
}

matrixf& net_feedf::get_outputs() {
	return out;
}

void net_feedf::init_net() {
	inp.init(1, 1, L);
	out.init(1, 1, N);
	tp.init(1, 1, N);
	switch (nr_hidd) {
	case 0:
		if (LOADED == OFF) {
			wohi.init(1,N,L);
		}
		dwohi.init(1,N,L);
		break;
	case 1:
		if (LOADED == OFF) {
			wohi.init(1, N, M0);
			hid0.init(1, 1, M0);
			bhin.init(1, 1, M0);
			whin.init(1, M0, L);
		}
		dwohi.init(1, N, M0);
		dwhin.init(1, M0, L);
		dbhin.init(1, 1, M0);
		break;
	case 2:
		if (LOADED == OFF) {
			wohi.init(1, N, M1);
			hid0.init(1, 1, M0);
			bhin.init(1, 1, M0);
			hid1.init(1, 1, M1);
			wh10.init(1, M1, M0);
			whin.init(1, M0, L);
		}
		dwhin.init(1, M0, L);
		dbhin.init(1, 1, M0);
		dwh10.init(1, M1, M0);
		bh10.init(1, 1, M1);
		dbh10.init(1, 1, M1);
		dwohi.init(1, N, M1);
		break;
	}
	if (LOADED == OFF) {
		bohi.init(1, 1, N);
	}
	dbohi.init(1, 1, N);
}

int net_feedf::create() {

	return 0;
}

void net_feedf::free_net() {
	inp.free_mat();
	out.free_mat();
	tp.free_mat();
	hid0.free_mat();
	hid1.free_mat();
	whin.free_mat();
	dwhin.free_mat();
	bhin.free_mat();
	dbhin.free_mat();
	wh10.free_mat();
	dwh10.free_mat();
	bh10.free_mat();
	dbh10.free_mat();
	wohi.free_mat();
	dwohi.free_mat();
	bohi.free_mat();
	dbohi.free_mat();
	LOADED = OFF;
}

//SAVE-LOADS
int net_feedf::save(const char *path) {
	int dFile = open(path, O_CREAT | O_RDWR);
	chmod(path, S_IRUSR | S_IWUSR);
	if (dFile <= 0) {
		perror("Could not open file for write");
		return -1;
	}
	write(dFile, &typenet, sizeof(char));
	saveInternal_inf(dFile);
	close(dFile);
	return 0;
}

void net_feedf::saveInternal_inf(int dFile) {
	int size = strlen(name) + 1;
	write(dFile, &size, sizeof(int));
	write(dFile, name, sizeof(char) * size);
	write(dFile, &nr_hidd, sizeof(int));
	write(dFile, &L, sizeof(int));
	write(dFile, &M0, sizeof(int));
	write(dFile, &M1, sizeof(int));
	write(dFile, &N, sizeof(int));
	switch (nr_hidd) {
	case 0:
		wohi.save(dFile);
		bohi.save(dFile);
		break;
	case 1:
		wohi.save(dFile);
		bohi.save(dFile);
		hid0.save(dFile);
		bhin.save(dFile);
		whin.save(dFile);
		break;
	case 2:
		wohi.save(dFile);
		bohi.save(dFile);
		hid0.save(dFile);
		bhin.save(dFile);
		hid1.save(dFile);
		wh10.save(dFile);
		whin.save(dFile);
		break;
	}
}

void net_feedf::loadInternal_inf(int dFile) {
	int size;
	read(dFile, &size, sizeof(int));
	read(dFile, name, sizeof(char) * size);
	read(dFile, &nr_hidd, sizeof(int));
	read(dFile, &L, sizeof(int));
	read(dFile, &M0, sizeof(int));
	read(dFile, &M1, sizeof(int));
	read(dFile, &N, sizeof(int));
	switch (nr_hidd) {
	case 0:
		wohi.load(dFile);
		bohi.load(dFile);
		break;
	case 1:
		wohi.load(dFile);
		bohi.load(dFile);
		hid0.load(dFile);
		bhin.load(dFile);
		whin.load(dFile);
		break;
	case 2:
		wohi.load(dFile);
		bohi.load(dFile);
		hid0.load(dFile);
		bhin.load(dFile);
		hid1.load(dFile);
		wh10.load(dFile);
		whin.load(dFile);
		break;
	}
	LOADED = ON;
}

int net_feedf::load_inf(const char* path) {
	int dFile = open(path,O_RDONLY);
	if (dFile <= 0) {
		perror("Could not open file for read");
		return -1;
	}
	read(dFile,&typenet,sizeof(char));
	loadInternal_inf(dFile);
	close(dFile);
	return 0;
}

void net_feedf::put_perf() {

}

//WEIGHTS of the network
void net_feedf::initweights(float offset, float multiplicator) {
	srand(time(NULL));
	switch (nr_hidd) {
	case 0:
		wohi.init_rand(offset,multiplicator);
		bohi.init_rand(offset,multiplicator);
		break;
	case 1:
		wohi.init_rand(offset,multiplicator);
		bohi.init_rand(offset,multiplicator);
		hid0.init_rand(offset,multiplicator);
		bhin.init_rand(offset,multiplicator);
		whin.init_rand(offset,multiplicator);
		break;
	case 2:
		wohi.init_rand(offset,multiplicator);
		bohi.init_rand(offset,multiplicator);
		hid0.init_rand(offset,multiplicator);
		bhin.init_rand(offset,multiplicator);
		hid1.init_rand(offset,multiplicator);
		wh10.init_rand(offset,multiplicator);
		whin.init_rand(offset,multiplicator);
		break;
	}
}

//INITALIZATIONS
void net_feedf::inp_noise() {

}

void net_feedf::init_inp(matrix &inputs) {
	inp = inputs;
}

void net_feedf::init_inp(matrixf &inputs) {
	inp = inputs;
}

void net_feedf::load_trainning_text_File(char *path) {
	ifstream file(path);
	inp.load_text(file);
	tp.load_text(file);
	file.close();
}

matrixf& net_feedf::get_inp() {
	return inp;
}

matrixf& net_feedf::get_tp() {
	return tp;
}

matrixf& net_feedf::get_trainning_inp(int index) {
	return trainning_inp[index];
}

matrixf& net_feedf::get_trainning_tp(int index) {
	return trainning_tp[index];
}

matrixf& net_feedf::get_test_inp(int index) {
	return test_inp[index];
}

matrixf& net_feedf::get_test_tp(int index) {
	return test_tp[index];
}

void net_feedf::load_trainning_suite_text_file(char *path) {
	ifstream file(path);
	file >> nr_in_suite_trainning;
	trainning_inp = new matrixf[nr_in_suite_trainning]();
	trainning_tp = new matrixf[nr_in_suite_trainning]();
	for (int i = 0; i < nr_in_suite_trainning; i++) {
		trainning_inp[i].load_text(file);
		trainning_tp[i].load_text(file);
	}
	file.close();
}

void net_feedf::del_trainning_suite() {
	delete[] trainning_inp;
	delete[] trainning_tp;
	nr_in_suite_trainning = 0;
}

void net_feedf::load_test_suite_text_file(char *path) {
	ifstream file(path);
	file >> nr_in_suite_test;
	test_inp = new matrixf[nr_in_suite_test]();
	test_tp = new matrixf[nr_in_suite_test]();
	for (int i = 0; i < nr_in_suite_test; i++) {
		test_inp[i].load_text(file);
		test_tp[i].load_text(file);
	}
	file.close();
}

void net_feedf::del_test_suite() {
	delete[] test_inp;
	delete[] test_tp;
	nr_in_suite_test = 0;
}
