/*
	emulation linux module 

*/

//====================================================================
//include

#include	<EmuLnx/GrElmBase.h>
#include	<EmuLnx/GrElmDevBase.h>

#if (defined EMU_SOLO_6010) || (defined EMU_SOLO_6110)
#include	<Hw/Solo/RegSolo.h>
#include	<EmuLnx/GrElmDevSolo.h>
#endif

#include	<GrDumyTool.h>
#include	<GrBufTool.h>
#include	<GrStrTool.h>

//====================================================================
//local const

#define	E_GrEmuMdlTimerListQueSize		0x1000

//====================================================================
//local type

typedef	struct St_GrEmuMldIrqSlot 
{
	Fnc_GrEmuMdlIsr	Isr;
	DWORD		Id;
}	*Ptr_GrEmuMldIrqSlot;


//====================================================================
//global var
St_GrEmuMdlDrv	V_GrEmuDrvTbl[E_GrEmuMdlMaxDrvCnt]	=	
{	
	{FALSE, NULL },	{FALSE, NULL },			
	{FALSE, NULL},	{FALSE, NULL }
};

Cls_GrElmDevBase*	V_GrEmuDevTbl[E_GrEmuMdlMaxDevCnt]	=	
{	NULL, NULL, NULL, NULL };

St_GrEmuMldIrqSlot	V_GrEmuIsrTbl[E_GrEmuMdlMaxIrqCnt]	=	
{	{NULL, 0},	{NULL, 0},	{NULL, 0},	{NULL, 0}	};

St_GrPrcCritSct		V_GrEmuMdlIrqCrti;

St_GrPrcCritSct		V_GrEmuMdlTimerCrti;
UINT_PTR	V_GrEmuMdlTimerId; 
BYTE	V_GrEmuMdlTimerQue[E_GrEmuMdlTimerListQueSize];

//====================================================================
//functions
VOID CALLBACK GrEmuMdlTimerProc( HWND A_Hndl, UINT A_Msg, UINT_PTR A_IdPara, DWORD A_Time );

