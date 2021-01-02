// TMemory.cpp: implementation of the CTMemory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RWBitmap.h"
#include "TMemory.h"
#include <string.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTMemory::CTMemory()
{
	m_bitmap=NULL;
	m_cmem=NULL;
	m_mem=NULL;
	m_mem_q=NULL;
}

CTMemory::~CTMemory()
{
	if(m_file.is_open()!=0) m_file.close();
	free(m_fname);
	while(m_mem!=NULL)
	{
		m_cmem=m_mem;
		m_mem=m_mem->next;
		free(*(m_cmem->data));
		free(m_cmem->data);
		free(m_cmem);
	}
}

void CTMemory::SetFileName(char *name)
{
	m_fname=(char *)calloc(strlen(name),sizeof(char));
	m_fname=strcpy(m_fname,name);
}

bool CTMemory::Get(unsigned long ***data,unsigned int *type)
{
	if(m_cmem==NULL) return false;
	else
	{
		*data=m_cmem->data;
		*type=m_cmem->type;
		m_cmem=m_cmem->next;
		return true;
	}
}

bool CTMemory::GetEx(unsigned long **data, unsigned int *type)
{
	if(m_cmem==NULL) return false;
	else
	{
		*data=&(m_cmem->data[0][0]);
		*type=m_cmem->type;
		m_cmem=m_cmem->next;
		return true;
	}
}

void CTMemory::Put(char *name)
{
	unsigned long i, j, k, l;
	unsigned long* ptemp;
	if(m_file.is_open()!=0) m_file.close();
	m_file.open(m_fname,ios::app | ios::binary );
	m_bitmap = new CRWBitmap();
	m_bitmap->SetIFileName(name);
	m_bitmap->ReadFH();
	m_bitmap->ReadData();
	struct smem *stemp;
	stemp=(struct smem *)calloc(1,sizeof(struct smem));
	if(m_mem==NULL) m_mem=m_cmem=stemp;
	else m_mem_q->next=stemp;
	m_mem_q=stemp;
	stemp->next=NULL;
	stemp->data=(unsigned long **)calloc(32,sizeof(unsigned long *));
	ptemp=(unsigned long *)calloc(32*32,sizeof(unsigned long));
	for(i=0;i<32;i++)
	{
		stemp->data[i]=ptemp;
		ptemp+=32;
	}
	//average window
	unsigned long temp;
	unsigned long ltemp;
	for(i=0;i<32;i++)
	{
		for(j=0;j<32;j++)
		{
			temp=0;
			for (k = 0; k < 8; k++)
				for (l = 0; l < 8; l++)
					temp += m_bitmap->data[i * 8 + k][j * 8 + l] >> 16;
			temp=temp/64;
			ltemp=temp;
			stemp->data[i][j] = (ltemp << 24) >> 8;
		}
	}
	m_file.write((char *)(&(stemp->data[0][0])),32*32*sizeof(unsigned long));
	m_cmem=stemp;
	delete m_bitmap;
}

void CTMemory::Reset()
{
	m_cmem=m_mem;
}

void CTMemory::ReadData()
{
	if(m_file.is_open()!=0) m_file.close();
	m_file.open(m_fname,ios::in | ios::binary);
	while(m_mem!=NULL)
	{
		m_cmem=m_mem;
		m_mem=m_mem->next;
		free(*(m_cmem->data));
		free(m_cmem->data);
		free(m_cmem);
	}
	struct smem *stemp;
	unsigned long *ptemp,i;
	while(m_file.eof()==0)
	{
		stemp=(struct smem *)calloc(1,sizeof(struct smem));
		if(m_mem==NULL) m_mem=m_cmem=stemp;
		else m_mem_q->next=stemp;
		stemp->next=NULL;
		stemp->data=(unsigned long **)calloc(32,sizeof(unsigned long *));
		ptemp=(unsigned long *)calloc(32*32,sizeof(unsigned long));
		for(i=0;i<32;i++)
		{
			stemp->data[i]=ptemp;
			ptemp+=32;
		}
		m_file.read((char *)(&(stemp->data[0][0])),32*32*sizeof(unsigned long));
		m_file.read((char *)(&(stemp->type)),sizeof(unsigned int));
		if(m_file.eof()!=0)
		{
			free(*(stemp->data));
			free(stemp->data);
			free(stemp);
			m_mem_q->next=NULL;
			break;
		}
		else	m_mem_q=stemp;
	}
	m_file.close();
}

void CTMemory::SetType(unsigned int type)
{
	m_mem_q->type=type;
	m_file.write((char *)(&type),sizeof(unsigned int));
	m_file.close();
}

void CTMemory::WriteData()
{
	if(m_file.is_open()!=0) m_file.close();
	m_file.open(m_fname,ios::trunc | ios::out |ios::binary);
	m_cmem=m_mem;
	while(m_cmem!=NULL)
	{
		m_file.write((char *)(&(m_cmem->data[0][0])),32*32*sizeof(unsigned long));
		m_file.write((char *)(&(m_cmem->type)),sizeof(unsigned int));
		m_cmem=m_cmem->next;
	}
	m_file.close();
}


void CTMemory::GetDim(unsigned long *dim,unsigned int type)
{
	unsigned int i;
	for(i=0;i<type;i++) dim[i]=0;
	m_cmem=m_mem;
	while(m_cmem!=NULL)
	{
		dim[m_cmem->type-1]++;
		m_cmem=m_cmem->next;
	}
}
