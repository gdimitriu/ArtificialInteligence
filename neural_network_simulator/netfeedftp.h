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
	virtual ~net_feedftp();
	void friend info_net(int, int, net_feedftp&);
	virtual int create();
	virtual void init_inp(matrix&);
	virtual void init_inpbin(int, float*);
	virtual void init_net();
	virtual void forward();
	virtual void backward();
	virtual int save(const char *);
	virtual int load_inf(const char *);
	//functia backward pentru 2 nivele ascunse si o singura harta de trasaturi in al noilea nivel ascuns
	virtual void backward_1();
	//functia backward pentru 2 nivele ascunse si mai multe harti de trasaturi in al doilea nivel ascuns
	virtual void backward_2();
};

#endif /* NETFEEDFTP_H_ */
