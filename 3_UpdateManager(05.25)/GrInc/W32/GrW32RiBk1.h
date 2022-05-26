/*
	win32 dib quad
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrW32RiBk1
#define	_EPC_GrW32RiBk1

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<Dvr/GrDvrRiBase.h>
#include	<GrTimeTool.h>
#include	<GrFile64.h>

//====================================================================
//constance
#define E_GrW32RiBk1MaxChCnt			16

#define E_GrW32RiBk1MaxFileCnt		512

//#define E_GrW32RiBk1FccFileHead		0x48464946
//#define E_GrW32RiBk1FccFileHead		0x31564B42
#define E_GrW32RiBk1FccFileHead		0x32564B42

#define E_GrW32RiBk1MaxChukSize		0x200000
#define E_GrW32RiBk1ChukMax				48
#define E_GrW32RiBk1PassXorVal		0x18
#define E_GrW32RiBk1PassMax				8

//====================================================================
//global macro

//====================================================================
//global type

#pragma pack(push)
#pragma pack(1)

typedef	struct St_GrW32RiBk1ChukItm 
{
	__u32		Fptr;							//file pointer
	__u32		ContSize;					//data size
	__u32		Fcnt;							//frame count
	St_GrTimeRng	TimeRng;		//time range
}	*Ptr_GrW32RiBk1ChukItm;

typedef struct St_GrW32RiBk1FileHead
{
	__u32		Fcc;							//fcc code
	St_GrTimeRng		TimeRng;	//total time range
	St_GrTimeRule		TimeRule;
	Def_StrName			StrId[E_GrW32RiBk1MaxChCnt];
	__s8		StrPass[E_GrW32RiBk1PassMax];
	__u32		ChukCnt;					//chunk count
	St_GrW32RiBk1ChukItm	ChukTbl[E_GrW32RiBk1ChukMax];
} *Ptr_GrW32RiBk1FileHead;

#pragma pack(pop)

typedef	struct St_GrW32RiBk1FileItm 
{
	Def_StrShort	StrFn;
	St_GrW32RiBk1FileHead	Fh;			//file header
}	*Ptr_GrW32RiBk1FileItm;

//====================================================================
//class

class Cls_GrW32RiBk1	:	public Cls_GrDvrRiBase
{
	private:
		__u32		m_FileCnt;
		St_GrW32RiBk1FileItm	m_FileTbl[E_GrW32RiBk1MaxFileCnt];
		St_GrTimeRng		m_TimeRng;			//total time range
	protected:
		void		InitFileTbl( void );
		BOOL8		FindFccAv( void* A_PtrBuf, __u32 A_BufSize, __u32* A_PtrRtOfs );
		__u32		FindFrmCnt( void* A_PtrBuf, __u32 A_BufSize );	
		void		LcPassDec( void );
	public:
		Cls_GrW32RiBk1( void );
		virtual	~Cls_GrW32RiBk1();

		virtual	BOOL8	RecGetAllTimeRng( Ptr_GrTimeRng A_PtrRtTmRng, BOOL8 A_IsInst, BOOL8 A_IsBackup = FALSE );
		virtual	BOOL8	RecReadChunk( BOOL8 A_IsInst, Def_GrTime A_Time, __u32 A_TimeType, 
									__u32* A_PtrRtFrmCnt, void* A_PtrChunk, BOOL8 A_IsBackup = FALSE );
		//custom function
		void		OpenByPath( __s8* A_StrPath );
		BOOL8		IsPassExist( void );
		BOOL8		GetPassword( __s8* A_StrRt );
		void		Reset( void );
		BOOL8		GetChName( __u32 A_Ch, __s8* A_StrRt );

};

//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

