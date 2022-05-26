/*
	Gauss Platform tree base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoTreeBase
#define	_EPC_GrP4DoTreeBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoBase.h>

//====================================================================
//constance
#define	E_GrP4DoTreeBaseIconCnt			16

#define	E_GrP4DoTreeItemIdNone			0

#define	E_GrP4DoTreeBaseItemMaxCnt	128

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoTreeBasePara
{
	__u16	PntBg;					// paint background
	__u16	PntItemBg;
	__u16	ItemHeight;
	__u16	IdentSize;

	__u16	IconWidth;
	__u16	ItemGapLeft;
	__u16	ItemGapRight;
	__u16	IconGap;

	__u16	GapLeft;
	__u16	GapTop;
	__u16	GapRight;
	__u16	GapBottom;

	__u16	RelSbarId;	// relate scroll bar id
	
	__u16	PntIcon[E_GrP4DoTreeBaseIconCnt];

}	*Ptr_GrP4DoTreeBasePara;

typedef struct St_GrP4DoTreeBaseItem
{
	__u8	IconIdx;
	BOOL8	IsChildExp;		// child item expand
	__s16	PrntIdx;			// parent item index

	__u8	ItmIdx;
	__u8	_rsvd0;
	__u16	IdenPos;

	__u32	Id;
	__u32	UserData;
	void*	PtrRel;			// relate data pointer - for child class
}	*Ptr_GrP4DoTreeBaseItem;

//====================================================================
//global class
class	Cls_GrP4DoTreeBase	:	public	Cls_GrP4DoBase
{
	protected:
		St_GrP4DoTreeBasePara	m_ParaTreeBase;

		St_GrP4DoTreeBaseItem	m_TreeBaseItemTbl[E_GrP4DoTreeBaseItemMaxCnt];

		Ptr_GrP4DoTreeBaseItem	m_DispTbl[E_GrP4DoTreeBaseItemMaxCnt];
		__u8	m_DispItemCnt;
		__u8	m_DispAbleCnt;
		BOOL8	m_IsDispBldReq;		// display rebuild requirement

		__u8	m_DispPos;
		__u8	m_ScrlMax;			// scroll range
		__s32	m_DispAreaCnt;

		__u8	m_PntItemIdx;
		St_GrRect	m_PntRcCont;	// contents rect
		St_GrRect	m_PntRcItem;	// item draw
		St_GrRect	m_PntRcValue;	// item value area

		// method
		BOOL8	LcItemAdd(__u32 A_Id, __u32 A_PrntId, __u32 A_UserData, __u8* A_PtrRtItmIdx);
		BOOL8	LcItemDel(__u32 A_Id);
		__s16	LcItemFindById(__u32 A_Id);
		BOOL8	LcItemIconSet(__u32 A_Id, __u8 A_IconIdx);

		void	LcSubItemDel(__s16 A_PrntIdx);

		void	LcDispAddSlave(__s16 A_PrntIdx,__u16 A_IdentPos);
		void	LcDispTblBld(void);

		void	LcProcEvtSbarValChg(__u16 A_ObjId);

		void	LcPntCont(void);
		void	LcPntItem(void);
		virtual	void	LcPntValue(void);

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

		// inherited
		virtual	void	ProcUiMsg(Ptr_GrP4MsgHd A_PtrMsg);
		virtual	void	ProcPaint( void );		// do painting script

		virtual	BOOL8	ProcEvtMseLbtnUp(__u16 A_X, __u16 A_Y, __u8 A_BtnMap);

	public:
		Cls_GrP4DoTreeBase(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoTreeBase( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

