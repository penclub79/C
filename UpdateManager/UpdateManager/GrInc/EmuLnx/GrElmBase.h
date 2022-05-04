/*
	emulation linux module
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrEmlBase
#define	_EPC_GrEmlBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include	<GrDebug.h>
#include	<GrProcess.h>
#include	<EmuLnx/GrEmuLnxBase.h>

//====================================================================
//constance
#define E_GrEmuMdlMaxDevCnt		4
#define E_GrEmuMdlMaxIrqCnt		4

#define E_GrEmuMdlPciRsrcCnt	6

//define const

#define THIS_MODULE		NULL
#define PCI_ANY_ID		0xFFFF
#define KERN_DEBUG
#define printk				DbgMsgPrint
#define HZ						1000

#define VERIFY_READ			0
#define VERIFY_WRITE		1

//irq
#define SA_SHIRQ				0x01
#define IRQF_SHARED			0x01

#define IRQ_NONE				0
#define IRQ_HANDLED			1

//page
#define PAGE_SHIFT      0		/* emulation only */
#define PAGE_SIZE       (1UL << PAGE_SHIFT)
#define PAGE_MASK       (~(PAGE_SIZE-1))

//pci register
#define PCI_VENDOR_ID           0x00    /* 16 bits */
#define PCI_DEVICE_ID           0x02    /* 16 bits */
#define PCI_COMMAND             0x04    /* 16 bits */
#define  PCI_COMMAND_IO         0x1     /* Enable response in I/O space */
#define  PCI_COMMAND_MEMORY     0x2     /* Enable response in Memory space */
#define  PCI_COMMAND_MASTER     0x4     /* Enable bus mastering */
#define  PCI_COMMAND_SPECIAL    0x8     /* Enable response to special cycles */
#define  PCI_COMMAND_INVALIDATE 0x10    /* Use memory write and invalidate */
#define  PCI_COMMAND_VGA_PALETTE 0x20   /* Enable palette snooping */
#define  PCI_COMMAND_PARITY     0x40    /* Enable parity checking */
#define  PCI_COMMAND_WAIT       0x80    /* Enable address/data stepping */
#define  PCI_COMMAND_SERR       0x100   /* Enable SERR */
#define  PCI_COMMAND_FAST_BACK  0x200   /* Enable back-to-back writes */
#define  PCI_COMMAND_INTX_DISABLE 0x400 /* INTx Emulation Disable */

#define PCI_STATUS              0x06    /* 16 bits */
#define  PCI_STATUS_CAP_LIST    0x10    /* Support Capability List */
#define  PCI_STATUS_66MHZ       0x20    /* Support 66 Mhz PCI 2.1 bus */
#define  PCI_STATUS_UDF         0x40    /* Support User Definable Features [obsolete] */
#define  PCI_STATUS_FAST_BACK   0x80    /* Accept fast-back to back */
#define  PCI_STATUS_PARITY      0x100   /* Detected parity error */
#define  PCI_STATUS_DEVSEL_MASK 0x600   /* DEVSEL timing */
#define  PCI_STATUS_DEVSEL_FAST 0x000   
#define  PCI_STATUS_DEVSEL_MEDIUM 0x200
#define  PCI_STATUS_DEVSEL_SLOW 0x400
#define  PCI_STATUS_SIG_TARGET_ABORT 0x800 /* Set on target abort */
#define  PCI_STATUS_REC_TARGET_ABORT 0x1000 /* Master ack of " */
#define  PCI_STATUS_REC_MASTER_ABORT 0x2000 /* Set on master abort */
#define  PCI_STATUS_SIG_SYSTEM_ERROR 0x4000 /* Set when we drive SERR */
#define  PCI_STATUS_DETECTED_PARITY 0x8000 /* Set on parity error */

#define PCI_CLASS_REVISION      0x08    /* High 24 bits are class, low 8
                                           revision */
#define PCI_REVISION_ID         0x08    /* Revision ID */
#define PCI_CLASS_PROG          0x09    /* Reg. Level Programming Interface */
#define PCI_CLASS_DEVICE        0x0a    /* Device class */

