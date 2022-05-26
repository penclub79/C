/*
	iso9660 format
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrIso9660
#define	_EPC_GrIso9660

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

//====================================================================
//constance
#define E_GrIsoBufSize									0x80000
#define E_GrIsoFileMaxCnt								48

//position
#define E_GrIsoPosPvd										0x10		//primary volume descriptor
#define E_GrIsoPosSvd										0x11		//supplementary volume descriptor
#define E_GrIsoPosVdt										0x12		//volume descriptor terminator
#define E_GrIsoPosLpathPri							0x13		//L path table primary
#define E_GrIsoPosMpathPri							0x14		//M path table primary
#define E_GrIsoPosLpathSpl							0x15		//L path table supplementary
#define E_GrIsoPosMpathSpl							0x16		//M path table supplementary
#define E_GrIsoPosDirRecPri							0x17		//directory record primary - 8 sector
#define E_GrIsoPosDirRecSpl							0x1F		//directory record supplementary - 8 sector
#define E_GrIsoPosFileData							0x27		//file data start position

//specific
#define E_GrIsoPathRootSize							0x0A
#define E_GrIsoDirRecHeadSize						0x21
#define E_GrIsoDirRecRootSize						0x22

#define E_GrIsoSectorSize								0x800
#define E_GrIsoSectorModMask						0x7FF
#define E_GrIsoSectorShift							11

#define E_GrIsoLastGapSctCnt						0x150

// volume descriptor type
#define E_GrIsoVoldscTypePrimary				0x01
#define E_GrIsoVoldscTypeSupplementary	0x02
#define E_GrIsoVoldscTypePartition			0x03
#define E_GrIsoVoldscTypeBootrecord			0x00
#define E_GrIsoVoldscTypeSetTerminator	0xFF

//work stage
#define E_GrOddStageNone								0
#define E_GrOddStageInit								1
#define E_GrOddStagePrepare							2
#define E_GrOddStageHeadWrite						3
#define E_GrOddStageFileWrite						4
#define E_GrOddStageTailWrite						5
#define E_GrOddStageTailFinish					6
#define E_GrOddStageTailEnd							7
#define E_GrOddStageError								8

//head build stage
#define E_GrOddHeadStgGap								0	/* head gap write */
#define E_GrOddHeadStgImgBuild					1	/* image build to buffer */
#define E_GrOddHeadStgWrite							2	/* head write */


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrIsoDirRecHead
{
	__u8 		DirRecLen;						//[0x0000] length of directory record
	__u8 		ExtDirRecLen;					//[0x0001] extended attribute record length
	__u8		LSBLoExtent[4];				//[0x0002] lsb location of extent
	__u8		MSBLoExtent[4];				//[0x0006] msb location of extent
	__u8	 	LSBDataLength[4];			//[0x000A] lsb data length
	__u8 		MSBDataLength[4];			//[0x000E] msb data length
	__u8		Time[7];							//[0x0012] recording date and time
	__u8 		Flags;								//[0x0019] file flags
	__u8		UnitSize;							//[0x001A] file unit size
	__u8 		GapSize;							//[0x001B] interleave gap size
	__u8 		LSBVolSeqNum[2];			//[0x001C] lsb volume sequence number
	__u8		MSBVolSeqNum[2];			//[0x001E] msb volume sequence number
	__u8		LenFileIdt;						//[0x0020] length file identifier
}	*Ptr_GrIsoDirRecHead;

typedef struct St_GrIsoVolDes		//primary volume descriptor
{
	__u8		VolType;							//[0x0000] volume descriptor
	char		StdIdt[5];						//[0x0001] standard identifier
	__u8		VolVer;								//[0x0006] volume descriptor version
	__u8		VolFlags;							//[0x0007] primary-unused field-1, supplementary-volume flags
	char 		SysIdt[32];						//[0x0008] system identifier
	char 		VolIdt[32];						//[0x0028] volume identifier
	__u8		_rsvd1[8];						//[0x0048] reserved
	__u8		LSBVolSize[4];				//[0x0050] volume space size
	__u8		MSBVolSize[4];				//[0x0054] volume space size
	__u8		EscSeq[32];						//[0x0058] escape sequence - SVD only
	__u8		LSBVolSetSize[2];			//[0x0078] lsb volume set size
	__u8		MSBVolSetSize[2];			//[0x007A] msb volume set size
	__u8		LSBVolSeqNum[2];			//[0x007C] lsb volume sequence number
	__u8		MSBVolSeqNum[2];			//[0x007E] msb volume sequence number
	__u8		LSBLgcBlkSize[2];			//[0x0080] logical block size
	__u8		MSBLgcBlkSize[2];			//[0x0082] logical block size
	__u8		LSBPathTblSize[4];		//[0x0084] lsb path table size
	__u8		MSBPathTblSize[4];		//[0x0088] msb path table size
	__u8		OcurLPath[4];					//[0x008C] location of occurrence of type L Path Table
	__u8		OptLPath[4];					//[0x0090] location of optional of type L Path Table
	__u8		OcurMPath[4];					//[0x0094] location of occurrence of type M Path Table
	__u8		OptMPath[4];					//[0x0098] location of optional of type M Path Table
	__u8		DirRec[E_GrIsoDirRecRootSize]; 		//[0x009C] directory record for root directory
	__u8		VolSetIdt[128]; 			//[0x00BE] volume set identifier
	__u8 		PubIdt[128];					//[0x013E] publisher identifier
	__u8		DataPreIdt[128];			//[0x01BE] data preparer identifier
	__u8		AppIdt[128];					//[0x023E] application identifier
	__u8 		CopyFileIdt[37];			//[0x02BE] copyright file identifier
	__u8 		AbsFileIdt[37];				//[0x02E3] abstract file identifier
	__u8 		BibFileIdt[37];				//[0x0308] bibliographic file identifier
	__u8 		VolCreateTime[17];		//[0x032D] volume creation date and time
	__u8 		VolModiTime[17];			//[0x033E] volume modification date and time
	__u8		VolExpTime[17];				//[0x034F] volume expiration date and time
	__u8		VolEffTime[17];				//[0x0360] volume effective date and time
	__u8		FileVer;							//[0x0371] file structure version
	__u8		Reserved1;						//[0x0372] reserved
	__u8		AppUse[512];					//[0x0373] application use
	__u8		Reserved2[653];				//[0x0573] reserved
} *Ptr_GrIsoVolDes;

typedef struct St_GrIsoPathHead
{
	__u8		LenDirIdt;						//[0x0000] length of directory identifier
	__u8		ExtAttRecLen;					//[0x0001] extended attribute record length
	__u8		LocatExt[4];					//[0x0002] location of extent
	__u8		ParentDirNum[2];			//[0x0006] parent directory number
	__u8		DirIdt;								//[0x0008] directory identifier
} *Ptr_GrIsoPathHead;

typedef	struct St_GrIsoFileInfo 
{
	Def_WstrName	StrFn;		//file name - name only, can not include directory.
	__u32	Size;
	__u32	SctCnt;						//sector count
	__u32	SctPos;						//sector positon
	__u32	Writed;						//data writed
}	*Ptr_GrIsoFileInfo;

//====================================================================
//class


//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif
