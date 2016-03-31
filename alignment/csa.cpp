#include "../csa/CSA.h"
#include "../utils.h"
#include "csa.h"



csa_t* csa_construct_core(ubyte_t* T,int64_t seq_len,char* prefix)
{
    csa_t* csa=new csa_t();
    csa->csa= new CSA(T,seq_len,prefix,64);
    return csa;
}

int64_t csa_dump(csa_t* csa,char* csafile)
{
    return csa->csa->Save(csafile);
}

csa_t* csa_restore(char* csafile)
{
    csa_t* csa = new csa_t();
    csa->csa = new CSA();
    csa->csa->Load(csafile);
    return csa;
}

void csa_restore_sa_back(csa_t* csa,char* safile)
{
    csa->sa=new int64_t[csa->csa->GetN()];
    FILE* fp=xopen(safile,"rb");
    err_fseek(fp,0,SEEK_END);
    int64_t sa_size=err_ftell(fp);
	xassert(sa_size/sizeof(integer) == csa->csa->GetN(), "Error: Read SA error");
	err_fseek(fp,0,SEEK_SET);
    err_fread_noeof(csa->sa,sizeof(int64_t),csa->csa->GetN(),fp);
    err_fclose(fp);
}


int64_t csa_destroy(csa_t* csa)
{
    if(csa!=NULL)
    {
        if(csa->csa)
            delete csa->csa;
        if(csa->sa)
            delete[] csa->sa;
        delete csa;
        return 0;
    }
    return -1;
}

int64_t csa_match_exact_alt(csa_t *csa, u64_t len, const ubyte_t *str, u64_t *k0, u64_t *l0)
{
    int64_t i;
    u64_t k, l;
    k = *k0;
    l = *l0;
    for (i = len - 1; i >= 0; --i) {
        ubyte_t c = str[i];
        if (c > 3)
            return 0; // there is an N here. no match
        csa->csa->boundary(k,l,k0,l0,c);
        k = *k0;
        l = *l0;
        if (k > l) return 0; // no match
    }
    return l - k + 1;
}

int64_t csa_sa(const csa_t *csa,u64_t pos)
{
	if(csa)
		return csa->csa->lookup(pos);
	else
	{
		fprintf(stderr,"[%s]:csa is null\n",__func__);
		exit(1);
	}
}

int64_t csa_sa_back(const csa_t* csa,u64_t pos)
{
    if(csa && csa->sa)
        return (u64_t)csa->sa[pos];
    else
        return (u64_t)-1;
}