//====================================================================
//--------------------------------------------------------------------
void*	request_mem_region( DWORD A_PhysAdr, DWORD A_Size, char* A_StrName )
{
		return	(void*)-1;
}
//--------------------------------------------------------------------
void*	ioremap( DWORD A_PhysAdr, DWORD A_Size )
{
		return	(void*)A_PhysAdr;
}
//--------------------------------------------------------------------
void*	ioremap_nocache( DWORD A_PhysAdr, DWORD A_Size )
{
		return	(void*)A_PhysAdr;
}
//--------------------------------------------------------------------
int		alloc_chrdev_region( dev_t* A_PtrDev, int A_MinorStart, int A_MinorCount, char* A_StrName )
{
	// local -------------------
		int		Tv_Result;
		int		Tv_WkIdx;
	// code --------------------
		Tv_Result	=	-1;
		//check able slot
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrEmuMdlMaxDrvCnt;Tv_WkIdx++ )
		{
			//check able
			if ( !V_GrEmuDrvTbl[Tv_WkIdx].IsUse )
			{
				//success
				*A_PtrDev	=	(Tv_WkIdx << 16) | A_MinorStart;
				V_GrEmuDrvTbl[Tv_WkIdx].IsUse	=	TRUE;
				GrStrCopy( V_GrEmuDrvTbl[Tv_WkIdx].StrName, A_StrName );
				Tv_Result	=	0;
				break;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	cdev_init( struct cdev* A_PtrDev, struct file_operations* A_PtrFop )
{
		memset( A_PtrDev, 0, sizeof(struct cdev) );
		A_PtrDev->ops		=	A_PtrFop;
		A_PtrDev->Node	=	-1;

}
//--------------------------------------------------------------------
int		cdev_add( struct cdev* A_PtrDev, int A_Dnode, int A_Cnt )
{
	// local -------------------
		int		Tv_Result;
		int		Tv_Node;
	// code --------------------
		//init
		Tv_Result	=	-1;

		//check able node
		Tv_Node	=	MAJOR(A_Dnode);
		//check able handle table
		if ( V_GrEmuDrvTbl[Tv_Node].IsUse )
		{
			//intialize table update table
			V_GrEmuDrvTbl[Tv_Node].PtrFo	=	A_PtrDev->ops;
			//success
			A_PtrDev->Node	=	Tv_Node;
			Tv_Result	=	0;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int		GrEmuMdlGetEmptyDevIdx( void )
{
	// local -------------------
		int		Tv_Result;
		int		Tv_WkIdx;
	// code --------------------
		Tv_Result	=	-1;

		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrEmuMdlMaxDevCnt;Tv_WkIdx++ )
		{
			if ( NULL == V_GrEmuDevTbl[Tv_WkIdx] )
			{
				//finded
				Tv_Result	=	Tv_WkIdx;
				break;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	pci_register_driver( struct pci_driver* A_PtrDrv )
{
	// local -------------------
		DWORD		Tv_IdtIdx;			//id table index
		int			Tv_DevIdx;
		Ptr_pci_dev	Tv_PtrPciDev;
		BOOL		Tv_Is6110;
	// code --------------------
		//find device
		Tv_IdtIdx	=	0;

		while ( 0 != A_PtrDrv->id_table[Tv_IdtIdx].vendor )
		{
#if (defined EMU_SOLO_6010) || (defined EMU_SOLO_6110)
			//soft logic
			if ( E_SoloVid == A_PtrDrv->id_table[Tv_IdtIdx].vendor )
			{
				//check device
#ifdef EMU_SOLO_6110
				if ( E_SoloPid6110 == A_PtrDrv->id_table[Tv_IdtIdx].device )
#else
				if ( E_SoloPid6010 == A_PtrDrv->id_table[Tv_IdtIdx].device )
#endif
				{
#ifdef EMU_SOLO_6110
					Tv_Is6110	=	TRUE;
#else
					Tv_Is6110	=	FALSE;
#endif
					Tv_DevIdx	=	GrEmuMdlGetEmptyDevIdx();
					if ( 0 <= Tv_DevIdx )
					{
						//add device
						V_GrEmuDevTbl[Tv_DevIdx]	=	(Cls_GrElmDevBase*)new Cls_GrElmDevSolo( (DWORD)Tv_DevIdx, Tv_Is6110 );
						//do probe
						Tv_PtrPciDev	=	V_GrEmuDevTbl[Tv_DevIdx]->PciDevPtrGet();
						A_PtrDrv->probe( Tv_PtrPciDev, &A_PtrDrv->id_table[Tv_IdtIdx] );
					}
#ifdef EMU_SOLO_DUAL
					Tv_DevIdx	=	GrEmuMdlGetEmptyDevIdx();
					if ( 0 <= Tv_DevIdx )
					{
						//add device

					}
#endif
				}
			}
#endif	//	(defined EMU_SOLO_6010) || (defined EMU_SOLO_6110)

			//next
			Tv_IdtIdx ++;
		}
}
//--------------------------------------------------------------------
void	pci_unregister_driver(struct pci_driver* A_PtrDrv )
{
	// local -------------------
		DWORD		Tv_IdtIdx;			//id table index
		int			Tv_DevIdx;
		Ptr_pci_dev	Tv_PtrPciDev;
	// code --------------------
		//find device
		Tv_IdtIdx	=	0;
		while ( 0 != A_PtrDrv->id_table[Tv_IdtIdx].vendor )
		{
			//remoce driver
			for ( Tv_DevIdx=0;Tv_DevIdx < E_GrEmuMdlMaxDevCnt;Tv_DevIdx++ )
			{
				if ( NULL != V_GrEmuDevTbl[Tv_DevIdx] )
				{
					//solo check
					if (	( V_GrEmuDevTbl[Tv_DevIdx]->PciVendorIdGet() == A_PtrDrv->id_table[Tv_IdtIdx].vendor) && 
								( V_GrEmuDevTbl[Tv_DevIdx]->PciProductIdGet() == A_PtrDrv->id_table[Tv_IdtIdx].device)	)
					{
						//remove service call
						Tv_PtrPciDev	=	V_GrEmuDevTbl[Tv_DevIdx]->PciDevPtrGet();
						A_PtrDrv->remove( Tv_PtrPciDev );
						//release device
						delete	V_GrEmuDevTbl[Tv_DevIdx];
						V_GrEmuDevTbl[Tv_DevIdx]	=	NULL;
					}
				}
			}
			//next
			Tv_IdtIdx ++;
		}
}
//--------------------------------------------------------------------
void	cdev_del(struct cdev *A_PtrDev )
{
	// local -------------------
		int		Tv_Node;
	// code --------------------
		//check current dev
		if ( 0 <= A_PtrDev->Node )
		{
			Tv_Node		=	A_PtrDev->Node;
			//check exist node
			if ( V_GrEmuDrvTbl[Tv_Node].IsUse )
			{
				//release
				V_GrEmuDrvTbl[Tv_Node].IsUse	=	FALSE;
				V_GrEmuDrvTbl[Tv_Node].PtrFo	=	NULL;
			}
		}
}
//--------------------------------------------------------------------
void	unregister_chrdev_region(dev_t A_DevNum, unsigned A_Count )
{
		//none work
}
//--------------------------------------------------------------------
void	iounmap( void* A_Ptr )
{
		//none work
}
//--------------------------------------------------------------------
void	release_mem_region( DWORD A_PhysAdr, DWORD A_Size )
{
		//none work
}
//--------------------------------------------------------------------
int		remap_pfn_range(struct vm_area_struct *A_PtrVme, DWORD A_From, DWORD A_Pfn, DWORD A_Size, pgprot_t A_Prot )
{
		A_PtrVme->MapAdr	=	(void*)A_Pfn;
		return	0;
}
//--------------------------------------------------------------------
int		pci_write_config_byte(struct pci_dev* A_PtrPciDev, int A_Where, BYTE A_Val )
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	-1;
		//check exist device
		if ( NULL != V_GrEmuDevTbl[A_PtrPciDev->DevIdx] )
		{
			if ( V_GrEmuDevTbl[A_PtrPciDev->DevIdx]->PciCfgWriteByte( (DWORD)A_Where, A_Val ) )
			{
				Tv_Result	=	0;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
int		pci_write_config_word(struct pci_dev* A_PtrPciDev, int A_Where, WORD A_Val )
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	-1;
		//check exist device
		if ( NULL != V_GrEmuDevTbl[A_PtrPciDev->DevIdx] )
		{
			if ( V_GrEmuDevTbl[A_PtrPciDev->DevIdx]->PciCfgWriteWord( (DWORD)A_Where, A_Val ) )
			{
				Tv_Result	=	0;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
int		pci_write_config_dword(struct pci_dev* A_PtrPciDev, int A_Where, DWORD A_Val )
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	-1;
		//check exist device
		if ( NULL != V_GrEmuDevTbl[A_PtrPciDev->DevIdx] )
		{
			if ( V_GrEmuDevTbl[A_PtrPciDev->DevIdx]->PciCfgWriteDword( (DWORD)A_Where, A_Val ) )
			{
				Tv_Result	=	0;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
int		pci_read_config_byte(struct pci_dev* A_PtrPciDev, int A_Where, BYTE* A_PtrRtVal )
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	-1;
		//check exist device
		if ( NULL != V_GrEmuDevTbl[A_PtrPciDev->DevIdx] )
		{
			if ( V_GrEmuDevTbl[A_PtrPciDev->DevIdx]->PciCfgReadByte( (DWORD)A_Where, A_PtrRtVal ) )
			{
				Tv_Result	=	0;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
int		pci_read_config_word(struct pci_dev* A_PtrPciDev, int A_Where, WORD* A_PtrRtVal )
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	-1;
		//check exist device
		if ( NULL != V_GrEmuDevTbl[A_PtrPciDev->DevIdx] )
		{
			if ( V_GrEmuDevTbl[A_PtrPciDev->DevIdx]->PciCfgReadWord( (DWORD)A_Where, A_PtrRtVal ) )
			{
				Tv_Result	=	0;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
int		pci_read_config_dword(struct pci_dev* A_PtrPciDev, int A_Where, DWORD* A_PtrRtVal )
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	-1;
		//check exist device
		if ( NULL != V_GrEmuDevTbl[A_PtrPciDev->DevIdx] )
		{
			if ( V_GrEmuDevTbl[A_PtrPciDev->DevIdx]->PciCfgReadDword( (DWORD)A_Where, A_PtrRtVal ) )
			{
				Tv_Result	=	0;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	pci_set_drvdata (struct pci_dev* A_PtrPciDev, void* A_Val )
{
		//check exist device
		if ( NULL != V_GrEmuDevTbl[A_PtrPciDev->DevIdx] )
		{
			 V_GrEmuDevTbl[A_PtrPciDev->DevIdx]->PciUserDataSet( A_Val );
		}
}
//--------------------------------------------------------------------
void*	pci_get_drvdata (struct pci_dev* A_PtrPciDev )
{
	// local -------------------
		void*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		//check exist device
		if ( NULL != V_GrEmuDevTbl[A_PtrPciDev->DevIdx] )
		{
			Tv_Result	=	V_GrEmuDevTbl[A_PtrPciDev->DevIdx]->PciUserDataGet( );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
int		pci_enable_device(struct pci_dev* A_PtrPciDev )
{
		return	0;
}
//--------------------------------------------------------------------
void	pci_disable_device( struct pci_dev* A_PtrPciDev )
{
		//not work
}
//--------------------------------------------------------------------
void	pci_set_master( struct pci_dev* A_PtrPciDev )
{
		//not work
}
//--------------------------------------------------------------------
void	spin_lock_init( spinlock_t* A_PtrLock )
{
		GrPrcCritInit( A_PtrLock );
}
//--------------------------------------------------------------------
DWORD	pci_resource_start( struct pci_dev* A_PtrPciDev, DWORD A_BarIdx )
{
		return	A_PtrPciDev->Rsrc[A_BarIdx].Adr;
}
//--------------------------------------------------------------------
DWORD	pci_resource_len( struct pci_dev* A_PtrPciDev, DWORD A_BarIdx )
{
		return	A_PtrPciDev->Rsrc[A_BarIdx].Size;
}
//--------------------------------------------------------------------
int		pci_request_regions( struct pci_dev* A_PtrPciDev, char* A_StrName )
{
		return	0;
}
//--------------------------------------------------------------------
void	pci_release_regions(struct pci_dev* A_PtrPciDev )
{
		//not work
}
//--------------------------------------------------------------------
int		request_irq(DWORD A_Irq, irqreturn_t (*A_Isr)(int, void *, struct pt_regs *), DWORD A_Flags, char* A_StrName, void* A_UserId )
{
	// local -------------------
		int		Tv_Result;
		DWORD	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	-1;
		//check empty irq slot
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrEmuMdlMaxIrqCnt;Tv_WkIdx++ )
		{
			//check empty slot
			if ( NULL == V_GrEmuIsrTbl[Tv_WkIdx].Isr )
			{
				//find empty slot
				V_GrEmuIsrTbl[Tv_WkIdx].Isr		=	A_Isr;
				V_GrEmuIsrTbl[Tv_WkIdx].Id		=	(DWORD)A_UserId;
				Tv_Result	=	TRUE;
				break;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	free_irq( DWORD A_Irq, void* A_UserId )
{
	// local -------------------
		DWORD	Tv_WkIdx;
	// code --------------------
		//find match slot
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrEmuMdlMaxIrqCnt;Tv_WkIdx++ )
		{
			//check empty slot
			if ( (NULL != V_GrEmuIsrTbl[Tv_WkIdx].Isr) && ( (DWORD)A_UserId == V_GrEmuIsrTbl[Tv_WkIdx].Id ) )
			{
				//reset slot
				V_GrEmuIsrTbl[Tv_WkIdx].Isr		=	NULL;
				V_GrEmuIsrTbl[Tv_WkIdx].Id		=	0;
				break;
			}
		}
}
//--------------------------------------------------------------------
void	rmb( void )
{
		//not work
}
//--------------------------------------------------------------------
void	wmb( void )
{
		//not work
}
//--------------------------------------------------------------------
DWORD readl( void* A_Ptr )
{
	// local -------------------
		DWORD		Tv_Result;
		DWORD		Tv_WkIdx;
		BOOL		Tv_IsWork;
	// code --------------------
		Tv_Result	=	*( (DWORD*)A_Ptr );
		Tv_IsWork	=	FALSE;
		//run device
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrEmuMdlMaxDevCnt;Tv_WkIdx++ )
		{
			if ( NULL != V_GrEmuDevTbl[Tv_WkIdx] )
			{
				if ( V_GrEmuDevTbl[Tv_WkIdx]->MioReadDword( A_Ptr, &Tv_Result ) )
				{
					Tv_IsWork	=	TRUE;
					break;
				}
			}
		}
		//check worked
		if ( !Tv_IsWork )
		{
			DbgMsgPrint( "readl : non device address %8X . \n", (DWORD)A_Ptr );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	writel( DWORD A_Val, void* A_Ptr )
{
	// local -------------------
		DWORD		Tv_WkIdx;
		BOOL		Tv_IsWork;
	// code --------------------
		Tv_IsWork	=	FALSE;
		//run device
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrEmuMdlMaxDevCnt;Tv_WkIdx++ )
		{
			if ( NULL != V_GrEmuDevTbl[Tv_WkIdx] )
			{
				if ( V_GrEmuDevTbl[Tv_WkIdx]->MioWriteDword( A_Ptr, A_Val ) )
				{
					Tv_IsWork	=	TRUE;
					break;
				}
			}
		}
		//check worked
		if ( !Tv_IsWork )
		{
			DbgMsgPrint( "writel : non device address %8X . \n", (DWORD)A_Ptr );
		}
}
//--------------------------------------------------------------------
void	spin_lock_irqsave( spinlock_t* A_PtrLock, DWORD A_Flag )
{
		GrPrcCritEnter( A_PtrLock );
}
//--------------------------------------------------------------------
void	spin_unlock_irqrestore( spinlock_t* A_PtrLock, DWORD A_Flag )
{
		GrPrcCritLeave( A_PtrLock );
}
//--------------------------------------------------------------------
void	local_irq_disable( void )
{
		GrPrcCritEnter( &V_GrEmuMdlIrqCrti );
}
//--------------------------------------------------------------------
void	local_irq_enable( void )
{
		GrPrcCritLeave( &V_GrEmuMdlIrqCrti );
}
//--------------------------------------------------------------------
int	copy_from_user( void* A_PtrTg, void* A_PtrUser, DWORD A_Size )
{
		CopyMemory( A_PtrTg, A_PtrUser, A_Size );
		return	0;
}
//--------------------------------------------------------------------
int	copy_to_user( void* A_PtrUser, void* A_PtrSr, DWORD A_Size )
{
		CopyMemory( A_PtrUser, A_PtrSr, A_Size );
		return	0;
}
//--------------------------------------------------------------------
void	GrEmuMdlInit( void )
{
		GrPrcCritInit( &V_GrEmuMdlIrqCrti );
		GrPrcCritInit( &V_GrEmuMdlTimerCrti );
		
		GrDumyZeroMem( V_GrEmuDrvTbl, sizeof(V_GrEmuDrvTbl) );
		GrDumyZeroMem( V_GrEmuDevTbl, sizeof(V_GrEmuDevTbl) );
		GrDumyZeroMem( V_GrEmuIsrTbl, sizeof(V_GrEmuIsrTbl) );

		//timer create
		GrBufQueInit( (Ptr_GrBufQue)V_GrEmuMdlTimerQue , sizeof(timer_list), E_GrEmuMdlTimerListQueSize );
		V_GrEmuMdlTimerId	=	SetTimer( NULL, (DWORD)V_GrEmuMdlTimerQue, 20, GrEmuMdlTimerProc );
}
//--------------------------------------------------------------------
void	GrEmuMdlFinish( void )
{
		//timer delete
		KillTimer( NULL, V_GrEmuMdlTimerId );
		
		GrPrcCritDel( &V_GrEmuMdlTimerCrti );
		GrPrcCritDel( &V_GrEmuMdlIrqCrti );
}
//--------------------------------------------------------------------
void	GrEmuMdlIrqOccure( BYTE A_Irq )
{
	// local -------------------
		DWORD	Tv_WkIdx;
		irqreturn_t	Tv_Rslt;
		BOOL	Tv_IsOcced;
	// code --------------------
		GrPrcCritEnter( &V_GrEmuMdlIrqCrti );

		//do interrupt
		Tv_IsOcced	=	FALSE;
		for ( Tv_WkIdx = 0;Tv_WkIdx < E_GrEmuMdlMaxIrqCnt;Tv_WkIdx++ )
		{
			if ( NULL != V_GrEmuIsrTbl[Tv_WkIdx].Isr )
			{
				Tv_Rslt	=	(*V_GrEmuIsrTbl[Tv_WkIdx].Isr)( (int)A_Irq, (void*)V_GrEmuIsrTbl[Tv_WkIdx].Id, NULL );
				if ( IRQ_HANDLED == Tv_Rslt )
				{
					Tv_IsOcced	=	TRUE;
				}
			}
		}
	
		GrPrcCritLeave( &V_GrEmuMdlIrqCrti );
}
//--------------------------------------------------------------------
VOID CALLBACK GrEmuMdlTimerProc( HWND A_Hndl, UINT A_Msg, UINT_PTR A_IdPara, DWORD A_Time )
{
	// local -------------------
		Ptr_GrBufQue	Tv_PtrQue;
		Ptr_timer_list	Tv_PtrItm;
		DWORD		Tv_TickGap;
	// code --------------------
		Tv_PtrQue	=	(Ptr_GrBufQue)V_GrEmuMdlTimerQue;
		Tv_PtrItm	=	(Ptr_timer_list)GrBufQueGetOutPtr( Tv_PtrQue );
		if ( NULL != Tv_PtrItm )
		{
			//check time
			Tv_TickGap	=	A_Time - Tv_PtrItm->TickStart;
			if ( Tv_PtrItm->TickRun <= Tv_TickGap )
			{
				//run timer
				Tv_PtrItm->function( Tv_PtrItm->data );
				//delete timer
				GrBufQueOut( Tv_PtrQue );
			}
		}
}
//--------------------------------------------------------------------
void init_timer(struct timer_list * A_PtrTimePara )
{
		//not work
}
//--------------------------------------------------------------------
void	add_timer(struct timer_list * A_PtrTimePara )
{
	// local -------------------
		Ptr_GrBufQue	Tv_PtrQue;
		Ptr_timer_list	Tv_PtrItm;
	// code --------------------
		//intert timer queue
		Tv_PtrQue	=	(Ptr_GrBufQue)V_GrEmuMdlTimerQue;
		GrPrcCritEnter( &V_GrEmuMdlTimerCrti );
		
		Tv_PtrItm	=	(Ptr_timer_list)GrBufQueGetInPtr( Tv_PtrQue );
		if ( NULL != Tv_PtrItm )
		{
			//add
			Tv_PtrItm->TickStart	=	GetTickCount();
			Tv_PtrItm->TickRun	=	Tv_PtrItm->expires - Tv_PtrItm->TickStart;
			*Tv_PtrItm	=	*A_PtrTimePara;
			GrBufQueIn( Tv_PtrQue );
		}

		GrPrcCritLeave( &V_GrEmuMdlTimerCrti );
}
//--------------------------------------------------------------------
void	del_timer(struct timer_list* A_PtrTimePara )
{
		//not implement
}
//--------------------------------------------------------------------
void	mdelay( DWORD A_Ms )
{
		Sleep( A_Ms );
}
//--------------------------------------------------------------------
int		misc_register(struct miscdevice * misc)
{
		return	0;
}
//--------------------------------------------------------------------
int		misc_deregister(struct miscdevice *misc)
{
		return	0;
}
//--------------------------------------------------------------------
ktime_t	ktime_set(const long secs, const unsigned long nsecs)
{
		return	(((ktime_t)secs * 1000000000) + (ktime_t)nsecs);
}
//--------------------------------------------------------------------
void hrtimer_init(struct hrtimer *timer, clockid_t which_clock, enum hrtimer_mode mode)
{

}
//--------------------------------------------------------------------
int hrtimer_start(struct hrtimer *timer, ktime_t tim, const enum hrtimer_mode mode)
{
		return	0;
}
//--------------------------------------------------------------------
int hrtimer_cancel(struct hrtimer *timer)
{
		return	0;
}
//--------------------------------------------------------------------

