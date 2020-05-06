/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _LINUX_NFS_IOSTAT
#define _LINUX_NFS_IOSTAT

#define NFS_IOSTAT_VERS		"1.1"


enum nfs_stat_bytecounters {
	NFSIOS_NORMALREADBYTES = 0,
	NFSIOS_NORMALWRITTENBYTES,
	NFSIOS_DIRECTREADBYTES,
	NFSIOS_DIRECTWRITTENBYTES,
	NFSIOS_SERVERREADBYTES,
	NFSIOS_SERVERWRITTENBYTES,
	NFSIOS_READPAGES,
	NFSIOS_WRITEPAGES,
	__NFSIOS_BYTESMAX,
};


enum nfs_stat_eventcounters {
	NFSIOS_INODEREVALIDATE = 0,
	NFSIOS_DENTRYREVALIDATE,
	NFSIOS_DATAINVALIDATE,
	NFSIOS_ATTRINVALIDATE,
	NFSIOS_VFSOPEN,
	NFSIOS_VFSLOOKUP,
	NFSIOS_VFSACCESS,
	NFSIOS_VFSUPDATEPAGE,
	NFSIOS_VFSREADPAGE,
	NFSIOS_VFSREADPAGES,
	NFSIOS_VFSWRITEPAGE,
	NFSIOS_VFSWRITEPAGES,
	NFSIOS_VFSGETDENTS,
	NFSIOS_VFSSETATTR,
	NFSIOS_VFSFLUSH,
	NFSIOS_VFSFSYNC,
	NFSIOS_VFSLOCK,
	NFSIOS_VFSRELEASE,
	NFSIOS_CONGESTIONWAIT,
	NFSIOS_SETATTRTRUNC,
	NFSIOS_EXTENDWRITE,
	NFSIOS_SILLYRENAME,
	NFSIOS_SHORTREAD,
	NFSIOS_SHORTWRITE,
	NFSIOS_DELAY,
	NFSIOS_PNFS_READ,
	NFSIOS_PNFS_WRITE,
	__NFSIOS_COUNTSMAX,
};


enum nfs_stat_fscachecounters {
	NFSIOS_FSCACHE_PAGES_READ_OK,
	NFSIOS_FSCACHE_PAGES_READ_FAIL,
	NFSIOS_FSCACHE_PAGES_WRITTEN_OK,
	NFSIOS_FSCACHE_PAGES_WRITTEN_FAIL,
	NFSIOS_FSCACHE_PAGES_UNCACHED,
	__NFSIOS_FSCACHEMAX,
};

#endif	