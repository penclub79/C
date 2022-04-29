/*
	disk direct access

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrDskAcs
#define	_EPC_GrDskAcs

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class

class Cls_GrDskAcs
{
	protected:
		Def_StrName		m_StrDev;	
		Def_StrTag		m_StrIdf;				//disk idf
		Def_StrShort	m_StrAcc;				//accessable file name

		__u32			m_SctSize;
		__u64			m_SctCnt;
		__u64			m_TotalSize;
		BOOL8			m_IsExist;
		BOOL8			m_IsGood;
	public:
		Cls_GrDskAcs( __s8* A_StrDev, __s8* A_StrIdf );
		virtual	~Cls_GrDskAcs();

		//info
		__s8*		GetStrDev( void );
		__s8*		GetStrIdf( void );
		__u32		GetSectorSize( void );
		__u64		GetTotalSector( void );
		__u64		GetTotalSize( void );
		BOOL8		IsExist( void );
		BOOL8		IsGood( void );

		//operation
		virtual	BOOL8		ReadSector( __u64 A_SctPos, __u32 A_SctCnt, void* A_PtrBuf );
		virtual	BOOL8		WriteSector( __u64 A_SctPos, __u32 A_SctCnt, void* A_PtrBuf );

		//misc
		__u32		CalcSctCntBySize( __u32 A_Size );
};

//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

