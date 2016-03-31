#ifndef SAM_H
#define SAM_H

#include <stdint.h>
#include "../seqio/bntseq.h"
#include "../alignment/csa.h"

#define CSAA_IDX_CSA 0x1
#define CSAA_IDX_BNS 0x2
#define CSAA_IDX_PAC 0x4
#define CSAA_IDX_ALL 0x7

typedef struct {
	csa_t    *csa; // csa-index
	bntseq_t *bns; // information on the reference sequences
	uint8_t  *pac; // the actual 2-bit encoded reference sequences with 'N' converted to a random base
} csaaidx_t;

typedef struct {
	int l_seq;
	char *name, *comment, *seq, *qual, *sam;
} bseq1_t;

extern int csaa_verbose;
extern char csaa_rg_id[256];

#ifdef __cplusplus
extern "C" {
#endif

	bseq1_t *bseq_read(int chunk_size, int *n_, void *ks1_, void *ks2_);
	void csaa_fill_scmat(int a, int b, int8_t mat[25]);
	uint32_t *csaa_gen_cigar(const int8_t mat[25], int q, int r, int w_, int64_t l_pac, const uint8_t *pac, int l_query, uint8_t *query, int64_t rb, int64_t re, int *score, int *n_cigar, int *NM);
	uint32_t *csaa_gen_cigar2(const int8_t mat[25], int o_del, int e_del, int o_ins, int e_ins, int w_, int64_t l_pac, const uint8_t *pac, int l_query, uint8_t *query, int64_t rb, int64_t re, int *score, int *n_cigar, int *NM);

	char *csaa_idx_infer_prefix(const char *hint);
	csa_t *csaa_idx_load_csa(const char *hint);

	csaaidx_t *csaa_idx_load(const char *hint, int which);
	void csaa_idx_destroy(csaaidx_t *idx);

	void csaa_print_sam_hdr(const bntseq_t *bns, const char *rg_line);
	char *csaa_set_rg(const char *s);

#ifdef __cplusplus
}
#endif

#endif
