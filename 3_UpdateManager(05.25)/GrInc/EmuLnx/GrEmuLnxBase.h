/*
	emulation linux common base
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrEmuLnxBase
#define	_EPC_GrEmuLnxBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include	<GrDebug.h>
#include	<GrProcess.h>
//====================================================================
//const

#define E_GrEmuDispMaxCnt			2			/* max display device count */

#define E_GrEmuMdlMaxDrvCnt		8


//error number
#define EAGAIN								11
#define EINVAL								22

// misc
#define MISC_DYNAMIC_MINOR		255

//ioctl
#define _IOC_NRBITS						8
#define _IOC_TYPEBITS					8
#define _IOC_SIZEBITS					14
#define _IOC_DIRBITS					2

#define _IOC_NRMASK						((1 << _IOC_NRBITS)-1)
#define _IOC_TYPEMASK					((1 << _IOC_TYPEBITS)-1)
#define _IOC_SIZEMASK					((1 << _IOC_SIZEBITS)-1)
#define _IOC_DIRMASK					((1 << _IOC_DIRBITS)-1)

#define _IOC_NRSHIFT					0
#define _IOC_TYPESHIFT				(_IOC_NRSHIFT+_IOC_NRBITS)
#define _IOC_SIZESHIFT				(_IOC_TYPESHIFT+_IOC_TYPEBITS)
#define _IOC_DIRSHIFT					(_IOC_SIZESHIFT+_IOC_SIZEBITS)

/*
* Direction bits.
*/
#define _IOC_NONE							0U
#define _IOC_WRITE						1U
#define _IOC_READ							2U
	 
#define _IOC(dir,type,nr,size) \
				(((dir)  << _IOC_DIRSHIFT) | \
				((type) << _IOC_TYPESHIFT) | \
				((nr)   << _IOC_NRSHIFT) | \
				((size) << _IOC_SIZESHIFT))
	 
/* provoke compile error for invalid uses of size argument */
#define _IOC_TYPECHECK(t)	(sizeof(t))

/* used to create numbers */
#define _LEMU_IO(type,nr)            _IOC(_IOC_NONE,(type),(nr),0)
#define _LEMU_IOR(type,nr,size)      _IOC(_IOC_READ,(type),(nr),(_IOC_TYPECHECK(size)))
#define _LEMU_IOW(type,nr,size)      _IOC(_IOC_WRITE,(type),(nr),(_IOC_TYPECHECK(size)))
#define _LEMU_IOWR(type,nr,size)     _IOC(_IOC_READ|_IOC_WRITE,(type),(nr),(_IOC_TYPECHECK(size)))
#define _IOR_BAD(type,nr,size)  _IOC(_IOC_READ,(type),(nr),sizeof(size))
#define _IOW_BAD(type,nr,size)  _IOC(_IOC_WRITE,(type),(nr),sizeof(size))
#define _IOWR_BAD(type,nr,size) _IOC(_IOC_READ|_IOC_WRITE,(type),(nr),sizeof(size))

/* used to decode ioctl numbers.. */
#define _IOC_DIR(nr)            (((nr) >> _IOC_DIRSHIFT) & _IOC_DIRMASK)
#define _IOC_TYPE(nr)           (((nr) >> _IOC_TYPESHIFT) & _IOC_TYPEMASK)
#define _IOC_NR(nr)             (((nr) >> _IOC_NRSHIFT) & _IOC_NRMASK)
#define _IOC_SIZE(nr)           (((nr) >> _IOC_SIZESHIFT) & _IOC_SIZEMASK)

/* ...and for the drivers/sound files... */
//#define IOC_IN          (_IOC_WRITE << _IOC_DIRSHIFT)
//#define IOC_OUT         (_IOC_READ << _IOC_DIRSHIFT)
//#define IOC_INOUT       ((_IOC_WRITE|_IOC_READ) << _IOC_DIRSHIFT)
#define IOCSIZE_MASK    (_IOC_SIZEMASK << _IOC_SIZESHIFT)
#define IOCSIZE_SHIFT   (_IOC_SIZESHIFT)

//input
#define EV_KEY                  0x01
#define EV_REL                  0x02
#define EV_ABS                  0x03

#define BTN_LEFT                0x110
#define BTN_RIGHT               0x111
#define BTN_MIDDLE              0x112

#define REL_X                   0x00
#define REL_Y                   0x01
#define REL_WHEEL               0x08

//====================================================================
//global macro


//====================================================================
//global type

#ifndef _INC_WCHAR
typedef	DWORD	off_t;
#endif

typedef	DWORD	pgprot_t;