#define PCI_CACHE_LINE_SIZE     0x0c    /* 8 bits */
#define PCI_LATENCY_TIMER       0x0d    /* 8 bits */
#define PCI_HEADER_TYPE         0x0e    /* 8 bits */
#define  PCI_HEADER_TYPE_NORMAL 0
#define  PCI_HEADER_TYPE_BRIDGE 1
#define  PCI_HEADER_TYPE_CARDBUS 2

#define PCI_BIST                0x0f    /* 8 bits */
#define  PCI_BIST_CODE_MASK     0x0f    /* Return result */
#define  PCI_BIST_START         0x40    /* 1 to start BIST, 2 secs or less */
#define  PCI_BIST_CAPABLE       0x80    /* 1 if BIST capable */

/*
 * Base addresses specify locations in memory or I/O space.
 * Decoded size can be determined by writing a value of 
 * 0xffffffff to the register, and reading it back.  Only 
 * 1 bits are decoded.
 */
#define PCI_BASE_ADDRESS_0      0x10    /* 32 bits */
#define PCI_BASE_ADDRESS_1      0x14    /* 32 bits [htype 0,1 only] */
#define PCI_BASE_ADDRESS_2      0x18    /* 32 bits [htype 0 only] */
#define PCI_BASE_ADDRESS_3      0x1c    /* 32 bits */
#define PCI_BASE_ADDRESS_4      0x20    /* 32 bits */
#define PCI_BASE_ADDRESS_5      0x24    /* 32 bits */
#define  PCI_BASE_ADDRESS_SPACE 0x01    /* 0 = memory, 1 = I/O */
#define  PCI_BASE_ADDRESS_SPACE_IO 0x01
#define  PCI_BASE_ADDRESS_SPACE_MEMORY 0x00
#define  PCI_BASE_ADDRESS_MEM_TYPE_MASK 0x06
#define  PCI_BASE_ADDRESS_MEM_TYPE_32   0x00    /* 32 bit address */
#define  PCI_BASE_ADDRESS_MEM_TYPE_1M   0x02    /* Below 1M [obsolete] */
#define  PCI_BASE_ADDRESS_MEM_TYPE_64   0x04    /* 64 bit address */
#define  PCI_BASE_ADDRESS_MEM_PREFETCH  0x08    /* prefetchable? */
#define  PCI_BASE_ADDRESS_MEM_MASK      (~0x0fUL)
#define  PCI_BASE_ADDRESS_IO_MASK       (~0x03UL)
/* bit 1 is reserved if address_space = 1 */

#define	VM_IO						0
#define	VM_RESERVED			0
#define	IO_ADDRESS(x)		(x)

// timer
enum hrtimer_restart {
	HRTIMER_NORESTART,	/* Timer is not restarted */
	HRTIMER_RESTART,	/* Timer must be restarted */
};

enum hrtimer_mode {
	HRTIMER_MODE_ABS = 0x0,		/* Time value is absolute */
	HRTIMER_MODE_REL = 0x1,		/* Time value is relative to now */
	HRTIMER_MODE_PINNED = 0x02,	/* Timer is bound to CPU */
	HRTIMER_MODE_ABS_PINNED = 0x02,
	HRTIMER_MODE_REL_PINNED = 0x03,
};

#define CLOCK_REALTIME			0
#define CLOCK_MONOTONIC			1
#define CLOCK_PROCESS_CPUTIME_ID	2
#define CLOCK_THREAD_CPUTIME_ID		3
#define CLOCK_MONOTONIC_RAW		4
#define CLOCK_REALTIME_COARSE		5
#define CLOCK_MONOTONIC_COARSE		6
#define CLOCK_BOOTTIME			7
#define CLOCK_REALTIME_ALARM		8
#define CLOCK_BOOTTIME_ALARM		9

#define I2C_NAME_SIZE	20

//====================================================================
//global macro

#define MAJOR(a)	( (a) >> 16 )
#define access_ok(type,addr,size)		(TRUE)

#define jiffies		GetTickCount()

#define container_of(ptr, type, member) (	\
	(void*)((int)ptr - (int)(&((type*)0)->member)) )

#define I2C_BOARD_INFO(dev_type, dev_addr) \
	.type = dev_type, .addr = (dev_addr)

