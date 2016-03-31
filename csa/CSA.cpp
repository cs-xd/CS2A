/*============================================
# Filename: CSA.cpp
# Ver 1.0 2014-06-09
# Copyright (C) 2014 ChenLonggang (chenlonggang.love@163.com)
#
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 or later of the License.
#
# Description: 
=============================================*/
#include<stdlib.h>
#include "CSA.h"
#include<cstdio>
#include<cmath>
#include"divsufsort64.h"
#include<iostream>
using namespace std;
#define SearchKind 2


integer CSA::Save(const char * indexfile)
{
	savekit s(indexfile);

	s.writeu64(198809102510);
	s.writeinteger(n);
	s.writeinteger(alphabetsize);

	s.writeinteger(SL);
	s.writeinteger(L);
	s.writeinteger(D);
	s.writeinteger(RD);
	//sal
	SAL->write(s);
	//ranl
	RankL->write(s);
	//code
	s.writeinteger(256);
	s.writeintegerarray(code,256);
	//start
	s.writeinteger(alphabetsize+1);
	s.writeintegerarray(start,alphabetsize+1);
	//incode
	s.writeinteger(alphabetsize);
	s.writeintegerarray(incode,alphabetsize);

	//phi0
	Phi0->write(s);
	s.close();
	return 0;
}

integer CSA::Load(const char * indexfile)
{
	loadkit s(indexfile);
	u64 magicnum;
	s.loadu64(magicnum);
	if(magicnum!=198809102510)
	{
		cerr<<"Not a CSA file"<<endl;
		exit(0);
	}
	s.loadinteger(this->n);
	s.loadinteger(this->alphabetsize);

	s.loadinteger(SL);
	s.loadinteger(L);
	s.loadinteger(D);
	s.loadinteger(RD);

	SAL=new InArray();
	SAL->load(s);

	RankL=new InArray();
	RankL->load(s);

	//code
	integer len=0;
	s.loadinteger(len);
	this->code=new integer[len];
	s.loadintegerarray(this->code,len);
	//start
	s.loadinteger(len);
	this->start=new integer[len];
	s.loadintegerarray(this->start,len);
	//incode
	s.loadinteger(len);
	this->incode=new integer[len];
	s.loadintegerarray(this->incode,len);

	Phi0=new Phi();
	Phi0->load(s);
	s.close();
	return 0;
}

CSA::CSA(const char * sourcefile,integer L,integer D,integer phitype)
{
	this->SL=L*18;
	this->L =L;
	this->D =D;
	this->RD=D*16;
	this->code =new integer[256];
	for(integer i=0;i<256;i++)
		code[i]=0;

	this->alphabetsize =0;
	uchar *T=NULL;
	T=Getfile(sourcefile);
	statics(T);
	integer *SA=new integer[n];
	//cout<<"before SA"<<endl;
	divsufsort64(T,SA,n);
	//cout<<"SA is ok"<<endl;
	parmaters p={alphabetsize,n,SL,L,start,lastchar,SA,T,code,phitype};
	CreateSupportStructer(&p);

	if(SA!=NULL)
		delete [] SA;
	if(T!=NULL)
		delete [] T;
	cout<<"CSA  is done"<<endl;
}

