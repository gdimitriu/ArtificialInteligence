/*
 * netfeedft.h
 * retele feedforward cu extractori ierarhici de trasaturi
 *  Created on: Apr 5, 2022
 *      Author: Gabriel Dimitriu
 *  @Copyright
 *  Code for net taken from SSN described in
 *  Retele neuronale artficiale by Gavril Toderean, Mircea Costeiu, Mircea Giurgiu
 *  Editura Albastra, Cluj-Napoca 1995
 */

#ifndef NETFEEDFT_H_
#define NETFEEDFT_H_

#include "netfeedl.h"

class net_feedft: public net_feedl {
public:
	net_feedft();
	virtual ~net_feedft();
	virtual int create();
	virtual void init_inp(matrixf& input);
	virtual void init_inp(matrix& input);
	virtual void init_net();
	virtual void forward();
	virtual void backward();
	virtual int save(const char*);
	virtual int load_inf(const char*);
	//functia backward pentru 2 nivele ascunse si o singura harta de trasaturi in al noilea nivel ascuns
	virtual void backward_1();
	//functia backward pentru 2 nivele ascunse si mai multe harti de trasaturi in al doilea nivel ascuns
	virtual void backward_2();
protected:
	virtual void saveInternal_inf(int fdescriptor);
	virtual void loadInternal_inf(int fdescriptor);
	//numarul de harti de trasaturi din primul nivel ascuns
	int nr0;
	//numarul de harti de trasaturi din cel de-al doilea nivel ascuns
	int nr1;
};

#endif /* NETFEEDFT_H_ */