//====================================================================
//global type

typedef	St_GrPrcCritSct	spinlock_t;
typedef int irqreturn_t;
#ifndef dev_t
#define dev_t		int
#endif

typedef __s64		ktime_t;
typedef int			clockid_t;

//driver

// file operation --------


// i2c -------------
struct i2c_board_info {
	char		type[I2C_NAME_SIZE];
	unsigned short	flags;
	unsigned short	addr;
	void		*platform_data;
	struct dev_archdata	*archdata;
	struct device_node *of_node;
	struct fwnode_handle *fwnode;
	int		irq;
};

// virtual address ----
typedef	struct pt_regs 
{
	DWORD	Eax;
}	*Ptr_pt_regs;

//pci ---------
typedef	struct pci_device_id 
{
	DWORD vendor, device;           /* Vendor and device ID or PCI_ANY_ID*/
	DWORD subvendor, subdevice;     /* Subsystem ID's or PCI_ANY_ID */
	DWORD _class, class_mask;        /* (class,subclass,prog-if) triplet */
	DWORD driver_data;     /* Data private to the driver */
}	*Ptr_pci_device_id;

typedef	struct St_GrEmuMdlPciRsrc 
{
	DWORD		Adr;
	DWORD		Size;
}	*Ptr_GrEmuMdlPciRsrc;

typedef	struct pci_dev 
{
	DWORD	DevIdx;			//device index
	struct St_GrEmuMdlPciRsrc	Rsrc[E_GrEmuMdlPciRsrcCnt];		//resource
	DWORD	irq;
}	*Ptr_pci_dev;

typedef struct pci_driver 
{
	char*	name;
	struct pci_device_id*	id_table;
	int  (*probe)  (struct pci_dev *dev, const struct pci_device_id *id);   /* New device inserted */
	void (*remove)	(struct pci_dev *dev);	/* Device removed (NULL if not a hot-plug capable driver) */
} *Ptr_pci_driver;

//management ----------
typedef	irqreturn_t	(*Fnc_GrEmuMdlIsr)(	int A_Irq, void* A_PtrPara, struct pt_regs * A_PtrReg);

//timer ---------------
typedef struct timer_list 
{
	DWORD		TickRun;
	DWORD		TickStart;
	unsigned long expires;
	void (*function)(unsigned long);
	unsigned long data;
}	*Ptr_timer_list;

typedef struct hrtimer
{
	__u32		node;
	ktime_t				_softexpires;
	enum hrtimer_restart(*function)(struct hrtimer *);
	unsigned long			state;
}	*Ptr_hrtimer;

// semaphore
typedef struct semaphore {
	int		lock;
	unsigned int		count;
	//struct list_head	wait_list;
}	*Ptr_semaphore;


//====================================================================
//class

//====================================================================
//global function

#define pgprot_val
#define pgprot_noncached

//driver
int		alloc_chrdev_region( dev_t* A_PtrDev, int A_MinorStart, int A_MinorCount, char* A_StrName );
void	unregister_chrdev_region(dev_t A_DevNum, unsigned A_Count );
void	cdev_init( struct cdev* A_PtrDev, struct file_operations* A_PtrFop );
int		cdev_add( struct cdev* A_PtrDev, int A_Dnode, int A_Cnt );
void	cdev_del(struct cdev *A_PtrDev );
int		misc_register(struct miscdevice * misc);
int		misc_deregister(struct miscdevice *misc);

//spin lock
void	spin_lock_init( spinlock_t* A_PtrLock );
void	spin_lock_irqsave( spinlock_t* A_PtrLock, DWORD A_Flag );
void	spin_unlock_irqrestore( spinlock_t* A_PtrLock, DWORD A_Flag );
void	spin_lock(spinlock_t* A_PtrLock);
void	spin_unlock(spinlock_t* A_PtrLock);

//mmap
int		remap_pfn_range(struct vm_area_struct *A_PtrVme, DWORD A_From, DWORD A_Pfn, DWORD A_Size, pgprot_t A_Prot );

//resource
void*	request_mem_region( DWORD A_PhysAdr, DWORD A_Size, char* A_StrName );
void	release_mem_region( DWORD A_PhysAdr, DWORD A_Size );
void*	ioremap( DWORD A_PhysAdr, DWORD A_Size );
void*	ioremap_nocache( DWORD A_PhysAdr, DWORD A_Size );
void	iounmap( void* A_Ptr );
int		request_irq(DWORD A_Irq, irqreturn_t (*A_Isr)(int, void *, struct pt_regs *), DWORD A_Flags, char* A_StrName, void* A_UserId );
void	free_irq( DWORD A_Irq, void* A_UserId );

//io
int		copy_from_user( void* A_PtrTg, void* A_PtrUser, DWORD A_Size );
int		copy_to_user( void* A_PtrUser, void* A_PtrSr, DWORD A_Size );
void	local_irq_disable( void );
void	local_irq_enable( void );
void	rmb( void );
void	wmb( void );
DWORD readl( void* A_Ptr );
void	writel( DWORD A_Val, void* A_Ptr );

//pci driver
void	pci_register_driver( struct pci_driver* A_PtrDrv );
void	pci_unregister_driver( struct pci_driver* A_PtrDrv );
void	pci_set_drvdata ( struct pci_dev* A_PtrPciDev, void* A_Val );
void*	pci_get_drvdata ( struct pci_dev* A_PtrPciDev );
int		pci_enable_device( struct pci_dev* A_PtrPciDev );
void	pci_disable_device( struct pci_dev* A_PtrPciDev );
void	pci_set_master( struct pci_dev* A_PtrPciDev );
//pci resource
DWORD	pci_resource_start( struct pci_dev* A_PtrPciDev, DWORD A_BarIdx );
DWORD	pci_resource_len( struct pci_dev* A_PtrPciDev, DWORD A_BarIdx );
int		pci_request_regions( struct pci_dev* A_PtrPciDev, char* A_StrName );
void	pci_release_regions(struct pci_dev* A_PtrPciDev );
//pci configuration
int		pci_write_config_byte(struct pci_dev* A_PtrPciDev, int A_Where, BYTE A_Val );
int		pci_write_config_word(struct pci_dev* A_PtrPciDev, int A_Where, WORD A_Val );
int		pci_write_config_dword(struct pci_dev* A_PtrPciDev, int A_Where, DWORD A_Val );
int		pci_read_config_byte(struct pci_dev* A_PtrPciDev, int A_Where, BYTE* A_PtrRtVal );
int		pci_read_config_word(struct pci_dev* A_PtrPciDev, int A_Where, WORD* A_PtrRtVal );
int		pci_read_config_dword(struct pci_dev* A_PtrPciDev, int A_Where, DWORD* A_PtrRtVal );
//timer
void	init_timer(struct timer_list* A_PtrTimePara );
void	add_timer(struct timer_list* A_PtrTimePara );
void	del_timer(struct timer_list* A_PtrTimePara );
void	mdelay( DWORD A_Ms );
ktime_t ktime_set(const long secs, const unsigned long nsecs);
void hrtimer_init(struct hrtimer *timer, clockid_t which_clock,enum hrtimer_mode mode);
int hrtimer_start(struct hrtimer *timer, ktime_t tim,	const enum hrtimer_mode mode);
int hrtimer_cancel(struct hrtimer *timer);

// semaphore
void sema_init(struct semaphore *sem, int val);
void down(struct semaphore *sem);
void up(struct semaphore *sem);

//emulation runtime
void	GrEmuMdlInit( void );
void	GrEmuMdlFinish( void );
void	GrEmuMdlIrqOccure( BYTE A_Irq );
int		GrEmuMdlGetEmptyDevIdx( void );

struct i2c_adapter* i2c_get_adapter(int nr);
void i2c_put_adapter(struct i2c_adapter *adap);
struct i2c_client* i2c_new_device(struct i2c_adapter *adap, struct i2c_board_info const *info);
void i2c_unregister_device(struct i2c_client *client);
int i2c_master_send(const struct i2c_client *client, const char *buf, int count);
int i2c_master_recv(const struct i2c_client *client, char *buf, int count);

//====================================================================
//out Enviroment conditional
#endif
