/*
 * netfeedftp.cpp
 *
 *  Created on: Apr 5, 2022
 *      Author: Gabriel Dimitriu
 *  @Copyright
 *  Code for net taken from SSN described in
 *  Retele neuronale artficiale by Gavril Toderean, Mircea Costeiu, Mircea Giurgiu
 *  Editura Albastra, Cluj-Napoca 1995
 */

#include "netfeedftp.h"

net_feedftp::net_feedftp() {
	typenet = '*';
}

net_feedftp::net_feedftp(int nr_hidd, int L, int M0, int M1, int N, int nr_latinp, int disin, int nr_lath0, int dis0,
		int nr0, int nr1) : net_feedft(nr_hidd,L,M0,M1,N,nr_latinp,disin,nr_lath0,dis0,nr0,nr1) {
	typenet = '*';
}

void net_feedftp::forward() {
	int i,j,k,x,y;
	float val;
	switch (nr_hidd) {
	case 1: //1 nivel ascuns
	{
		//hid0 <- inp
		for (x = 0; x < nr0; x++)
			for (i = 0; i < h0; i++)
				for (j = 0; j < h0; j++) {
					val = 0.0;
					for (k = 0; k < nr_latinp * nr_latinp; k++)
						val += whin(0, x, k) * inp(0, (i * disin + k/nr_latinp) % l, (j * disin + k%nr_latinp) % l);
					hid0(x,i,j) = to(val + bhin(0,0,x), temp);
				}
		//out <- hid0
		for (i = 0; i < N; i++) {
			val = 0.0;
			for (x = 0; x < nr0; x++)
				for (j = 0; j < h0; j++)
					for (k = 0; k < h0; k++)
						val += wohi(x, i, j * h0 + k) * hid0(x, j, k);
			out(0,0,i) = to(val + bohi(0, 0, i), temp);
		}
		break;
	}
	case 2: // 2 nivele ascunse
	{
		//hid0 <- inp
		for (x = 0; x < nr0; x++)
			for (i = 0; i < h0; i++)
				for (j = 0; j < h0; j++) {
					val = 0.0;
					for (k = 0; k < nr_latinp * nr_latinp; k++)
						val += whin(0, x, k) * inp(0, (i *disin + k/nr_latinp) % l, (j * disin + k%nr_latinp) % l);
					hid0(x,i,j) = to(val + bhin(0, 0, x), temp);
				}
		//hid1 <- hid0
		for (x = 0; x < nr1; x++)
			for (i = 0; i < h1; i++)
				for (j = 0; j < h1; j++) {
					val = 0.0;
					if (nr1 == 1) { //un singur plan
						for (k = 0; k < nr_lath0 * nr_lath0; k++)
							for (y = 0; y < nr0; y++)
								val += wh10(y, i * h1 +j, k) * hid0(y, (i * dis0 + k/nr_lath0) % h0, (j * dis0 + k%nr_lath0) % h0);
						hid1(x,i,j) = to(val + bh10(0, 0, i * h1 + j), temp);
					} else {
						for (k = 0; k < nr_lath0 * nr_lath0; k++)
							for (y = 0; y < nr0; y++)
								val += wh10(x, y, k) * hid0(y, (i * dis0 + k/nr_lath0) % h0, (j * dis0 + k%nr_lath0) % h0);
						hid1(x,i,j) = to(val + bh10(0, 0, x), temp);
					}
				}
		//out <- hid1
		for (i = 0; i < N; i++) {
			val = 0.0;
			for (x = 0; x < nr1; x++)
				for (j = 0; j < h1; j++)
					for (k = 0; k < h1; k++)
						val += wohi(x, i, j * h1 + k) * hid1(x, j, k);
			out(0, 0, i) = to(val + bohi(0, 0, i), temp);
		}
		break;
	}
	default:
		fprintf(stderr,"Illegal hidden layers number !!");
	}
}

