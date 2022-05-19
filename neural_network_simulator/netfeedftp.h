/*
 * netfeedftp.h
 *
 *  Created on: Apr 5, 2022
 *      Author: Gabriel Dimitriu
 *  @Copyright
 *  Code for net taken from SSN described in
 *  Retele neuronale artficiale by Gavril Toderean, Mircea Costeiu, Mircea Giurgiu
 *  Editura Albastra, Cluj-Napoca 1995
 */

#ifndef NETFEEDFTP_H_
#define NETFEEDFTP_H_

#include "netfeedft.h"

class net_feedftp: public net_feedft {
public:
	net_feedftp();
	net_feedftp(int nr_hidd, int L, int M0, int M1, int N, int nr_latinp, int disin, int nr_lath0, int dis0, int nr0, int nr1);
	virtual ~net_feedftp() {}
	virtual void forward();
	virtual void backward();
private:
	//functia backward pentru 2 nivele ascunse si o singura harta de trasaturi in al noilea nivel ascuns
	void backward_1();
	//functia backward pentru 2 nivele ascunse si mai multe harti de trasaturi in al doilea nivel ascuns
	void backward_2();
};

#endif /* NETFEEDFTP_H_ */
