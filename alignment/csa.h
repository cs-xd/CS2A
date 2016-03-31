#ifndef CSA_C_H
#define CSA_C_H
#include "../csa/CSA.h"

typedef struct{
    CSA* csa;
    int64_t* sa;
}csa_t;

//typedef uint64_t u64_t;
typedef unsigned long long int u64_t;
typedef unsigned char ubyte_t;

//构建csa索引
csa_t* csa_construct_core(ubyte_t* T,int64_t seq_len,char* prefix);
//存储csa索引到文件
int64_t csa_dump(csa_t* csa,char* csafile);
//从文件读取csa
csa_t* csa_restore(char* csafile);
//西够csa
int64_t csa_destroy(csa_t* csa);
//精确匹配
int64_t csa_match_exact_alt(csa_t *csa, u64_t len, const ubyte_t *str, u64_t *k0, u64_t *l0);
//csa查找sa
int64_t csa_sa(const csa_t* csa,u64_t pos);
//恢复sa
void csa_restore_sa(csa_t* csa,char* safile);
#endif