void net_feedftp::backward() {
	int i,j,k,q,x;
	float netdelta, netdeltab, deltaw, deltab;
	matrixf deltaohi(1,1,N);
	matrixf delta(1,1,M0);
	matrixf oldwohi;
	oldwohi = wohi;
	switch(nr_hidd) {
	case 1: {
		//out <- hid0
		for (i = 0; i < N; i++) {
			deltaohi(0,0,i) = tp(0,0,i) - out(0,0,i);
			deltaohi(0,0,i) *= out(0,0,i) * (1 - out(0,0,i));
			for (x = 0; x < nr0; x++)
				for (j = 0; j < M0; j++) {
					deltaw = eta * deltaohi(0,0,i) * hid0(x, j/h0, j%h0);
					wohi(x,i,j) = wohi(x, i, j) + (deltaw + mom * dwohi(x, i, j));
					dwohi(x,i,j) = deltaw;
				}
			deltab = eta * deltaohi(0,0,i);
			bohi(0,0,i) = bohi(0,0,i) + (deltab + mom * dbohi(0,0,i));
			dbohi(0,0,i) = deltab;
		}
		//hid0 <- inp
		for (q = 0; q < nr_latinp * nr_latinp; q++)
			for (x = 0; x < nr0; x++) {
				deltaw = 0.0;
				deltab = 0.0;
				for (i = 0; i < h0; i++)
					for (j = 0; j < h0; j++) {
						if (q == 0) {
							netdelta = 0.0;
							for (k = 0; k < N; k++)
								netdelta += deltaohi(0,0,k) * oldwohi(x, k, i * h0 + j);
							delta(0, 0, i * h0 + j) = netdelta * hid0(x, i, j) * (1 - hid0(x, i, j));
						}
						deltaw += delta(0, 0, i * h0 + j) * inp(0, (i * disin + q/nr_latinp) % l, (j * disin + q%nr_latinp) %l);
						deltab += delta(0, 0, i * h0 + j);
					}
				if (q == 0) {
					//adjust scale movement
					deltab *= eta;
					bhin(0, 0, x) += deltab + mom * dbhin(0, 0, x);
					dbhin(0, 0, x) = deltab;
				}
				deltaw *= eta;
				whin(0,x,q) = whin(0, x, q) + (deltaw + mom * dwhin(0, x, q));
				dwhin(0,x,q) = deltaw;
			}
		break;
	}
	case 2: {
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

void net_feedftp::backward_1() {
	int i,j,k,x,q,r,xx,yy;
	float netdelta, deltaw, deltab;
	matrixf deltaohi(1,1,N);
	matrixf delta10(1,1,M1);
	matrixf oldwohi, oldwh10;
	oldwohi = wohi;
	oldwh10 = wh10;

	matrixf delta(1, M1, nr_lath0 * nr_lath0);
	//out <- hid1
	for (i = 0; i < N; i++) {
		deltaohi(0,0,i) = tp(0, 0, i) - out(0, 0, i);
		deltaohi(0,0,i) *= out(0, 0, i) * (1 - out(0, 0, i));
		for (j = 0; j < M1; j++) {
			deltaw = eta * deltaohi(0, 0, i) * hid1(0, j/h1, j%h1);
			wohi(0,i,j) += deltaw + mom * dwohi(0, i, j);
			dwohi(0,i,j) = deltaw;
		}
		deltab = eta * deltaohi(0, 0, i);
		bohi(0,0,i) += deltab + mom * dbohi(0, 0, i);
		dbohi(0,0,i) = deltab;
	}
	//hid1 <- hid0
	for (j = 0; j < M1; j++) {
		netdelta = 0.0;
		for (i = 0; i < N; i++)
			netdelta += deltaohi(0, 0, i) * oldwohi(0, i, j);
		delta10(0,0,j) = netdelta * hid1(0, j/h1, j%h1) * (1 - hid1(0, j/h1, j%h1));
		//adjust matrix wh10
		for (x = 0; x < nr0; x++)
			for (q = 0; q < nr_lath0 * nr_lath0; q++) {
				xx = (j/h1 * dis0 + q/nr_lath0) % h0;
				yy = (j%h1 * dis0 + q%nr_lath0) % h0;
				deltaw = eta * netdelta * hid0(x, xx, yy);
				wh10(x, j, q) += deltaw + mom * dwh10(x, j, q);
				dwh10(x,j,q) = deltaw;
			}
		//adjust scale mapping
		deltab = eta * delta10(0, 0, j);
		bh10(0,0,j) += deltab + mom * dbh10(0, 0, j);
		dbh10(0,0,j) = deltab;
	}
	//hid0 <- inp
	for (x = 0; x < nr0; x++) {
		netdelta = 0.0;
		for (j = 0; j < M1; j++)
			for (q = 0; q < nr_lath0 * nr_lath0; q++) {
				xx = (j/h1 * dis0 + q/nr_lath0) % h0;
				yy = (j%h1 * dis0 + q%nr_lath0) % h0;
				delta(0,j,q) = delta10(0, 0, j) * oldwh10(x, j, q) * hid0(x, xx, yy) * (1 - hid0(x, xx, yy));
				netdelta += delta(0, j, q);
			}
		//adjust scale movement
		deltab = netdelta * eta;
		bhin(0,0,x) += deltab + mom * dbhin(0, 0, x);
		dbhin(0,0,x) = deltab;
		//weights adjust
		for (r = 0; r < nr_latinp * nr_latinp; r++) {
			netdelta = 0.0;
			for (j = 0; j < M1; j++)
				for (q = 0; q < nr_lath0 * nr_lath0; q++) {
					xx = (j/h1 * dis0 + q/nr_lath0) % h0;
					yy = (j%h1 * dis0 + q%nr_lath0) % h0;
					netdelta += delta(0, j, q) * inp(0, (xx * disin + r/nr_latinp) % l, (yy * disin + r%nr_latinp) % l);
				}
			deltaw = eta * netdelta;
			whin(0, x, r) += deltaw + mom * dwhin(0, x, r);
			dwhin(0, x, r) = deltaw;
		}
	}
}

void net_feedftp::backward_2() {
	int i,j,k,x,y,q,r,xx,yy;
	float netdelta, netdeltab, deltaw, deltab, cc,cc1;
	matrixf deltaohi(1,1,N);
	matrixf delta10(1,nr1,M1);
	matrixf oldwohi, oldwh10;
	oldwohi = wohi;
	oldwh10 = wh10;
	matrixf XX(nr1 ,M1, nr_lath0 * nr_lath0);
	matrixf YY(nr1, M1, nr_lath0 * nr_lath0);
	matrixf netd(nr1, M1, nr_lath0 * nr_lath0);
	//out <- hid1
	for (i = 0; i < N; i++) {
		deltaohi(0,0,i) = tp(0, 0, i) - out(0, 0, i);
		deltaohi(0,0,i) *= out(0, 0, i) * (1 - out(0, 0, i));
		for (x = 0; x < nr1; x++)
			for (j = 0; j < M1; j++) {
				deltaw = eta * deltaohi(0, 0, i) * hid1(x, j/h1, j%h1);
				wohi(x,i,j) += deltaw + mom * dwohi(x, i, j);
				dwohi(x,i,j) = deltaw;
			}
		deltab = eta * deltaohi(0, 0, i);
		bohi(0,0,i) += deltab + mom * dbohi(0, 0, i);
		dbohi(0,0,i) = deltab;
	}
	//hid1 <- hid0
	for (y = 0; y < nr0; y++)
		for (q = 0; q < nr_lath0 * nr_lath0; q++)
			for (x = 0; x < nr1; x++) {
				netdelta = 0.0;
				netdeltab = 0.0;
				for (j = 0; j < M1; j++) {
					cc = 0.0;
					for (i = 0; i < N; i++)
						cc += deltaohi(0, 0, i) * oldwohi(x, i, j);
					cc *= hid1(x, j/h1, j%h1) * (1 - hid1(x, j/h1, j%h1));
					cc1 = cc * hid0(y, (j/h1 * dis0 + q/nr_lath0) % h0, (j%h1 + q%nr_lath0) % h0);
					netdeltab += cc;
					netdelta += cc1;
					if (y == 0 && q == 0)
						delta10(0, x, j) = cc;
				}
				if (y == 0 && q == 0) {
					deltab = eta * netdeltab;
					bh10(0,0,x) += deltab + mom * dbh10(0, 0, x);
					dbh10(0,0,x) = deltab;
				}
				//adjust matrix wh10
				deltaw = eta * netdelta;
				wh10(x,y,q) += deltaw + mom * dwh10(x, y, q);
				dwh10(x,y,q) = deltaw;
			}
	//hid0 <- inp
	for (y = 0; y < nr0; y++) {
		netdelta = 0.0;
		for (j = 0; j < M1; j++)
			for (q = 0; q < nr_lath0 * nr_lath0; q++) {
				xx = (j/h1 * dis0 + q/nr_lath0) % h0;
				yy = (j%h1 * dis0 + q%nr_lath0) % h0;
				for (x = 0; x < nr1; x++) {
					netd(x,j,q) = delta10(0, x, j) * oldwh10(x, y, q) * hid0(y, xx, yy) * (1 - hid0(y, xx, yy));
					netdelta += netd(x, j, q);
				}
			}
		deltab = eta * netdeltab;
		bhin(0, 0, y) += deltab + mom * dbhin(0, 0, y);
		dbhin(0, 0, y) = deltab;
		for (r = 0; r < nr_latinp * nr_latinp; r++) {
			netdelta = 0.0;
			for (j = 0; j < M1 ; j++)
				for (q = 0; q < nr_lath0 * nr_lath0; q++) {
					xx = (j/h1 * dis0 + q/nr_lath0) % h0;
					yy = (j%h1 * dis0 + q%nr_lath0) % h0;
					for (x = 0; x < nr1; x++)
						netdelta += netd(x, j, q) * inp(0, (xx * disin + r/nr_latinp) % l, (yy * disin + r%nr_latinp) % l);
				}
			deltaw = eta * netdelta;
			whin(0,y,r) += deltaw + mom * dwhin(0, y, r);
			dwhin(0,y,r) = deltaw;
		}
	}
}
