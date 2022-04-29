/*
		script encoder

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<DvlUi/GrUiScptEnc.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<Ui/GrUiConsole.h>
#include	<DvlUi/GrUiDvlPara.h>

//====================================================================
//local const

#define E_GrUiScpeEncDfltWinSizeX	300
#define E_GrUiScpeEncDfltWinSizeY	300

//====================================================================
//local type

//====================================================================
//local macro

//====================================================================
//local function

//====================================================================
//var

//====================================================================
//--------------------------------------------------------------------
Cls_GrUiScptEnc::Cls_GrUiScptEnc(  )
{
		//init
		m_DvlParaCnt	=	GrUiDvlGetTotalParaCnt();
}
//--------------------------------------------------------------------
Cls_GrUiScptEnc::~Cls_GrUiScptEnc()
{
}
//--------------------------------------------------------------------
__u32	Cls_GrUiScptEnc::EncodeObj( Cls_GrUiDoBase* A_Obj, void* A_PtrBuf )
{
	// local -------------------
	// code --------------------
		//init property
		m_EncSize		=	0;
		m_PtrBuf		=	A_PtrBuf;
		m_PtrEnc		=	(__u8*)m_PtrBuf;

		//start get parametter
		LcEncChainObj( A_Obj );
		
		return	m_EncSize;
}
//--------------------------------------------------------------------
void	Cls_GrUiScptEnc::LcEncOneObj( Cls_GrUiDoBase* A_Obj )
{
	// local -------------------
		__u32		Tv_WkIdx;
		Ptr_GrUiParaDesc	Tv_PtrParaInfo;
		Ptr_GrUiScptObjHd	Tv_PtrScObjHd;
		Ptr_GrRect	Tv_PtrRc;
		Ptr_GrUiScptValHd	Tv_PtrScValHd;
		__u32		Tv_ValDfltSize;
		__u32		Tv_ValObjSize;
		BOOL8		Tv_IsMatch;
		void*		Tv_PtrVal;
		__u8		Tv_ObjTypeId;
	// code --------------------
		//build object header
		Tv_PtrScObjHd	=	(Ptr_GrUiScptObjHd)m_PtrEnc;
		Tv_PtrScObjHd->ScptId	=	A_Obj->ObjIdGet();
		Tv_PtrScObjHd->BeginId	=	E_GrUiScptObjBeginId;
		Tv_PtrScObjHd->TypeId	=	A_Obj->TypeIdGet();
		Tv_PtrScObjHd->Layer	=	0;
		Tv_PtrScObjHd->ValCnt	=	0;
		Tv_PtrRc	=	A_Obj->RectGet();
		Tv_PtrScObjHd->Rect.left		=	Tv_PtrRc->left;
		Tv_PtrScObjHd->Rect.top			=	Tv_PtrRc->top;
		Tv_PtrScObjHd->Rect.right		=	Tv_PtrRc->right;
		Tv_PtrScObjHd->Rect.bottom	=	Tv_PtrRc->bottom;

		//add head
		m_PtrEnc	=	m_PtrEnc + sizeof(St_GrUiScptObjHd);
		m_EncSize	=	m_EncSize + sizeof(St_GrUiScptObjHd);

		//value table
		for (Tv_WkIdx=0;Tv_WkIdx < m_DvlParaCnt;Tv_WkIdx++ )
		{
			Tv_ObjTypeId	=	A_Obj->TypeIdGet();
			//check parametter able
			Tv_PtrParaInfo	=	GrUiDvlGetParaInfoByIdx( Tv_WkIdx );
			if ( NULL != Tv_PtrParaInfo )
			{
				//check able parametter
				if ( GrUiDvlIsAblePara( Tv_ObjTypeId, Tv_PtrParaInfo->Id ) )
				{
					//set value header
					Tv_PtrScValHd	=	(Ptr_GrUiScptValHd)m_PtrEnc;		
					Tv_PtrScValHd->ParaId		=	(__u16)Tv_PtrParaInfo->Id;
					Tv_PtrVal	=	(void*)( (__u32)m_PtrEnc + sizeof(Ptr_GrUiScptValHd) );
					//check change value
					Tv_ValDfltSize	=	GrUiDvlGetDfltVal( Tv_PtrParaInfo->Id, (void*)m_ValDflt );
					Tv_ValObjSize		=	GrUiDvlGetParaVal( A_Obj, Tv_PtrParaInfo->Id, Tv_PtrVal );
					//check change
					Tv_IsMatch	=	FALSE;
					if ( Tv_ValDfltSize == Tv_ValObjSize  )
					{
						if ( 0 == GrDumyMemCmp( (void*)m_ValDflt, Tv_PtrVal, Tv_ValObjSize  ) )
						{
							//match
							Tv_IsMatch	=	TRUE;
						}
					}
					else
					{
#ifndef LINUX_APP
						MessageBox( NULL, L"Parametter size missmatch!. Go to DAMGI and HIT!", L"ScripteEncode:LcEncOneObj", MB_OK | MB_ICONERROR );
#endif
					}
					//check add able
					if ( !Tv_IsMatch )
					{
						//add value item
						Tv_PtrScObjHd->ValCnt ++;
						Tv_PtrScValHd->Size	=	(__u16)Tv_ValObjSize;
						m_PtrEnc	=	m_PtrEnc + sizeof(Ptr_GrUiScptValHd) + Tv_ValObjSize;
						m_EncSize	=	m_EncSize + sizeof(Ptr_GrUiScptValHd) + Tv_ValObjSize;
					}
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiScptEnc::LcEncChainObj( Cls_GrUiDoBase* A_Obj )
{
	// local -------------------
		Cls_GrUiDoBase*	Tv_ObjSlave;
	// code --------------------
		LcEncOneObj( A_Obj );
		//check object type
		if ( E_GrUiDoTypeIdFrame != A_Obj->TypeIdGet() )
		{
			Tv_ObjSlave	=	A_Obj->SlaveObjGet();
			while ( NULL != Tv_ObjSlave )
			{
				LcEncChainObj( Tv_ObjSlave );
				//next
				Tv_ObjSlave	=	Tv_ObjSlave->NextObjGet();
			}
		}
		//mark end object
		*m_PtrEnc	=	E_GrUiScptObjEndId;
		m_EncSize	++;
		m_PtrEnc	++;
}
//--------------------------------------------------------------------
void	Cls_GrUiScptEnc::LcEncSlaveCode( __u16 A_ObjId )
{
	// local -------------------
		Ptr_GrUiScptObjHd	Tv_PtrScObjHd;
	// code --------------------
	//build object header
		Tv_PtrScObjHd	=	(Ptr_GrUiScptObjHd)m_PtrEnc;
		Tv_PtrScObjHd->ScptId	=	A_ObjId;
		Tv_PtrScObjHd->BeginId	=	E_GrUiScptObjBeginId;
		Tv_PtrScObjHd->TypeId	=	E_GrUiDoTypeIdNone;
		Tv_PtrScObjHd->Layer	=	0;
		Tv_PtrScObjHd->ValCnt	=	0;
		Tv_PtrScObjHd->Rect.left		=	0;
		Tv_PtrScObjHd->Rect.top			=	0;
		Tv_PtrScObjHd->Rect.right		=	0;
		Tv_PtrScObjHd->Rect.bottom	=	0;
		//add head
		m_PtrEnc	=	m_PtrEnc + sizeof(St_GrUiScptObjHd);
		m_EncSize	=	m_EncSize + sizeof(St_GrUiScptObjHd);
}
//--------------------------------------------------------------------
__u32	Cls_GrUiScptEnc::EncodeSlaveObj( Cls_GrUiDoBase* A_Obj, void* A_PtrBuf )
{
	// local -------------------
		Cls_GrUiDoBase*	Tv_ObjSlave;
	// code --------------------
		//init property
		m_EncSize		=	0;
		m_PtrBuf		=	A_PtrBuf;
		m_PtrEnc		=	(__u8*)m_PtrBuf;

		//encode
		LcEncSlaveCode( A_Obj->ObjIdGet() );
		Tv_ObjSlave	=	A_Obj->SlaveObjGet();
		while ( NULL != Tv_ObjSlave )
		{
			LcEncChainObj( Tv_ObjSlave );
			//next
			Tv_ObjSlave	=	Tv_ObjSlave->NextObjGet();
		}
		//mark end object
		*m_PtrEnc	=	E_GrUiScptObjEndId;
		m_EncSize	++;
		m_PtrEnc	++;

		return	m_EncSize;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiScptEnc::EncodeOneSlaveObj( Cls_GrUiDoBase* A_Obj, void* A_PtrBuf )
{
	// local -------------------
	// code --------------------
		//init property
		m_EncSize		=	0;
		m_PtrBuf		=	A_PtrBuf;
		m_PtrEnc		=	(__u8*)m_PtrBuf;

		//check window
		if ( !A_Obj->IsWindow() )
		{
			//encode
			LcEncSlaveCode( E_GrUiObjIdNone );
			LcEncChainObj( A_Obj );
			//mark end object
			*m_PtrEnc	=	E_GrUiScptObjEndId;
			m_EncSize	++;
			m_PtrEnc ++;
		}

		return	m_EncSize;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiScptEnc::EncodeEmptyWin( __u16 A_ObjId, void* A_PtrBuf )
{
	// local -------------------
		Ptr_GrUiScptObjHd	Tv_PtrScObjHd;
	// code --------------------
		//init property
		m_EncSize		=	0;
		m_PtrBuf		=	A_PtrBuf;
		m_PtrEnc		=	(__u8*)m_PtrBuf;

		//build object header
		Tv_PtrScObjHd	=	(Ptr_GrUiScptObjHd)m_PtrEnc;
		Tv_PtrScObjHd->ScptId	=	A_ObjId;
		Tv_PtrScObjHd->Layer	=	E_GrUiDoWinDfltLayer;
		Tv_PtrScObjHd->BeginId	=	E_GrUiScptObjBeginId;
		Tv_PtrScObjHd->TypeId	=	E_GrUiDoTypeIdWin;
		Tv_PtrScObjHd->ValCnt	=	0;
		Tv_PtrScObjHd->Rect.left		=	0;
		Tv_PtrScObjHd->Rect.top			=	0;
		Tv_PtrScObjHd->Rect.right		=	E_GrUiScpeEncDfltWinSizeX;
		Tv_PtrScObjHd->Rect.bottom	=	E_GrUiScpeEncDfltWinSizeY;
		//add head
		m_PtrEnc	=	m_PtrEnc + sizeof(St_GrUiScptObjHd);
		m_EncSize	=	m_EncSize + sizeof(St_GrUiScptObjHd);

		//mark end object
		*m_PtrEnc	=	E_GrUiScptObjEndId;
		m_EncSize	++;
		m_PtrEnc	++;

		return	m_EncSize;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiScptEnc::EncodeEmptyPage( __u16 A_ObjId, void* A_PtrBuf )
{
	// local -------------------
		Ptr_GrUiScptObjHd	Tv_PtrScObjHd;
	// code --------------------
		//init property
		m_EncSize		=	0;
		m_PtrBuf		=	A_PtrBuf;
		m_PtrEnc		=	(__u8*)m_PtrBuf;

		//build object header
		Tv_PtrScObjHd	=	(Ptr_GrUiScptObjHd)m_PtrEnc;
		Tv_PtrScObjHd->ScptId	=	A_ObjId;
		Tv_PtrScObjHd->Layer	=	0;
		Tv_PtrScObjHd->BeginId	=	E_GrUiScptObjBeginId;
		Tv_PtrScObjHd->TypeId	=	E_GrUiDoTypeIdNone;
		Tv_PtrScObjHd->ValCnt	=	0;
		Tv_PtrScObjHd->Rect.left		=	0;
		Tv_PtrScObjHd->Rect.top			=	0;
		Tv_PtrScObjHd->Rect.right		=	0;
		Tv_PtrScObjHd->Rect.bottom	=	0;
		//add head
		m_PtrEnc	=	m_PtrEnc + sizeof(St_GrUiScptObjHd);
		m_EncSize	=	m_EncSize + sizeof(St_GrUiScptObjHd);

		//mark end object
		*m_PtrEnc	=	E_GrUiScptObjEndId;
		m_EncSize	++;
		m_PtrEnc	++;

		return	m_EncSize;
}
//--------------------------------------------------------------------


