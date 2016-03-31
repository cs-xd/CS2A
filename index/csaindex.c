#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <zlib.h>
#include "../seqio/bntseq.h"
#include "../alignment/csa.h"
#include "../utils.h"
#include "csaindex.h"

int64_t get_seq_len(char* packfile)
{
	FILE *fp;
	int64_t pac_len;
	ubyte_t c;
	fp = xopen(packfile, "rb");
	err_fseek(fp, -1, SEEK_END);
	pac_len = err_ftell(fp);
	err_fread_noeof(&c, 1, 1, fp);
	err_fclose(fp);
	return (pac_len - 1) * 4 + (int)c;
}


csa_t* csa_construct(char* packfile,char* prefix)
{
    ubyte_t *buf,*buf2;
	int64_t seq_len = get_seq_len(packfile);
	FILE* fp = xopen(packfile, "rb");

	// prepare sequence
	int64_t pac_size = (seq_len>>2) + ((seq_len&3) == 0? 0 : 1);
	buf2 = (ubyte_t*)calloc(pac_size, 1);
	err_fread_noeof(buf2, 1, pac_size, fp);
	err_fclose(fp);
	buf = (ubyte_t*)calloc(seq_len + 1, 1);
	for (int64_t i = 0; i < seq_len; ++i) {
		buf[i] = buf2[i>>2] >> ((3 - (i&3)) << 1) & 3;
	}
	free(buf2);
    //construct csa index
   csa_t* csa = csa_construct_core(buf,seq_len,prefix);
   return csa;
}



int csa_index(int argc,char* argv[])
{
    char* prefix=0,*str,*str2,*str3;
    int c;
    clock_t t;
    int64_t l_pac;

    while((c=getopt(argc,argv,"p:"))>=0)
    {
        switch(c)
        {
            case 'p':
                prefix=strdup(optarg);
                break;
            default:
                return 1;
        }
    }
    if(optind+1>argc)
    {
        fprintf(stderr,"\n");
        fprintf(stderr,"Usage: csaa index [-c] <in.fasta>\n");
        fprintf(stderr,"Options: -p STR     prefix of the index [same as fasta name]\n");
        return 1;
    }
    if(prefix==0)
    {
        prefix = (char*)calloc(strlen(argv[optind])+4,1);
        strcpy(prefix,argv[optind]);
    }
    str=(char*)calloc(strlen(prefix)+10,1);
    str2=(char*)calloc(strlen(prefix)+10,1);
    str3=(char*)calloc(strlen(prefix)+10,1);

    {
        //read reference genome
        gzFile fp = xzopen(argv[optind],"r");
        t=clock();
        fprintf(stderr,"[csaa_index] Pack FASTA...");
        l_pac = bns_fasta2bntseq(fp,prefix,0);
        fprintf(stderr,"%2.f sec\n",(float)(clock()-t)/CLOCKS_PER_SEC);
        err_gzclose(fp);
    }
    {
        //construct csa index
        strcpy(str,prefix);strcat(str,".pac");
        strcpy(str2,prefix);strcat(str2,".csa");
        t=clock();
        fprintf(stderr,"[csaa_index] Construct CSA for the packed sequence...\n");
        csa_t* csa = csa_construct(str,prefix);
        csa_dump(csa,str2);
        csa_destroy(csa);
        fprintf(stderr,"%.2f sec \n",(float)(clock()-t)/CLOCKS_PER_SEC);
    }
    {
        //reread the reference genome
        gzFile fp=xzopen(argv[optind],"r");
        t=clock();
        fprintf(stderr,"[csa_index] Pack forward-only FASTA...");
        l_pac=bns_fasta2bntseq(fp,prefix,1);
        fprintf(stderr,"%.2f sec \n",(float)(clock()-t)/CLOCKS_PER_SEC);
        err_gzclose(fp);
    }
    free(str3);
    free(str2);
    free(str);
    free(prefix);
    return 0;
}
