

#ifndef _DVB_RINGBUFFER_H_
#define _DVB_RINGBUFFER_H_

#include <linux/spinlock.h>
#include <linux/wait.h>


struct dvb_ringbuffer {
	u8               *data;
	ssize_t           size;
	ssize_t           pread;
	ssize_t           pwrite;
	int               error;

	wait_queue_head_t queue;
	spinlock_t        lock;
};

#define DVB_RINGBUFFER_PKTHDRSIZE 3


extern void dvb_ringbuffer_init(struct dvb_ringbuffer *rbuf, void *data,
				size_t len);


extern int dvb_ringbuffer_empty(struct dvb_ringbuffer *rbuf);


extern ssize_t dvb_ringbuffer_free(struct dvb_ringbuffer *rbuf);


extern ssize_t dvb_ringbuffer_avail(struct dvb_ringbuffer *rbuf);


extern void dvb_ringbuffer_reset(struct dvb_ringbuffer *rbuf);




extern void dvb_ringbuffer_flush(struct dvb_ringbuffer *rbuf);


extern void dvb_ringbuffer_flush_spinlock_wakeup(struct dvb_ringbuffer *rbuf);


#define DVB_RINGBUFFER_PEEK(rbuf, offs)	\
			((rbuf)->data[((rbuf)->pread + (offs)) % (rbuf)->size])


#define DVB_RINGBUFFER_SKIP(rbuf, num)	{\
			(rbuf)->pread = ((rbuf)->pread + (num)) % (rbuf)->size;\
}


extern ssize_t dvb_ringbuffer_read_user(struct dvb_ringbuffer *rbuf,
				   u8 __user *buf, size_t len);


extern void dvb_ringbuffer_read(struct dvb_ringbuffer *rbuf,
				   u8 *buf, size_t len);




#define DVB_RINGBUFFER_WRITE_BYTE(rbuf, byte)	\
			{ (rbuf)->data[(rbuf)->pwrite] = (byte); \
			(rbuf)->pwrite = ((rbuf)->pwrite + 1) % (rbuf)->size; }


extern ssize_t dvb_ringbuffer_write(struct dvb_ringbuffer *rbuf, const u8 *buf,
				    size_t len);


extern ssize_t dvb_ringbuffer_write_user(struct dvb_ringbuffer *rbuf,
					 const u8 __user *buf, size_t len);


extern ssize_t dvb_ringbuffer_pkt_write(struct dvb_ringbuffer *rbuf, u8 *buf,
					size_t len);


extern ssize_t dvb_ringbuffer_pkt_read_user(struct dvb_ringbuffer *rbuf,
					    size_t idx,
					    int offset, u8 __user *buf,
					    size_t len);


extern ssize_t dvb_ringbuffer_pkt_read(struct dvb_ringbuffer *rbuf, size_t idx,
				       int offset, u8 *buf, size_t len);


extern void dvb_ringbuffer_pkt_dispose(struct dvb_ringbuffer *rbuf, size_t idx);


extern ssize_t dvb_ringbuffer_pkt_next(struct dvb_ringbuffer *rbuf,
				       size_t idx, size_t *pktlen);

#endif 