//bluecliff add on 14/12/09
CSA::CSA(uchar *T,integer length,char *prefix,integer L,integer D,integer phitype)
{
	this->SL=L*18;
	this->L =L;
	this->D =D;
	this->RD=D*16;
	this->code =new integer[256];
	this->n=length;
	for(integer i=0;i<256;i++)
		code[i]=0;

	this->alphabetsize =0;
	statics(T);
	integer *SA=new integer[n];
	clock_t beg,end;
	fprintf(stderr,"%s: %ld bytes...\n",prefix,n);
	beg=clock();
	if(0!=divsufsort64(T,SA,n))
	{
		fprintf(stderr,"%s: can not allocate memory.\n",__func__);
		exit(EXIT_FAILURE);
	}
	end = clock();
	fprintf(stderr,"sa is done... %.4f seconds\n",(double)(end-beg)/(double)CLOCKS_PER_SEC);
	//save sa
	//char *safile=(char*)malloc(sizeof(char)*(strlen(prefix)+5));
	//strcpy(safile,prefix);
	//strcat(safile,".sa");
	//FILE *fp;
	//if((fp=fopen(safile,"wb"))==NULL)
	//{
	//	fprintf(stderr,"%s: can not open file '%s'\n",__func__,safile);
	//	perror(NULL);
	//	exit(EXIT_FAILURE);
	//}
	//if(fwrite(SA,sizeof(integer),(size_t)n,fp)!=(size_t)n)
	//{
	//	fprintf(stderr,"%s: %s: '%s'\n",__func__,(ferror(fp)||!feof(fp))? "Can not write to":"Unexpected EOF in",safile);
	//	perror(NULL);
	//	exit(EXIT_FAILURE);
	//}
	//fclose(fp);
	
	//create csa
	beg=clock();
	parmaters p={alphabetsize,n,SL,L,start,lastchar,SA,T,code,phitype};
	CreateSupportStructer(&p);

	if(SA!=NULL)
		delete [] SA;
	//if(safile!=NULL)
	//	free(safile);
	end=clock();
	fprintf(stderr,"csa is done...%.4f sec\n",(double)(end-beg)/(double)CLOCKS_PER_SEC);
}
void CSA::boundary(u64 left,u64 right,u64 *l,u64 *r,uchar ch)
{
	integer coding=code[ch];
	integer l0=start[coding];
	integer r0=start[coding+1]-1;
	if(left==0 || right==n-1)
	{
		*l=l0;
		*r=r0;
	}
	else
	{
		*r=Phi0->RightBoundary(right,l0,r0);
		*l=Phi0->LeftBoundary(left,l0,r0);
	}
}
//bluecliff end
integer CSA::GetN()
{
	return n;
}

integer CSA::Getalphabetsize()
{
	return this->alphabetsize;
}

CSA::~CSA(void)
{
	delete SAL;
	delete RankL;
	delete [] start;
	delete Phi0;
	delete [] code;
	delete [] incode;
}


bool CSA::Existential(const char *Pattern)
{
	integer L=0;
	integer R=0;
	Search2(Pattern,L,R);

	if(R>=L)
		return true;
	else
		return false;
}

void CSA::Counting(const char *Pattern,integer &num)
{
	integer L=0;
	integer R=0;

	Search2(Pattern,L,R);
	cout<<L<<" "<<R<<endl;
	num=R-L+1;
}

void CSA::statics(uchar * T)
{
	for(integer i=0;i<n;i++)
		code[T[i]]++;

	for(integer i=0;i<256;i++)
		if(code[i]!=0)
			this->alphabetsize ++;
	this->start =new integer[this->alphabetsize +1];
	this->start [this->alphabetsize ]=n;
	this->start [0]=0;
	integer k=1;
	integer pre=0;
	for(integer i=0;i<256;i++)
		if(code[i]!=0)
		{
			start[k]=pre+code[i];
			pre=start[k];
			k++;
		}
	this->incode =new integer[this->alphabetsize];
	k=0;
	for(integer i=0;i<256;i++)
		if(code[i]!=0)
		{
			code[i]=k;
			incode[k]=i;
			k++;
		}
		else
			code[i]=-1;
	lastchar=T[n-1];

}


uchar* CSA::Getfile(const char * filename)
{

	FILE *fp=fopen(filename,"r+");
	if(fp==NULL)
	{
		cout<<"Be sure that the file is available"<<endl;
		exit(0);
	}
	fseek(fp , 0, SEEK_END);       //�ö�дָ��Ϊ�ļ�ĩβ
	this->n = ftell(fp);           //���ض�дָ��ƫ����
	uchar * T=new uchar[n];
	fseek(fp , 0, SEEK_SET);

	integer e=0;
	integer num=0;
	while((e=fread(T+num,sizeof(uchar),n-num,fp))!=0)
		num=num+e;
	if(num!=n)
		exit(0);
	fclose(fp);
	return T;
}
double CSA::Size()
{
	return (SAL->GetMemorySize ()+Phi0->Size()+RankL->GetMemorySize ())/(1.0*n);
}


