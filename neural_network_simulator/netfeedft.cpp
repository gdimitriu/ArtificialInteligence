/*
 * netfeedft.cpp
 *
 *  Created on: Apr 5, 2022
 *      Author: Gabriel Dimitriu
 *  @Copyright
 *  Code for net taken from SSN described in
 *  Retele neuronale artficiale by Gavril Toderean, Mircea Costeiu, Mircea Giurgiu
 *  Editura Albastra, Cluj-Napoca 1995
 */

#include "netfeedft.h"
#include <unistd.h>
#include <stdlib.h>

net_feedft::net_feedft() : net_feedl(){
	nr0 = 0;
	nr1 = 0;
	typenet = '#';
}

net_feedft::net_feedft(int nr_hidd, int L, int M0, int M1, int N, int nr_latinp, int disin, int nr_lath0,
		int dis0, int nr0, int nr1)
	:net_feedl(nr_hidd,L,M0,M1,N,nr_latinp,disin, nr_lath0, dis0) {
	this->nr0 = nr0;
	this->nr1 = nr1;
	typenet = '#';
}

void net_feedft::forward() {
	int i,j,k,x,y;
	float val;
	switch(nr_hidd) {
	case 1: //1 nivel ascuns
	{
		//hid0 <- inp
		for (x = 0; x < nr0; x++)
			for (i = 0; i < h0; i++)
				for (j = 0; j < h0; j++) {
					val = 0.0;
					for (k = 0; k < nr_latinp * nr_latinp; k++)
						val += whin(0,x,k) * inp(0, (i * disin + k/nr_latinp) % l, (j * disin + k % nr_latinp) % l);
					hid0(x,i,j) = to(val + bhin(0, x, i * h0 + j), temp);
				}
		//out <- hid0 always total connected
		for (i = 0; i < N; i++) {
			val = 0.0;
			for (x = 0; x < nr0; x++)
				for (j = 0; j < h0; j++)
					for (k = 0; k < h0; k++)
						val += wohi(x, i, j * h0 + k)*hid0(x, j, k);
			out(0,0,i) = to(val + bohi(0, 0, i), temp);
		}
		break;
	}
	case 2: //2 nivele ascunse
	{
		//hid0 <- inp
		for (x = 0; x < nr0; x++)
			for (i = 0; i < h0; i++)
				for (j = 0; j < h0; j++) {
					val = 0.0;
					for (k = 0; k < nr_latinp * nr_latinp; k++)
						val += whin(0, x, k)* inp(0, (i * disin + k/nr_latinp) % l, (j * disin + k%nr_latinp) % l);
					hid0(x,i,j) = to(val + bhin(0, x, i * h0 + j), temp);
				}
		//hid1 <- hid0
		for (x = 0; x < nr1; x++)
			for (i = 0; i < h1; i++)
				for (j = 0; j < h1; j++) {
					val = 0.0;
					if (nr1 == 1) {
						for (k = 0; k < nr_lath0 * nr_lath0; k++)
							for ( y = 0; y < nr0; y++)
								val += wh10(y, i * h1 + j, k) * hid0(y, (i* dis0 + k/nr_lath0) % h0, (j * dis0 + k%nr_lath0) % h0);
					} else {
						for (k = 0; k < nr_lath0 * nr_lath0; k++)
							for (y = 0; y < nr0; y++)
								val += wh10(x,y,k) * hid0(y, (i * dis0 + k/nr_lath0) % h0, (j * dis0 + k% nr_lath0) % h0);
					}
					hid1(x,i,j) = to(val + bh10(0, x, i * h1 + j), temp);
				}
		//out <- hid1
		for (i = 0; i < N; i++) {
			val = 0.0;
			for (x = 0; x < nr1; x++)
				for (j = 0; j < h1; j++)
					for (k = 0; k < h1; k++)
						val += wohi(x, i, j * h1 + k) * hid1(x, j, k);
			out(0,0,i) = to(val + bohi(0,0,i), temp);
		}
		break;
	}
	default:
		fprintf(stderr,"Illegal hidden layers number !!");
	}
}

void net_feedft::backward() {
	int i,j,k,q,x;
	float netdelta, deltaw, deltab;
	matrixf deltaohi(1,1,N);
	matrixf oldwohi;
	oldwohi = wohi;

	switch (nr_hidd) {
	case 1: //1 nivel ascuns cu mai multe plane (feature maps)
	{
		matrixf delta(1,1,M0);
		//out <- hid0
		for (i = 0; i < N; i++) {
			deltaohi(0,0,i) = tp(0,0,i) - out(0,0,i);
			deltaohi(0,0,i) *= out(0,0,i) * (1 - out(0,0,i));
			for (x = 0; x < nr0; x++)
				for (j = 0; j < M0; j++) {
					deltaw = eta * deltaohi(0,0,i) * hid0(x, j/h0, j%h0);
					wohi(x,i,j) = wohi(x,i,j) + (deltaw + mom * dwohi(x,i,j));
					dwohi(x,i,j) = deltaw;
				}
			deltab = eta * deltaohi(0,0,i);
			bohi(0,0,i) = bohi(0,0,i) + (deltab + mom * dbohi(0,0,i));
			dbohi(0,0,i) = deltab;
		}
		//hid0 <- inp
		for (x = 0; x < nr0; x++)
			for (q = 0; q < nr_latinp * nr_latinp; q++) {
				deltaw = 0.0;
				for (i = 0; i < h0; i++)
					for (j = 0; j < h0; j++) {
						if (q == 0) {
							netdelta = 0.0;
							for (k = 0; k < N; k++)
								netdelta = netdelta + (deltaohi(0,0,k) * oldwohi(x, k, i * h0 + j));
							delta(0, 0, i * h0 + j) = netdelta * hid0(x,i,j) * (1 - hid0(x, i, j));
							deltab = eta * delta(0, 0, i * h0 + j);
							bhin(0, x, i * h0 + j) += deltab + mom * dbhin(0, x, i * h0 + j);
							dbhin(0, x, i * h0 + j) = deltab;
						}
						deltaw += delta(0, 0, i * h0 + j) * inp(0, (i * disin + q/nr_latinp) % l, (j *disin + q%nr_latinp) % l);
					}
				deltaw *= eta;
				whin(0,x,q) += deltaw + mom * dwhin(0, x, q);
				dwhin(0,x,q) = deltaw;
			}
		break;
	}
	case 2: //2 nivele ascunse
	{
		if (nr1 == 1)
			backward_1();
		else
			backward_2();
		break;
	}
	default:
		fprintf(stderr,"Illegal hidden layers number !!");
	}
}

void net_feedft::backward_1(void) {
	int i,j,k,x,q,r,xx,yy;
	float netdelta, deltaw, deltab;
	matrixf deltaohi(1,1,N);
	matrixf delta10(1,1,M1);
	matrixf oldwohi, oldwh10;
	oldwohi = wohi;
	oldwh10 = wh10;
	matrixf xxyy(2, M1, nr_lath0 * nr_lath0);
	matrixf netd(1, M1, nr_lath0 * nr_lath0);

	//out <- hid1
	for (i = 0; i < N; i++) {
		deltaohi(0,0,i) = tp(0,0,i) - out(0,0,i);
		deltaohi(0,0,i) *= out(0,0,i) * (1 - out(0,0,i));
		for (j = 0; j < M1; j++) {
			deltaw = eta * deltaohi(0,0,i) * hid0(0, j/h1, j%h1);
			wohi(0,i,j) += deltaw + mom * dwohi(0,i,j);
			dwohi(0,i,j) = deltaw;
		}
		deltab = eta * deltaohi(0,0,i);
		bohi(0,0,i) += deltab + mom * dbohi(0,0,i);
		dbohi(0,0,i) = deltab;
	}
	//hid1 <- hid0
	for (j = 0; j < M1; j++) {
		netdelta = 0.0;
		for (i = 0; i < N; i++)
			netdelta += deltaohi(0,0,i) * oldwohi(0,i,j);
		delta10(0,0,j) = netdelta * hid1(0, j/h1, j%h1) * (1 - hid1(0, j/h1, j%h1));
		//adjust wh10
		for (x = 0; x < nr0; x++)
			for (q = 0; q < nr_lath0 * nr_lath0; q++) {
				xx = (j/h1 * dis0 + q/nr_lath0) % h0;
				yy = (j%h1 * dis0 + q%nr_lath0) % h0;
				deltaw = eta * delta10(0,0,j) * hid0(x, xx, yy);
				wh10(x,j,q) += deltaw + mom * dwh10(x,j,q);
				dwh10(x,j,q) = deltaw;
			}
		//adjust scale movement
		deltab = eta * delta10(0,0,j);
		bh10(0,0,j) += deltab + mom * dbh10(0,0,j);
		dbh10(0,0,j) = deltab;
	}
	//hid0 <- inp
	for (x = 0; x < nr0; x++) {
		for (r = 0; r < nr_latinp * nr_latinp; r++) {
			netdelta = 0.0;
			for (k = 0; k < M1; k++)
				for (q = 0; q < nr_lath0 * nr_lath0; q++) {
					if (r == 0) {
						xx = (k/h1 * dis0 + q/nr_lath0) % h0;
						yy = (k%h1 * dis0 + q%nr_lath0) % h0;
						xxyy(0,k,q) = xx;
						xxyy(1,k,q) = yy;
						netd(0,k,q) = delta10(0,0,k) * oldwh10(x, k, q) * hid0(x, xx, yy) * (1 - hid0(x, xx, yy));
					} else {
						xx = xxyy(0,k,q);
						yy = xxyy(1,k,q);
					}
					netdelta += netd(0, k, q) * inp(0,(xx * disin + r/nr_latinp) % l, (yy * disin + r%nr_latinp) % l);
				}
			deltaw = eta * netdelta;
			whin(0,x,r) += deltaw + mom * dwhin(0,x,r);
			dwhin(0,x,r) = deltaw;
		}
		//adjust scale movement BHIN
		for (i = 0; i < h0; i++)
			for (j = 0; j < h0; j++) {
				netdelta = 0.0;
				for (k = 0; k < M1; k++)
					for (q = 0; q < nr_lath0 * nr_lath0; q++) {
						if (!(xxyy(0,k,q) == i &&  xxyy(1,k,q) == j))
							continue;
						netdelta += netd(0,k,q);
					}
				deltab = eta * netdelta;
				bhin(0, x, i * h0 + j) += deltab + mom * dbhin(0, x, i * h0 + j);
				dbhin(0, x, i * h0 + j) = deltab;
			}
	}
}

void net_feedft::backward_2() {
	int i,j,k,x,y,q,r,xx,yy;
	float netdelta, deltaw, deltab;
	matrixf deltaohi(1,1,N);
	matrixf delta10(1,nr1,M1);
	matrixf oldwohi,oldwh10;
	oldwohi = wohi;
	oldwh10 = wh10;
	matrixf XX(nr1, M1, nr_lath0 * nr_lath0);
	matrixf YY(nr1, M1, nr_lath0 * nr_lath0);
	matrixf netd(nr1, M1, nr_lath0 * nr_lath0);

	//out >- hid1
	for (i = 0; i < N; i++) {
		deltaohi(0,0,i) = tp(0,0,i) - out(0,0,i);
		deltaohi(0,0,i) *= out(0,0,i) * (1 - out(0,0,i));
		for (x = 0; x < nr1; x++)
			for (j = 0; j < M1; j++) {
				deltaw = eta * deltaohi(0, 0, i) * hid1(x, j/h1, j%h1);
				wohi(x,i,j) += deltaw + mom * dwohi(x,i,j);
				dwohi(x,i,j) = deltaw;
			}
		deltab = eta * deltaohi(0,0,i);
		bohi(0,0,i) += deltab + mom * dbohi(0,0,i);
		dbohi(0,0,i) = deltab;
	}
	//hid1 <- hid0
	for (x = 0; x < nr1; x++) {
		for (j = 0; j < M1; j++) {
			netdelta = 0.0;
			for (i = 0; i < N; i++)
				netdelta += deltaohi(0,0,i) * oldwohi(x,i,j);
			delta10(0,x,j) = netdelta * hid1(x, j/h1, j%h1) * (1 - hid1 (x, j/h1, j%h1));
			//adjust BH10
			deltab = eta * delta10(0,x,j);
			bh10(0,x,j) += deltab + mom * dbh10(0, x, j);
			dbh10(0,x,j) = deltab;
		}
		//adjust matrix wh10
		for (y = 0; y < nr0; y++)
			for (q = 0; q < nr_lath0 * nr_lath0; q++) {
				netdelta = 0.0;
				for (k =0; k < M1; k++) {
					xx = (k/h1 * dis0 + q/nr_lath0) % h0;
					yy = (k%h1 * dis0 + q%nr_lath0) % h0;
					netdelta += delta10(0,x,k) * hid0(y, xx, yy);
				}
				deltaw = eta * netdelta;
				wh10(x,y,q) += deltaw + mom * dwh10(x,y,q);
				dwh10(x,y,q) = deltaw;
			}
	}
	//hid0 <- inp
	for (y = 0; y < nr0; y++) {
		for (r = 0; r < nr_latinp *nr_latinp; r++) {
			netdelta = 0.0;
			for (x = 0; x < nr1; x++)
				for (j = 0; j < M1; j++)
					for (q = 0; q < nr_lath0 * nr_lath0; q++) {
						if (r == 0) {
							xx = (j/h1 * dis0 + q/nr_lath0) % h0;
							yy = (j%h1 * dis0 + q%nr_lath0) % h0;
							XX(x,j,q) = xx;
							YY(x,j,q) = yy;
							netd(x,j,q) = delta10(0, x, j) * oldwh10(x, y, q)* hid0(y, xx, yy) * (1 - hid0(x, xx, yy));
						} else {
							xx = XX(x,j,q);
							yy = YY(x,j,q);
						}
						netdelta += netd(x,j,q) * inp(0, (xx * disin + r/nr_latinp) % l, (yy * disin + r%nr_latinp) % l);
					}
			deltaw = eta * netdelta;
			whin(0,y,r) += deltaw + mom * dwhin(0,y,r);
			dwhin(0,y,r) = deltaw;
		}
		//adjust scale movement
		for (k = 0; k < M0; k++) {
			netdelta = 0.0;
			for (x = 0; x < nr1; x++)
				for (j = 0; j < M1; j++)
					for (q = 0; q < nr_lath0 * nr_lath0; q++) {
						if (!(XX(x,j,q) * h0 + YY(x,j,q) == k))
							continue;
						netdelta += netd(x,j,q);
					}
			deltab = eta * netdelta;
			bhin(0,y,k) += deltab + mom * dbhin(0,y,k);
			dbhin(0,y,k) = deltab;
		}
	}
}

void net_feedft::init_net() {
	inp.init(1, l, l);
	out.init(1, 1, N);
	tp.init(1, 1, N);
	switch (nr_hidd) {
	case 1:
		if (LOADED == OFF) {
			wohi.init(nr0, N, M0);
			hid0.init(nr0, h0, h0);
			bhin.init(1, nr0, M0);
			whin.init(1, nr0, nr_latinp*nr_latinp);
		}
		dwohi.init(nr0, N, M0);
		dwhin.init(1, nr0, nr_latinp*nr_latinp);
		dbhin.init(1, nr0, M0);
		break;
	case 2:
		if (LOADED == OFF) {
			wohi.init(nr1, N, M1);
			hid0.init(nr0, h0, h0);
			bhin.init(1, nr0, M0);
			hid1.init(nr1, h1, h1);
			wh10.init(nr1, nr0, nr_lath0*nr_lath0);
			whin.init(1, nr0, nr_latinp*nr_latinp);
		}
		dwhin.init(1, nr0, nr_latinp*nr_latinp);
		dbhin.init(1, nr0, M0);
		dwh10.init(nr1, nr0, nr_lath0*nr_lath0);
		bh10.init(1, nr1, M1);
		dbh10.init(1, nr1, M1);
		dwohi.init(nr1, N, M1);
		break;
	}
	if (LOADED == OFF) {
		bohi.init(1, 1, N);
	}
	dbohi.init(1, 1, N);
}

int net_feedft::create() {
	return 0;
}

void net_feedft::saveInternal_inf(int dFile) {
	net_feedl::saveInternal_inf(dFile);
	write(dFile,&nr0,sizeof(int));
	write(dFile,&nr1,sizeof(int));
}
void net_feedft::loadInternal_inf(int dFile) {
	net_feedl::loadInternal_inf(dFile);
	read(dFile,&nr0,sizeof(int));
	read(dFile,&nr1,sizeof(int));
}

