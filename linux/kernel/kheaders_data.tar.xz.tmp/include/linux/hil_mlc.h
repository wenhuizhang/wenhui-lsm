

#include <linux/hil.h>
#include <linux/time.h>
#include <linux/interrupt.h>
#include <linux/semaphore.h>
#include <linux/serio.h>
#include <linux/list.h>

typedef struct hil_mlc hil_mlc;


enum hilse_act {
  	
	HILSE_OUT = 0,

  	
	HILSE_CTS,

	
	HILSE_OUT_LAST,

	
	HILSE_OUT_DISC,

	
	HILSE_FUNC,

  	
	HILSE_IN		= 0x100,

  	
	HILSE_EXPECT,

  	
	HILSE_EXPECT_LAST,

  	
	HILSE_EXPECT_DISC
};

typedef int	(hilse_func) (hil_mlc *mlc, int arg);
struct hilse_node {
	enum hilse_act		act;	
	union {
		hilse_func	*func;	
		hil_packet	packet;	
	} object;
	int			arg;	
	int			good;	
	int			bad;	
	int			ugly;	
};


typedef int	(hil_mlc_cts) (hil_mlc *mlc);
typedef void	(hil_mlc_out) (hil_mlc *mlc);
typedef int	(hil_mlc_in)  (hil_mlc *mlc, suseconds_t timeout);

struct hil_mlc_devinfo {
	uint8_t	idd[16];	
	uint8_t	rsc[16];	
	uint8_t	exd[16];	
	uint8_t	rnm[16];	
};

struct hil_mlc_serio_map {
	hil_mlc *mlc;
	int di_revmap;
	int didx;
};


#define HIL_MLC_DEVMEM 16

struct hil_mlc {
	struct list_head	list;	

	rwlock_t		lock;

	void *priv; 

	int 			seidx;	
	int			istarted, ostarted;

	hil_mlc_cts		*cts;
	struct semaphore	csem;   

	hil_mlc_out		*out;
	struct semaphore	osem;   
	hil_packet		opacket;

	hil_mlc_in		*in;
	struct semaphore	isem;   
	hil_packet		ipacket[16];
	hil_packet		imatch;
	int			icount;
	unsigned long		instart;
	unsigned long		intimeout;

	int			ddi;	
	int			lcv;	
	time64_t		lcv_time; 

	int			di_map[7]; 
	struct hil_mlc_devinfo	di[HIL_MLC_DEVMEM];
	struct serio		*serio[HIL_MLC_DEVMEM];
	struct hil_mlc_serio_map serio_map[HIL_MLC_DEVMEM];
	hil_packet		serio_opacket[HIL_MLC_DEVMEM];
	int			serio_oidx[HIL_MLC_DEVMEM];
	struct hil_mlc_devinfo	di_scratch; 

	int			opercnt;

	struct tasklet_struct	*tasklet;
};

int hil_mlc_register(hil_mlc *mlc);
int hil_mlc_unregister(hil_mlc *mlc);
