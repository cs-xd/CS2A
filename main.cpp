#include <iostream>
#include <string.h>
#include <unistd.h>
#include "index/csaindex.h"
#include "alignment/alignment.h"

using namespace std;

extern int csaa_aln(int argc, char *argv[]);
extern int csaa_sai2sam_se(int argc, char *argv[]);
extern int csaa_sai2sam_pe(int argc, char *argv[]);

void print_help()
{
    cerr<<"\n";
    cerr<<"\tindex \tindex sequences in the FASTA format\n";
    cerr<<"\taln \talign the short reads to ref sequences\n";
    cerr<<"\tse \tsingle end sai to sam\n";
    cerr<<"\tpe \tpair end sai to sam\n";
    cerr<<"\t-h print this help\n\n";
}

int main(int argc,char* argv[])
{
    if(argc<2)
    {
        print_help();
        return 0;
    }
    if(!strcmp(argv[1],"index"))
    {
        csa_index(argc-1,argv+1);
        return 0;
    }
    else if(!strcmp(argv[1],"aln"))
    {
        csaa_aln(argc-1,argv+1);
        return 0;
    }
    else if(!strcmp(argv[1],"se"))
    {
        csaa_sai2sam_se(argc-1, argv+1);
        return 0;
    }
    else if(!strcmp(argv[1],"pe"))
    {
        csaa_sai2sam_pe(argc-1,argv+1);
        return 0;
    }
    else
    {
        print_help();
        return 0;
    }
}

