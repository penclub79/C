/*
	solo motion plug in  motion check

*/

//====================================================================
//include
#include	<Hw/Solo/GrSoloMpMtn.h>
#include	<GrDumyTool.h>

#include	<stdlib.h>

//====================================================================
//local const
#define E_GrSoloMpMtnBlkCntX				22
#define E_GrSoloMpMtnBlkNtscCntY		15
#define E_GrSoloMpMtnBlkPalCntY			18

#define E_GrSoloMpMtnBlkNtscTtCnt		(E_GrSoloMpMtnBlkCntX*E_GrSoloMpMtnBlkNtscCntY)
#define E_GrSoloMpMtnBlkPalTtCnt		(E_GrSoloMpMtnBlkCntX*E_GrSoloMpMtnBlkPalCntY)

//====================================================================
//local type

//====================================================================
//global var

//====================================================================
//functions

//--------------------------------------------------------------------
Cls_GrSoloMpMtn::Cls_GrSoloMpMtn( Cls_GrTaskSvr* A_TaskSvr, Cls_GrDevSolo* A_ObjDev ):
Cls_GrTaskCli()
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		//init
		m_TaskSvr			=	A_TaskSvr;

		m_DevSolo			=	A_ObjDev;

		m_Is6110			=	m_DevSolo->Is6110();
		m_ChUseMap		=	0;
		m_MtnOcuMap		=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_SoloMaxChCnt;Tv_WkIdx++ )
		{
			m_MtnMapTbl[Tv_WkIdx]	=	NULL;
		}
		GrDumyZeroMem( m_DispBufTbl, sizeof(m_DispBufTbl) );
}
//--------------------------------------------------------------------
Cls_GrSoloMpMtn::~Cls_GrSoloMpMtn()
{
	// local -------------------
	// code --------------------

}
//--------------------------------------------------------------------
BOOL8	Cls_GrSoloMpMtn::RtlCallback( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrRecRcvMtnCallbackCmd == A_PtrMsg->Cmd )
		{
			//do motion check
			LcMtnCheck( (Ptr_GrRecRcvMtnCbMsg)A_PtrMsg );

			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrSoloMpMtn::LcMtnCheck( Ptr_GrRecRcvMtnCbMsg A_PtrMsg )
{
	// local -------------------
		__u32	Tv_Ch;
		__u32*	Tv_PtrSr;
		__u32*	Tv_PtrMap;
		__u32*	Tv_PtrDsp;
		__u32	Tv_WkIdx;
		BOOL8	Tv_IsOcu;
		__u32	Tv_OcuMap;
		__u8	Tv_Scale;
		__u32	Tv_WkY;
		__u32	Tv_WkX;
		__u32	Tv_BmSrOd;
		__u32	Tv_BmSrEv;
		__u32	Tv_BmMap;
		__u32	Tv_BpY;
		__u32*	Tv_PtrEsr;
		__u32	Tv_BlkCntY;
	// code --------------------
		Tv_IsOcu	=	FALSE;
		Tv_Ch	=	A_PtrMsg->Ch;

		//check exist map
		if ( (NULL != m_MtnMapTbl[Tv_Ch]) && (0 != ( (1 << Tv_Ch) & m_ChUseMap)) )
		{
			Tv_PtrMap	=	(__u32*)m_MtnMapTbl[Tv_Ch];
			Tv_PtrDsp	=	(__u32*)m_DispBufTbl[Tv_Ch];
			Tv_PtrSr	=	(__u32*)A_PtrMsg->PtrMap;	
			//change motion scale
			Tv_Scale	=	A_PtrMsg->Scale & 0x03;
			switch( Tv_Scale )
			{
				case	E_GrDvrVdoHighRes:
					Tv_BlkCntY	=	E_GrSoloMpMtnBlkNtscCntY;
					if ( 0 != (E_GrDvrVdoResPal & A_PtrMsg->Scale) )
					{
						Tv_BlkCntY	=	E_GrSoloMpMtnBlkPalCntY;
					}
					if ( !m_Is6110 )
					{
						Tv_BlkCntY --;
					}
					
					//high resolution
					Tv_BmMap	=	1;
					Tv_BpY		=	0;
					for ( Tv_WkY=0;Tv_WkY < Tv_BlkCntY;Tv_WkY++ )
					{
						Tv_PtrSr	=	(__u32*)( (__u32)A_PtrMsg->PtrMap + ((Tv_BpY >> 3) & ~3) );
						Tv_BmSrOd	=	0x03 << (Tv_BpY & 0x1F);
						Tv_PtrEsr	=	(__u32*)( (__u32)A_PtrMsg->PtrMap + (((Tv_BpY + (E_GrSoloMpMtnBlkCntX << 1)) >> 3) & ~3) );
						Tv_BmSrEv	=	0x03 << ((Tv_BpY + (1 << E_GrSoloMpMtnBlkCntX)) & 0x1F);
						for ( Tv_WkX=0;Tv_WkX < E_GrSoloMpMtnBlkCntX;Tv_WkX++ )
						{
							if ( 0 != ( Tv_BmMap & (*Tv_PtrMap) ) )
							{
								//check
								if ( ( 0 != (Tv_BmSrOd & (*Tv_PtrSr)) ) || ( 0 != (Tv_BmSrEv & (*Tv_PtrEsr)) ) )
								{
									Tv_IsOcu		=	TRUE;
									*Tv_PtrDsp	=	*Tv_PtrDsp | Tv_BmMap;
								}
								else
								{
									*Tv_PtrDsp	=	*Tv_PtrDsp & ~Tv_BmMap;
								}
							}
							//next
							Tv_BmMap	=	Tv_BmMap << 1;
							if ( 0 == Tv_BmMap )
							{
								Tv_BmMap	=	1;
								Tv_PtrMap ++;
								Tv_PtrDsp ++;
							}
							Tv_BmSrOd	=	Tv_BmSrOd << 2;
							if ( 0 == Tv_BmSrOd )
							{
								Tv_BmSrOd	=	3;
								Tv_PtrSr ++;
							}
							Tv_BmSrEv	=	Tv_BmSrEv << 2;
							if ( 0 == Tv_BmSrEv )
							{
								Tv_BmSrEv	=	3;
								Tv_PtrEsr ++;
							}
						}
						//next
						Tv_BpY	=	Tv_BpY + (E_GrSoloMpMtnBlkCntX << 2);
					}
					break;

				case	E_GrDvrVdoMidRes:
					//middle resolution
					Tv_BmMap	=	1;
					Tv_BmSrOd	=	3;
					Tv_BlkCntY	=	E_GrSoloMpMtnBlkNtscTtCnt;
					if ( 0 != (E_GrDvrVdoResPal & A_PtrMsg->Scale) )
					{
						Tv_BlkCntY	=	E_GrSoloMpMtnBlkPalTtCnt;
					}
					if ( !m_Is6110 )
					{
						Tv_BlkCntY	=	Tv_BlkCntY - E_GrSoloMpMtnBlkCntX;
					}

					for ( Tv_WkIdx=0;Tv_WkIdx < Tv_BlkCntY;Tv_WkIdx++ )
					{
						if ( 0 != ( Tv_BmMap & (*Tv_PtrMap) ) )
						{
							//check
							if ( 0 != (Tv_BmSrOd & (*Tv_PtrSr))  )
							{
								Tv_IsOcu		=	TRUE;
								*Tv_PtrDsp	=	*Tv_PtrDsp | Tv_BmMap;
							}
							else
							{
								*Tv_PtrDsp	=	*Tv_PtrDsp & ~Tv_BmMap;
							}
						}
						//next
						Tv_BmMap	=	Tv_BmMap << 1;
						if ( 0 == Tv_BmMap )
						{
							Tv_BmMap	=	1;
							Tv_PtrMap ++;
							Tv_PtrDsp ++;
						}
						Tv_BmSrOd	=	Tv_BmSrOd << 2;
						if ( 0 == Tv_BmSrOd )
						{
							Tv_BmSrOd	=	3;
							Tv_PtrSr ++;
						}
					}
					break;

				default:
					//low res
					Tv_BlkCntY	=	E_GrSoloMpMtnBlkNtscTtCnt;
					if ( 0 != (E_GrDvrVdoResPal & A_PtrMsg->Scale) )
					{
						Tv_BlkCntY	=	E_GrSoloMpMtnBlkPalTtCnt;
					}
					if ( !m_Is6110 )
					{
						Tv_BlkCntY	=	Tv_BlkCntY - E_GrSoloMpMtnBlkCntX;
					}
					Tv_BlkCntY	=	(Tv_BlkCntY + 31) >> 5;
					
					//check motion occured
					for ( Tv_WkIdx=0;Tv_WkIdx < Tv_BlkCntY;Tv_WkIdx++ )
					{
						Tv_OcuMap		=	(*Tv_PtrSr) & (*Tv_PtrMap);
						*Tv_PtrDsp	=	Tv_OcuMap;
						if ( 0 != Tv_OcuMap )
						{
							Tv_IsOcu	=	TRUE;
						}
						//next
						Tv_PtrSr ++;
						Tv_PtrMap ++;
						Tv_PtrDsp ++;
					}
					break;
			}

		}
		//update
		if ( Tv_IsOcu )
		{
			m_MtnOcuMap	=	m_MtnOcuMap | (1 << Tv_Ch);
		}
		else
		{
			m_MtnOcuMap	=	m_MtnOcuMap & ~(1 << Tv_Ch);
		}
}
//--------------------------------------------------------------------
void	Cls_GrSoloMpMtn::MotionMapSet( __u8 A_Ch, void* A_PtrMap )
{
	// local -------------------
	// code --------------------
		m_MtnMapTbl[A_Ch]	=	A_PtrMap;
}
//--------------------------------------------------------------------
void*	Cls_GrSoloMpMtn::DisplayMapGet( __u8 A_Ch )
{
	// local -------------------
	// code --------------------
		return	(void*)m_DispBufTbl[A_Ch];
}
//--------------------------------------------------------------------
__u32	Cls_GrSoloMpMtn::MotionOucMapGet( void )
{
		return	m_MtnOcuMap;
}
//--------------------------------------------------------------------
void	Cls_GrSoloMpMtn::ChannelUseMapSet( __u32 A_Map )
{
		m_ChUseMap	=	A_Map;
}
//--------------------------------------------------------------------