typedef	struct file_operations 
{	
	void*	owner;
	int (*ioctl) (struct inode *, struct file *, DWORD, DWORD);
	int (*mmap) (struct file *, struct vm_area_struct *);
	int (*open) (struct inode *, struct file *);
	int (*release) (struct inode *, struct file *);
	long (*unlocked_ioctl) (struct file*, unsigned int, unsigned long);
}	*Ptr_file_operations;

typedef	struct miscdevice 
{	
	int	minor;
	char *name;
	struct file_operations *fops;
	//struct list_head list;
	//struct device *parent;
	//struct device *this_device;
	char *nodename;
	unsigned short mode;
}	*Ptr_miscdevice;

typedef struct cdev
{
	int		Node;
	void*	owner;
	struct file_operations* ops;
}	*Ptr_cdev;

typedef	struct inode
{
	int		Node;
	struct  cdev*	i_cdev;
}	*Ptr_inode;

struct dentry {
	//int d_count;
	//unsigned int d_flags;

	struct inode  * d_inode;	/* Where the name belongs to - NULL is negative */
#if 0
	struct dentry * d_parent;	/* parent directory */
	struct dentry * d_mounts;	/* mount information */
	struct dentry * d_covers;
	struct list_head d_hash;	/* lookup hash list */
	struct list_head d_lru;		/* d_count = 0 LRU list */
	struct list_head d_child;	/* child of parent list */
	struct list_head d_subdirs;	/* our children */
	struct list_head d_alias;	/* inode alias list */
	struct qstr d_name;
	unsigned long d_time;		/* used by d_revalidate */
	struct dentry_operations  *d_op;
	struct super_block * d_sb;	/* The root of the dentry tree */
	unsigned long d_reftime;	/* last time referenced */
	void * d_fsdata;		/* fs-specific data */
	unsigned char d_iname[DNAME_INLINE_LEN]; /* small names */
#endif
};

struct path {
	//struct vfsmount *mnt;
	struct dentry *dentry;
};

typedef	struct file 
{
	int		Handle;
	struct path		f_path;
	void*	private_data;
}	*Ptr_file;

typedef	struct St_GrEmuMdlDrv 
{
	Def_StrName	StrName;
	BOOL	IsUse;
	struct file_operations*	PtrFo;
	//struct pci_driver	PciDrv;

}	*Ptr_GrEmuMdlDrv;

typedef	struct vm_area_struct 
{
	//struct mm_struct * vm_mm;       /* The address space we belong to. */
	DWORD	vm_start;         /* Our start address within vm_mm. */
	DWORD	vm_end;           /* The first byte after our end address within vm_mm. */

	/* linked list of VM areas per task, sorted by address */
	struct vm_area_struct *vm_next;

	pgprot_t vm_page_prot;          /* Access permissions of this VMA. */
	DWORD	vm_flags;         /* Flags, listed below. */

	//struct rb_node vm_rb;
	/*
	* For areas with an address space and backing store,
	* linkage into the address_space->i_mmap prio tree, or
	* linkage to the list of like vmas hanging off its node, or
	* linkage of vma in the address_space->i_mmap_nonlinear list.
	*/
	union {
		struct {
			//struct list_head list;
			void*	parent;   /* aligns with prio_tree_node parent */
			struct vm_area_struct *head;
		} vm_set;

		//struct raw_prio_tree_node prio_tree_node;
	} shared;

	/*
	* A file's MAP_PRIVATE vma can be in both i_mmap tree and anon_vma
	* list, after a COW of one of the file pages.  A MAP_SHARED vma
	* can only be in the i_mmap tree.  An anonymous MAP_PRIVATE, stack
	* or brk vma (with NULL file) can only be in an anon_vma list.
	*/
	//struct list_head anon_vma_node; /* Serialized by anon_vma->lock */
	//struct anon_vma*	anon_vma;      /* Serialized by page_table_lock */

	/* Function pointers to deal with this struct. */
	//struct vm_operations_struct * vm_ops;

	/* Information about our backing store: */
	DWORD vm_pgoff;         /* Offset (within vm_file) in PAGE_SIZE
													units, *not* PAGE_CACHE_SIZE */
	//struct file * vm_file;          /* File we map to (can be NULL). */
	void* vm_private_data;         /* was vm_pte (shared mem) */
	unsigned long vm_truncate_count;/* truncate_count or restart_addr */
	void*	MapAdr;		// emulation mapping address
}	*Ptr_vm_area_struct;

typedef	struct input_event {
	//struct timeval time;
	WORD type;
	WORD code;
	int		value;
}	*Ptr_input_event;


//====================================================================
//class

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif
