/*
 * netfeedl.cpp
 *
 *  Created on: Apr 5, 2022
 *      Author: Gabriel Dimitriu
 *  @Copyright
 *  Code for net taken from SSN described in
 *  Retele neuronale artficiale by Gavril Toderean, Mircea Costeiu, Mircea Giurgiu
 *  Editura Albastra, Cluj-Napoca 1995
 */

#include "netfeedl.h"
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

net_feedl::net_feedl() : net_feedf() {
	l = 0;
	nr_latinp = 0;
	disin = 0;
	h0 = 0;
	nr_lath0 =0;
	dis0 =0;
	h1 = 0;
	typenet = '?';
}

net_feedl::net_feedl(int nr_hidd, int L, int M0, int M1, int N, int nr_latinp, int disin, int nr_lath0, int dis0)
	:net_feedf(nr_hidd,L,M0,M1,N) {

	this->nr_latinp = nr_latinp;
	this->disin = disin;
	this->nr_lath0 = nr_lath0;
	this->dis0 = dis0;
	h0 = sqrt(M0);
	h1 = sqrt(M1);
	l = nr_latinp;
	typenet = '?';
}

net_feedl::~net_feedl() {
	// TODO Auto-generated destructor stub
}


void net_feedl::forward() {
	int i,j,k;
	float val;
	switch(nr_hidd) {
	case 1: //1 nivel ascuns
	{
		//hid0 <-- inp
		for (i = 0 ; i < h0; i++)
			for (j = 0 ; j < h0; j++) {
				val = 0;
				for (k = 0; k < nr_latinp*nr_latinp; k++)
					val += whin(0,i*h0*+k,k) * inp(0,(i*disin+k/nr_latinp)%l,(j*disin+k % nr_latinp)%l);
				hid0(0,i,j) = to(val+bhin(0,0,i*h0+j),temp);
			}
		//out <-- hid0
		for (i = 0; i < N; i++) {
			val = 0;
			for (j = 0; j < h0; j++)
				for (k = 0; j < h0; j++)
					val += wohi(0,0,j*h0+k)*hid0(0,j,k);
			out(0,0,i) = to(val + bohi(0,0,i),temp);
		}
		break;
	}
	case 2: //2 nivele ascunse
	{
		//hid0 <-- inp
		for (i = 0; i < h0; i++)
			for (j = 0; j < h0; j++) {
				val = 0;
				for (k = 0; k < nr_latinp*nr_latinp;k++)
					val += whin(0,i*h0+j,k)*inp(0,(i*disin+k/nr_latinp)%l,(j*disin+k%nr_latinp)%l);
				hid0(0,i,j) = to(val+bhin(0,0,i*h0+j),temp);
			}
		//hid1 <-- hid0
		for (i = 0; i < h1; i++)
			for (j = 0; j < h1; j++) {
				val = 0;
				for (k = 0; k < nr_lath0*nr_lath0; k++)
					val += wh10(0,i*h1+j,k)*hid0(0,(i*dis0+k/nr_lath0)%h0,(j*dis0+k%nr_lath0)%h0);
				hid1(0,i,j) = to(val+bh10(0,0,i*h1+j),temp);
			}
		//out <-- hid1
		for (i = 0; i < N; i++) {
			val = 0;
			for (j = 0; j < h1; j++)
				for (k = 0; k < h1; k++)
					val += wohi(0,i,j*h1+k)*hid1(0,j,k);
			out(0,0,i) = to(val+bohi(0,0,i),temp);
		}
		break;
	}
	default:
		fprintf(stderr,"Illegal hidden layers number !!");
	}
}

void net_feedl::backward() {
	int i,j,k,r,q;
	matrixf deltaohi(1,1,N);
	matrixf oldwohi;
	oldwohi = wohi;
	float deltahi, netdelta;
	float deltaw, deltab;
	switch(nr_hidd) {
	case 1: //1 nivel ascuns
	{
		//out <-- hid0
		for (i = 0; i < N; i++) {
			deltaohi(0,0,i) = tp(0,0,i) - out(0,0,i);
			deltaohi(0,0,i) *= out(0,0,i)*(1 -out(0,0,i));
			for (j = 0; j < M0; j++) {
				deltaw = eta * deltaohi(0,0,i) * hid0(0,j/h0, j%h0);
				wohi(0,i,j) += deltaw + mom * dwohi(0,i,k);
				dwohi(0,i,j) = deltaw;
			}
			deltab = eta  * deltaohi(0,0,i);
			bohi(0,0,i) += deltab + mom*dbohi(0,0,i);
		}
		//hid0 <-- inp
		for (i = 0; i < h0; i++)
			for (j = 0; j < h0; j++) {
				netdelta = 0;
				for (k = 0; k < N ; k++)
					netdelta += deltaohi(0,0,k)*oldwohi(0,k,i*h0+j);
				deltahi = netdelta * hid0(0,i,j)*(1-hid0(0,i,j));
				for (k = 0; k < nr_latinp*nr_latinp; k++) {
					deltaw = eta *deltahi *inp(0,(i*disin + k/nr_latinp)%l,(j*disin+k%nr_latinp)%l);
					whin(0,i*h0+j,k) += deltaw + mom*dwhin(0,i*h0+j,k);
					dwhin(0,i*h0+j,k) = deltaw;
				}
				deltab = eta * deltahi;
				bhin(0,0,i*h0+j) += deltab + mom * dbhin(0,0,i*h0+j);
				dbhin(0,0,i*h0+j) = deltab;
			}
		break;
	}
	case 2: //2 nivele ascunse
	{
		matrixf delta10(1,1,M1),oldwh10;
		oldwh10 = wh10;
		int xx, yy;
		//out <-- hid1
		for (i = 0; i < N; i++) {
			deltaohi(0,0,i) = tp(0,0,i) - out(0,0,i);
			deltaohi(0,0,i) *= out(0,0,i)*(1-out(0,0,i));
			for (j = 0; j < M1; j++) {
				deltaw = eta * deltaohi(0,0,i)*hid1(0,j/h1,j%h1);
				wohi(0,i,j) += deltaw + mom * dwohi(0,i,j);
				dwohi(0,i,j) = deltaw;
			}
			deltab = eta * deltaohi(0,0,i);
			bohi(0,0,i) += deltab + mom * dbohi(0,0,i);
			dbohi(0,0,i) = deltab;
		}
		//hid1 <-- hid0
		for (i = 0; i < h1; i++)
			for (j = 0; j < h1; j++) {
				netdelta = 0;
				for (k = 0; k < N; k++)
					netdelta += deltaohi(0,0,k) * oldwohi(0,k,i*h1+j);
				delta10(0,0,i*h1+j) = netdelta * hid1(0,i,j)*(1-hid1(0,i,j));
				for (k = 0; k < nr_lath0*nr_lath0; k++) {
					deltaw = eta * delta10(0,0,i*h1+j)* hid0(0,(i*dis0+k/nr_lath0)%h0,(j*dis0+k%nr_lath0)%h0);
					wh10(0,i*h1+j,k) += deltaw + mom * dwh10(0,i*h1+j,k);
					dwh10(0,i*h1+j,k) = deltaw;
				}
				deltab = eta * delta10(0,0,i*h1+j);
				bh10(0,0,i*h1+j) += deltab + mom*dbh10(0,0,i*h1+j);
				dbh10(0,0,i*h1+j) = deltab;
			}
		//hid0 <-- inp
		for (i = 0; i < h0; i++)
			for (j = 0; j < h0; j++) {
				netdelta = 0;
				for (k = 0; k < M1; k++)
					for (q = 0; q < nr_lath0*nr_lath0; q++) {
						xx = (k/h1*dis0+q/nr_lath0)%h0;
						yy = (k%h1*dis0+q%nr_lath0)%h0;
						if (!(xx == i && yy == j))
							continue;
						netdelta += delta10(0,0,k)*oldwh10(0,k,q);
					}
				netdelta *= hid0(0,i,j)*(1-hid0(0,i,j));
				//ajustare ponderi
				for (r = 0; r < nr_latinp*nr_latinp; r++) {
					deltaw = eta * netdelta * inp(0,(i*disin+r/nr_latinp)%l,(j*disin+r%nr_latinp)%l);
					whin(0,i*h0+j,r) += deltaw + mom * dwhin(0,i*h0+j,r);
					dwhin(0,i*h0+j,r) = deltaw;
				}
				deltab = eta * netdelta;
				bhin(0,0,i*h0+j) += deltab + mom*dbhin(0,0,i*h0+j);
				bhin(0,0,i*h0+j) = deltab;
			}
		break;
	}
	default:
		fprintf(stderr,"Illegal hidden layers number !!");
	}
}

//SAVE-LOADS
int net_feedl::save(const char *path) {
	int dFile = open(path, O_CREAT | O_RDWR);
	chmod(path, S_IRUSR | S_IWUSR);
	if (dFile <= 0) {
		perror("Could not open file for write");
		return -1;
	}
	write(dFile, &typenet, sizeof(char));
	net_feedf::saveInternal_inf(dFile);
	saveInternal_inf(dFile);
	close(dFile);
	return 0;
}

void net_feedl::saveInternal_inf(int dFile) {
	write(dFile,&l,sizeof(int));
	write(dFile,&nr_latinp,sizeof(int));
	write(dFile,&disin,sizeof(int));
	write(dFile,&h0,sizeof(int));
	write(dFile,&nr_lath0,sizeof(int));
	write(dFile,&dis0,sizeof(int));
	write(dFile,&h1,sizeof(int));
}

int net_feedl::load_inf(const char* path) {
	int dFile = open(path,O_RDONLY);
	if (dFile <= 0) {
		perror("Could not open file for read");
		return -1;
	}
	read(dFile,&typenet,sizeof(char));
	net_feedf::loadInternal_inf(dFile);
	close(dFile);
	return 0;
}

void net_feedl::loadInternal_inf(int dFile) {
	read(dFile,&l,sizeof(int));
	read(dFile,&nr_latinp,sizeof(int));
	read(dFile,&disin,sizeof(int));
	read(dFile,&h0,sizeof(int));
	read(dFile,&nr_lath0,sizeof(int));
	read(dFile,&dis0,sizeof(int));
	read(dFile,&h1,sizeof(int));
}

