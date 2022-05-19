// TMemory.h: interface for the CTMemory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMEMORY_H__0222248F_3B52_11DA_B910_F80BF543D709__INCLUDED_)
#define AFX_TMEMORY_H__0222248F_3B52_11DA_B910_F80BF543D709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <fstream>
#include <string.h>
#include "RWBitmap.h"
using namespace std;

class CTMemory  
{
public:
	int GetTotalNr();
	void GetDim(unsigned long *dim,unsigned int size);
	void WriteData();
	void SetType(unsigned int type);
	void ReadData();
	void Reset();
	void Put(char *name);
	bool Get(unsigned long ***data,unsigned int *type);
	bool GetEx(unsigned long *data,unsigned int *type);
	void SetFileName(char *name);
	CTMemory();
	virtual ~CTMemory();
protected:
	char * m_fname;
	struct smem
	{
		unsigned long **data;
		unsigned int type;
		struct smem *next;
	};
	fstream m_file;
	struct smem *m_mem;
	struct smem *m_mem_q;
	struct smem *m_cmem;
	CRWBitmap *m_bitmap;
};

#endif // !defined(AFX_TMEMORY_H__0222248F_3B52_11DA_B910_F80BF543D709__INCLUDED_)
