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
}

net_feedf::net_feedf(int nr_hidd,int L,int M0,int M1 ,int N) : net_feedf() {
	this -> nr_hidd = nr_hidd;
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
float net_feedf::to(float value,float threshold) {
	float exponentiala = 1.0 + exp(-threshold*value);
	return 1.0/exponentiala;
}

void net_feedf::forward(void) {
	int i,j;
	float val;
	switch (nr_hidd) {
	case 0: //nici un nivel ascuns
	{
		for(i = 0; i < N; i++) {
			val = 0;
			for (j = 0; j < L; j++)
				val += wohi(0,i,j)*inp(0,0,j);
			out(0,0,i) = to(val*bohi(0,0,i),temp);
		}
		break;
	}
	case 1: //exista un singur nivel ascuns
	{
		//hid0 <--inp
		for (i = 0; i < M0; i++) {
			val = 0;
			for(j = 0; j < L; j++)
				val += whin(0,i,j)*inp(0,0,j);
			hid0(0,0,i) = to(val + bhin(0,0,i), temp);
		}
		//out <-- hid0
		for (i = 0; i< N; i++) {
			val = 0;
			for (j = 0; j < M0; j++)
				val += wohi(0,i,j)*hid0(0,0,j);
			out(0,0,i) = to(val+bohi(0,0,i),temp);
		}
		break;
	}
	case 2: //2 nivele ascunse
	{
		//hid0 <-- inp
		for (i = 0; i < M0; i++) {
			val = 0;
			for (j = 0; j < L ; j++)
				val += whin(0,i,j)*inp(0,0,j);
			hid0(0,0,i) = to(val + bhin(0,0,i), temp);
		}
		//hid1 <-- hid0
		for (i = 0; i < M1; i++) {
			val = 0;
			for (j = 0; j < M0; j++)
				val += wh10(0,i,j)*hid0(0,0,j);
			hid1(0,0,i) = to(val + bh10(0,0,i), temp);
		}
		//out <-- hid1
		for(i = 0; i < N; i++) {
			val = 0;
			for(j = 0; j < M1; j++)
				val += wohi(0,i,j)*hid1(0,0,j);
			out(0,0,i) = to(val + bohi(0,0,i), temp);
		}
		break;
	}
	default:
		fprintf(stderr,"Nr. gresit de nivele ascunse ");
	}
}

void net_feedf::backward() {
	int i,j;
	matrixf deltaohi(1,1,N);
	matrixf oldwohi;
	oldwohi = wohi;

	float deltahi,netdelta,deltaw,deltab;

	switch (nr_hidd) {
	case 0: //nici un nivel ascuns
	{
		float delta;
		for (i = 0; i < N; i++) {
			delta = tp(0,0,i) - out(0,0,i);
			delta *= out(0,0,i)*(1-out(0,0,i));
			for(j = 0; j < L; j++) {
				deltaw = eta * delta * inp(0,0,j);
				wohi(0,i,j) +=  deltaw + mom * dwohi(0,i,j);
				dwohi(0,i,j) = deltaw;
			}
			deltab = eta * delta;
			bohi(0,0,i) += deltab + mom * dbohi(0,0,i);
			dbohi(0,0,i) = deltab;
		}
		break;
	}
	case 1: // un singur nivel ascuns
	{
		//out <-- hid0
		for( i = 0; i < N; i++) {
			deltaohi(0,0,i) = tp(0,0,i) - out(0,0,i);
			deltaohi(0,0,i) *= out(0,0,i) * (1 - out(0,0,i));
			for (j = 0; j < M0; j++) {
				deltaw = eta * deltaohi(0,0,i) * hid0(0,0,j);
				wohi(0,i,j) += deltaw + mom * dwohi(0,i,j);
				dwohi(0,i,j) = deltaw;
			}
			deltab = eta * deltaohi(0,0,i);
			bohi(0,0,i) += deltab + mom*dbohi(0,0,i);
			dbohi(0,0,i) = deltab;
		}
		//hid0 <-- inp
		for (i = 0 ; i < M0; i++) {
			netdelta = 0;
			for (j = 0; j < N; j++)
				netdelta += deltaohi(0,0,j) * oldwohi(0,j,i);
			deltahi = netdelta * hid0(0,0,i)*(1 - hid0(0,0,i));
			for (j = 0 ; j < L; j++) {
				deltaw = eta*deltahi * inp(0,0,j);
				whin(0,i,j) += deltaw + mom * dwhin(0,i,j);
				dwhin(0,i,j) = deltaw;
			}
			deltab = eta * deltahi;
			bhin(0,0,i) += deltab + mom *dbhin(0,0,i);
			dbhin(0,0,i) = deltab;
		}
		break;
	}
	case 2: //2 nivele ascunse
	{
		matrixf delta10(1,1,M1);
		matrixf oldwh10;
		oldwh10 = wh10;
		//out <-- hid1
		for (i = 0; i < N; i++) {
			deltaohi(0,0,i) = tp(0,0,i) - out(0,0,i);
			deltaohi(0,0,i) *= out(0,0,i)*(1 - out(0,0,i));
			for(j = 0 ; j < M1; j++) {
				deltaw = eta * deltaohi(0,0,i)*hid1(0,0,j);
				wohi(0,i,j) += deltaw + mom * dwohi(0,i,j);
				dwohi(0,i,j) = deltaw;
			}
			deltab = eta * deltaohi(0,0,i);
			bohi(0,0,i) += deltab + mom *dbohi(0,0,i);
			dbohi(0,0,i) = deltab;
		}
		//hid1 <-- hid0
		for(i = 0; i < M1; i++) {
			netdelta = 0;
			for (j = 0; j < N; j++)
				netdelta += deltaohi(0,0,j)*oldwohi(0,j,i);
			delta10(0,0,i) = netdelta * hid1(0,0,i)*(1-hid1(0,0,i));
			for(j = 0 ; j < M0; j++) {
				deltaw = eta * delta10(0,0,i)*hid0(0,0,j);
				wh10(0,i,j) += deltaw + mom * dwh10(0,i,j);
				dwh10(0,i,j) = deltaw;
			}
			deltab = eta * delta10(0,0,i);
			bh10(0,0,i) += deltab + mom * dbh10(0,0,i);
			dbh10(0,0,i) = deltab;
		}
		//hid0 <-- inp
		for(i = 0; i < M0; i++) {
			netdelta = 0;
			for (j = 0; j < M1; j++)
				netdelta += delta10(0,0,j) * oldwh10(0,j,i);
			deltahi = netdelta * hid0(0,0,i) * (1 - hid0(0,0,i));
			for (j = 0 ; j < L ; j++) {
				deltaw = eta * deltahi * inp(0,0,j);
				whin(0,i,j) += deltaw + mom * dwhin(0,i,j);
				dwhin(0,i,j) = deltaw;
			}
			deltab = eta * deltahi;
			bhin(0,0,i) += deltab + mom * dbhin(0,0,i);
			dbhin(0,0,i) = deltab;
		}
		break;
	}
	default:
		fprintf(stderr,"Nr. gresit de nivele ascunse ");
	}
}

void net_feedf::test() {

}

void net_feedf::train() {

}

int net_feedf::max() {
	float maxim = out(0,0,1);
	for (int i = 0; i < N; i++) {
		if (maxim < out(0,0,i))
			maxim = out(0,0,i);
	}
	return maxim;
}

matrixf& net_feedf::get_outputs() {
	return out;
}

void net_feedf::init_net() {
	inp.init(1,1,L);
	out.init(1,1,N);
	tp.init(1,1,N);
	switch(nr_hidd) {
	case 0:
		wohi.init(1,N,L);
		dwohi.init(1,N,L);
		break;
	case 1:
		hid0.init(1,1,M0);
		whin.init(1,M0,L);
		dwhin.init(1,M0,L);
		bhin.init(1,1,M0);
		dbhin.init(1,1,M0);
		wohi.init(1,N,M0);
		dwohi.init(1,N,M0);
		break;
	case 2:
		hid0.init(1,1,M0);
		hid1.init(1,1,M1);
		whin.init(1,M0,L);
		dwhin.init(1,M0,L);
		bhin.init(1,1,M0);
		dbhin.init(1,1,M0);
		wh10.init(1,M1,M0);
		dwh10.init(1,M1,M0);
		bh10.init(1,1,M1);
		dbh10.init(1,1,M1);
		wohi.init(1,N,M1);
		dwohi.init(1,N,M1);
		break;
	}
	bohi.init(1,1,N);
	dbohi.init(1,1,N);
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
}

//SAVE-LOADS
void net_feedf::save() {

}

void net_feedf::load_inf() {

}

void net_feedf::put_perf() {

}

//WEIGHTS of the network
void net_feedf::initweights() {

}

void net_feedf::load_weights() {

}

//INITALIZATIONS
void net_feedf::inp_noise() {

}

void net_feedf::init_tp(int size) {

}

void net_feedf::init_inp(matrix& inputs) {
	inp = inputs;
}

void net_feedf::init_inpbin(int size,float *vector) {

}
