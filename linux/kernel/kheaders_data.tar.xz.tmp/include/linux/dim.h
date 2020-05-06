/* SPDX-License-Identifier: GPL-2.0 OR Linux-OpenIB */


#ifndef DIM_H
#define DIM_H

#include <linux/module.h>


#define DIM_NEVENTS 64


#define IS_SIGNIFICANT_DIFF(val, ref) \
	(((100UL * abs((val) - (ref))) / (ref)) > 10)


#define BIT_GAP(bits, end, start) ((((end) - (start)) + BIT_ULL(bits)) \
		& (BIT_ULL(bits) - 1))


struct dim_cq_moder {
	u16 usec;
	u16 pkts;
	u16 comps;
	u8 cq_period_mode;
};


struct dim_sample {
	ktime_t time;
	u32 pkt_ctr;
	u32 byte_ctr;
	u16 event_ctr;
	u32 comp_ctr;
};


struct dim_stats {
	int ppms; 
	int bpms; 
	int epms; 
	int cpms; 
	int cpe_ratio; 
};


struct dim {
	u8 state;
	struct dim_stats prev_stats;
	struct dim_sample start_sample;
	struct dim_sample measuring_sample;
	struct work_struct work;
	void *priv;
	u8 profile_ix;
	u8 mode;
	u8 tune_state;
	u8 steps_right;
	u8 steps_left;
	u8 tired;
};


enum {
	DIM_CQ_PERIOD_MODE_START_FROM_EQE = 0x0,
	DIM_CQ_PERIOD_MODE_START_FROM_CQE = 0x1,
	DIM_CQ_PERIOD_NUM_MODES
};


enum {
	DIM_START_MEASURE,
	DIM_MEASURE_IN_PROGRESS,
	DIM_APPLY_NEW_PROFILE,
};


enum {
	DIM_PARKING_ON_TOP,
	DIM_PARKING_TIRED,
	DIM_GOING_RIGHT,
	DIM_GOING_LEFT,
};


enum {
	DIM_STATS_WORSE,
	DIM_STATS_SAME,
	DIM_STATS_BETTER,
};


enum {
	DIM_STEPPED,
	DIM_TOO_TIRED,
	DIM_ON_EDGE,
};


bool dim_on_top(struct dim *dim);


void dim_turn(struct dim *dim);


void dim_park_on_top(struct dim *dim);


void dim_park_tired(struct dim *dim);


void dim_calc_stats(struct dim_sample *start, struct dim_sample *end,
		    struct dim_stats *curr_stats);


static inline void
dim_update_sample(u16 event_ctr, u64 packets, u64 bytes, struct dim_sample *s)
{
	s->time	     = ktime_get();
	s->pkt_ctr   = packets;
	s->byte_ctr  = bytes;
	s->event_ctr = event_ctr;
}


static inline void
dim_update_sample_with_comps(u16 event_ctr, u64 packets, u64 bytes, u64 comps,
			     struct dim_sample *s)
{
	dim_update_sample(event_ctr, packets, bytes, s);
	s->comp_ctr = comps;
}




struct dim_cq_moder net_dim_get_rx_moderation(u8 cq_period_mode, int ix);


struct dim_cq_moder net_dim_get_def_rx_moderation(u8 cq_period_mode);


struct dim_cq_moder net_dim_get_tx_moderation(u8 cq_period_mode, int ix);


struct dim_cq_moder net_dim_get_def_tx_moderation(u8 cq_period_mode);


void net_dim(struct dim *dim, struct dim_sample end_sample);




#define RDMA_DIM_PARAMS_NUM_PROFILES 9
#define RDMA_DIM_START_PROFILE 0


void rdma_dim(struct dim *dim, u64 completions);

#endif 
