#ifndef SINGLEEND_H
#define SINGLEEND_H
#include "../seqio/bntseq.h"
#include "../alignment/csa.h"
#include "../alignment/alignment.h"

#ifdef __cplusplus
extern "C" {
#endif

    u64_t csaa_sa2pos(const bntseq_t *bns, const csa_t *csa, u64_t sapos, int len, int *strand);

	void csaase_initialize();
	// Calculate the approximate position of the sequence from the specified  suffix array.
	void csaa_cal_pac_pos_core(const bntseq_t *bns, const csa_t* csa, csaa_seq_t* seq, const int max_mm, const float fnr);
	// Refine the approximate position of the sequence to an actual placement for the sequence.
	void csaa_refine_gapped(const bntseq_t *bns, int n_seqs, csaa_seq_t *seqs, ubyte_t *_pacseq);
	// Backfill certain alignment properties mainly centering around number of matches.
	void csaa_aln2seq(int n_aln, const csa_aln1_t *aln, csaa_seq_t *s);
	// Calculate the end position of a read given a certain sequence.
	int64_t pos_end(const csaa_seq_t *p);

#ifdef __cplusplus
}
#endif

#endif
