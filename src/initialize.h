/**
 * @file initialize.h
 * @author Karin S. Dorman
 * @author Xiyu Peng
 *
 * Header file for initialization structs, defines and extern functions
 */

#ifndef __H_INITIALIZE__
#define __H_INITIALIZE__

#include "model.h"
#include "data.h"
#include "options.h"

/**
 * Initialization methods.  See kmodes.h for k-modes initialization methods.
 */
enum {
	INIT_PARTITION,			/*<! init. w/ partition */
	INIT_HAPLOTYPES,		/*<! init. w/ haplotypes */
	INIT_HAPLOTYPES_AND_PARTITION,	/*<! init. w/ both */
	INIT_SEEDS,			/*<! init. w/ selected observations */
	INIT_PARAMETERS,		/*<! init. w/ parameters */
	NUM_INIT_METHODS
};

/**
 * Run methods.
 */
enum {
	EXHAUSTIVE_EM,	/*<! run EM to convergence on each initialization */
	/* the rest do multiple random initializations and run EM on best */
	INI_EM,		/*<! use criterion-based initializer */
	RND_EM,		/*<! use ll-evaluated random initializations */
	EM_EM,		/*<! use ll after n EM iterations */
	NUM_RUN_METHODS
};

typedef struct _initializer initializer;

struct _initializer {
	unsigned int K;			/*<! number of clusters */

	/* current initialization */
	size_t *seed_idx;		/*<! seed indices */
	data_t **seeds;			/*<! access to seed data */
	unsigned int *seed_lengths;	/*<! seed lengths */
	unsigned int *cluster_id;	/*<! cluster assignments  */
	double *criterion;		/*<! criterion per cluster  */
	unsigned int *cluster_size;	/*<! cluster sizes  */

	/* sequence table with abundance (sorted) for AmpliCI */
	size_t *uniq_seq_idx;   /*<! unique sequences index in dmat */
	unsigned int *reads_uniq_id;  /*<! index of each read in unique sequence table */
	unsigned int *uniq_seq_count; /*<! abundance of unique sequences */
	double *abun_true;     /*<! estimated true abundance for each uniq seq*/
	double *p;            /*<!  probability of being chosen as hap for each uniq seq */
	unsigned int *H;     /*<! idx of haplotypes in uniq seq table */
	double *H_ee;       /*<! mean expected number of errors of selected haplotypes */
	double *H_abun;  /*<! relative true abundance of selected haplotypes */
	double *H_pvalue;  /*<! p value of selected haplotypes */  
	double *e_trans;	/*<! n*K expected transition probability  */
	double *self_trans;  /*<! self transition probability */
	unsigned int *nw_mismatch;  /*<! n_candidate * K number of mismatch from nw alignment results */
	unsigned int *nw_indels;  /*<! n_candidate * K number of indels from nw alignment results */

	/* error estimation */
	unsigned int * err_cnt;  /*<! final error count matrix for estimating error profiles */

	/* optimal initialization criterion across repeated initialization */
	double optimal_total;		/*<! optimal criterion across inits */
}; /* initializer */


int make_initializer(initializer **ini, data *dat, options *opt,fastq_data *fqdf);
int sync_initializer(initializer *ini, data *dat);
int realloc_initializer(initializer *ini, data *dat, options *opt);
void free_initializer(initializer *ini, options *opt);
int read_initialization_file(char const * const filename, data *dat,
	options *opt, fastq_data **fqdf);

#endif