void CSA::CreateSupportStructer(parmaters *csa)
{
	integer i=0;
	integer j=0;
	integer step1=D;
	integer step2=RD;
	SAL=new InArray(n/step1+1,blog(n));
	RankL=new InArray(n/step2+1,blog(n));
	for(i=0,j=0;i<n;i=i+step1,j++)
	{
		SAL->SetValue (j,csa->SA[i]);
	}

	for(i=0;i<n;i++)
	{
		if(csa->SA[i]%step2==0)
			RankL->SetValue (csa->SA[i]/step2,i);
	}
	//cout<<"before phi"<<endl;
	Phi0=new Phi(csa);
	//cout<<"phi is OK"<<endl;
}

void CSA::Search2(const char *Pattern, integer &L, integer &R)
{
	integer len=strlen(Pattern);
	if(len==0)
	{
		L=1;
		R=0;
		return;
	}
	unsigned char c=Pattern[len-1];
	integer coding=code[c];
	if(coding>alphabetsize-1||coding<0)
	{
		L=1;
		R=0;
		return ;
	}
	integer Left=start[coding];
	integer Right=start[coding+1]-1;
	integer l0=0;
	integer r0=0;

	for(integer i=len-2;i>=0;i--)
	{
		c=Pattern[i];
		coding=code[c];
		if(coding<0)
		{
			Left=1;
			Right=0;
			break;
		}
		l0=start[coding];
		r0=start[coding+1]-1;

		Right=Phi0->RightBoundary(Right,l0,r0);
		Left=Phi0->LeftBoundary(Left,l0,r0);
		if(Left>Right)
		{
			Left=1;
			Right=0;
			break;
		}
	}
	L=Left;
	R=Right;
	return ;


}
void CSA::Search(const char *Pattern, integer &L, integer &R)
{
	integer templeft;
	integer tempright;
	integer jj;
	integer mleft;
	integer mright;
	integer middle;
	integer Right;
	integer left;
	integer right;
	integer Left;
	integer i;
	integer len=strlen(Pattern);
	unsigned char c=Pattern[len-1];
	integer coding=code[c];
	if(coding>alphabetsize-1)
	{
		L=1;
		R=0;
		return ;
	}

	Left=start[coding];
	Right=start[coding+1]-1;//start����Ĵ�СΪalphabetsize+1��ʵ�����õ�Ϊalphabetsize�������һ����Ϊ�˷�ֹcoding+1Խ��ģ�
	//����start[alphabetsize]����start��������һ��ְλn�����Phi0��Ĺ��캯����
	for(i=len-2;i>=0;i--)
	{
		c=Pattern[i];
		coding =code[c];
		if(coding>alphabetsize-1)
		{
			L=1;
			R=0;
			return ;
		}
		left=start[coding];
		right=start[coding+1]-1;
		if(coding==code[lastchar])
			left=left+1;
		if(left>right || Phi0->GetValue (left)>Right || Phi0->GetValue (right)<Left)
		{
			L=1;
			R=0;
			return ;
		}
		else
		{
			mleft=left;
			mright=right;
			while(mright-mleft>1)
			{
				middle=(mleft+mright)/2;
				jj=Phi0->GetValue (middle);
				if(jj<Left)
					mleft=middle;
				else
					mright=middle;
			}
			jj=Phi0->GetValue (mleft);
			if(jj>=Left && jj<=Right)
				templeft=mleft;
			else
				templeft=mright;
			mleft=left;
			mright=right;
			while(mright-mleft>1)
			{
				middle=(mleft+mright)/2;
				jj=Phi0->GetValue (middle);
				if(jj>Right)
					mright=middle;
				else
					mleft=middle;
			}
			jj=Phi0->GetValue (mright);
			if(jj>=Left && jj<=Right)
				tempright=mright;
			else
				tempright=mleft;
			Left=templeft;
			Right=tempright;
			if(Left>Right)
			{
				L=1;
				R=0;
				return;
			}
		}
	}
	if(Left>Right)
		L=1,R=0;
	else
		L=Left,R=Right;
}

