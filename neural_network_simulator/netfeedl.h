/*
 * netfeedl.h
 * retele feedforward local conectate
 *  Created on: Apr 5, 2022
 *      Author: Gabriel Dimitriu
 *  @Copyright
 *  Code for net taken from SSN described in
 *  Retele neuronale artficiale by Gavril Toderean, Mircea Costeiu, Mircea Giurgiu
 *  Editura Albastra, Cluj-Napoca 1995
 */

#ifndef NETFEEDL_H_
#define NETFEEDL_H_

#include "netfeedf.h"

class net_feedl: public net_feedf {
public:
	net_feedl();
	virtual ~net_feedl();
	void friend info_net(int,int,net_feedl&);
	virtual int create();
	virtual void init_inp(matrix&);
	virtual void init_inpbin(int, float *);
	virtual void init_net();
	virtual void forward();
	virtual void backward();
	virtual int save(const char*);
	virtual int load_inf(const char*);
protected:
	//dimensiunea patratului de conexiune la nivelul de intrare
	int nr_latinp;
	//distanta dintre doua patrate
	int disin;
	//h0*h0=M0
	int h0;
	//dimensiunea patratului de conexiune
	int nr_lath0;
	//distanta intre patratele de conexiune
	int dis0;
	//h1*h1=M0
	int h1;
};

#endif /* NETFEEDL_H_ */