integer CSA::lookup(integer i)
{
	integer D=this->D;
	integer step=0;
	while(i%D!=0)
	{
		i=Phi0->GetValue(i);
		step++;
	}
	i=i/D;
	return (n+SAL->GetValue (i)-step)%n;
}
/*
   void CSA::SelfTesting()
   {
   cout<<"CSA is Selftesting..."<<endl;
   integer k=0;
   integer value=0;
   for(integer i=0;i<n;i++)
   {
   value=lookup(i);
   if(SA[i]!=value)
   k++;
   }
   if(k!=0)

   cout<<"CSA is wrong     "<<k<<endl;
   else
   cout<<"CSA is right"<<endl;
   }
   */

//�õ�λ��i������
integer CSA::Inverse(integer i)
{
	integer RD=this->RD ;
	integer anchor=i/RD;
	integer p=anchor*RD;
	integer sa=this->RankL ->GetValue (anchor);
	while(p<i)
	{
		sa=Phi0->GetValue(sa);
		p++;
	}
	return sa;
}

void CSA::Decompress(integer i, integer len,unsigned char *p)
{
	//integer * phi=Phi0->GetPhiArray();
	integer k=0;
	i=this->Inverse (i);
	for(integer j=0;j<len;j++)
	{
		k=this->Phi_list (i);
		p[j]=this->Character (k);
		//i=phi[i];
		i=Phi0->GetValue(i);
	}
}

integer CSA::Phi_list(integer i)
{
	integer l=0;
	integer r=this->alphabetsize ;
	integer m=0;
	while(l<r)
	{
		m=(l+r)/2;
		if(start[m]<=i)
			l=m+1;
		else
			r=m;
	}
	return r-1;
}

integer CSA::Character(integer i)
{
	return incode[i];
}

integer CSA::blog(integer x)
{
	integer ans=0;
	while(x>0)
	{
		ans++;
		x=x>>1;
	}
	return ans;
}

void CSA::Locating(const char *Pattern, integer &num, integer *&pos)
{
	integer L=0;
	integer R=0;
	this->Search2(Pattern,L,R);
	num=R-L+1;
	if(L>R)
		return ;
	pos=new integer[num];
	//	if(num>50)
	//		Enumerative2(L,R,pos);
	//	else
	Enumerative1(L,R,pos);


}

void CSA::Enumerative2(integer L,integer  R, integer *&pos)
{
	integer D=this->D;
	InArray *SAL=this->SAL ;
	integer * distance=new integer[R-L+1];
	integer *pred=new integer[R-L+1];
	integer f=0;
	integer step=0;
	integer q=0;
	integer s=0;
	integer i=0;
	for(integer i=0;i<R-L+1;i++)
	{
		pos[i]=0;
		pred[i]=-1;
	}
	for(integer j=L;j<=R;j++)
	{
		f=0;
		i=j;
		step=0;
		while(i%D!=0)
		{
			i=this->Phi0->GetValue(i);
			step++;
			if(L<=i&&i<=R)
			{
				distance[j-L]=step;
				pred[i-L]=j;
				f=1;
				break;
			}
		}
		if(f==0)
		{
			i=i/D;
			pos[j-L]=SAL->GetValue(i)-step;
		}
	}
	integer fu=0;
	for(integer j=L;j<=R;j++)
	{
		if(pos[j-L]!=0)
		{
			q=j;
			while(pred[q-L]!=-1)
			{
				fu++;
				s=pos[q-L];
				i=pred[q-L];
				step=distance[i-L];
				pos[i-L]=s-step;
				pred[q-L]=-1;
				q=i;
			}
		}
	}
	delete [] pred;
	delete [] distance;
}
void CSA::Enumerative1(integer L,integer R,integer *&pos)
{


	integer i;
	for(i=L;i<=R;i++)
	{
		pos[i-L]=lookup (i);
	}

}
