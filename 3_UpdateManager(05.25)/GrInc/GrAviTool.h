/*
	avi tool
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrAviTool
#define	_EPC_GrAviTool

//====================================================================
// uses

#include	<GrTypeBase.h>
#include	<GrFileCtrl.h>
#include	<GrBmpTool.h>

//====================================================================
//constance
#define E_GrAviVolSize				30
#define E_GrAviMaxFrmCnt			0x30000

#define E_GrAviMhHasIdx				0x00000010
#define E_GrAviMhInterleave		0x00000100
#define E_GrAviMhCktype				0x00000800

#define E_GrAviIdx1FlagIfrm		0x00000010

#define E_GrAviFrmVdoComp			0x00
#define E_GrAviFrmVdoDib			0x01
#define E_GrAviFrmAdo					0x02

//Fourcc struct
#define E_GrAviFccRiff				0x46464952
#define E_GrAviFccAvi					0x20495641
#define E_GrAviFccList				0x5453494C
#define E_GrAviFccHdrl				0x6C726468
#define E_GrAviFccAvih				0x68697661
#define E_GrAviFccStrl				0x6C727473
#define E_GrAviFccStrh				0x68727473
#define E_GrAviFccVids				0x73646976
#define E_GrAviFccAuds				0x73647561
#define E_GrAviFccStrf				0x66727473
#define E_GrAviFccMovi				0x69766F6D
#define E_GrAviFccIdx1				0x31786469
#define E_GrAviFccJunk				0x4B4E554A

#define E_GrAviFccSegm				0x6D676573

#define E_GrAviFccOdml				0x6C6D646F
#define E_GrAviFccDmlh				0x6C6D646F

#define E_GrAviFcc00dc				0x63643030
#define E_GrAviFcc01wb				0x62773130

//junk
#define E_GrAviEcxFcc					0x5843454A

//audio id
#define E_GrAviFccPcm					0x0001
#define E_GrAviFccAdpcm				0x0002
#define E_GrAviFccAlaw				0x0006
#define E_GrAviFccMulaw				0x0007
#define E_GrAviFccG723Antex		0x0014
#define E_GrAviFccG723Ms			0x0042
#define E_GrAviFccG723Lucent	0x0059
#define E_GrAviFccG723Msonic	0x0093
#define E_GrAviFccG723_1_Rc		0x00A3
#define E_GrAviFccG723Vivo		0x0111
#define E_GrAviFccG723Dec			0x0123
#define E_GrAviFccG723_1_Is		0xA100


//====================================================================
//global macro


//====================================================================
//global type

typedef struct St_GrAviListHd
{
	__u32 FccMain;				//fcc list
	__u32	Size;						//size of contents - 4
	__u32 FccSub;					//fcc of contents
} *Ptr_GrAviListHd;

typedef struct St_GrAviChunkHd
{
	__u32 Fcc;						//fourcc
	__u32 Size;						//size of contents
} *Ptr_GrAviChunkHd;

typedef struct St_GrAviMainHd 
{
	__u32		FrmStepUs;			//1000000 / framepersec
	__u32		MaxBytePerSec;	//can 0
	__u32		Align;					//can 0
	__u32		Flag;						//E_GrAviMhHasIdx | E_GrAviMhInterleave | E_GrAviMhCktype
	__u32		FrmCnt;					//video frame count
	__u32		InitFrm;				//interleave startframe - default = 0
	__u32		StrmCnt;				//stream type count
	__u32		SuggestBufSize;	//default 0
	__u32		ResX;						//video resolution x
	__u32		ResY;						//video resolution y
	__u32		_rsvd[4];				//reserved
} *Ptr_GrAviMainHd;

typedef struct St_GrAviStrmHd
{
	__u32		FccType;			//video = vids , audio = auds
	__u32		FccCodec;			
	__u32		Flag;					//always = 0
	__u16		Priority;			//always = 0
	__u16		Language;			//always = 0
	__u32		InitialFrames;	//always = 0
	__u32		Scale;				//video = 1, audio = byte of frame
	__u32		Rate;					//video = frame rate , audio = byte of sec
	__u32		Start;				//always = 0
	__u32		Length;				//total frame count
	__u32		ReqBufSize;		//video = 0x2700 audio = 0x1b0- max frame size
	__u32		Quality;			//always = 0 - 10000
	__u32		SmpSize;			//always = 0, audio = block align
	__s16		left;				//always = 0
	__s16		top;				//always = 0
	__s16		right;			//video = resx, audio = 0
	__s16		bottom;			//video = resy, audio = 0
}  *Ptr_GrAviStrmHd;

typedef struct	St_GrAviWaveformatEx	
{ 
	__u16		FormatId;				//format id
	__u16		ChCnt;					//channel count
	__u32		SmpRate;				//sample rate
	__u32		TransSize;			//average size per sec
	__u16		Align;					//block align - one block size
	__u16		SmpBit;					//sample bit - none pcm = 0, maybe g723 = 3
	//__u16		Size;						//total size
} *Ptr_GrAviWaveformatEx; 

typedef struct St_GrAviWfextG723 
{
	__u16    ExtSize;							//extra size default = 2
	__u16    AuxBlkSize;						//aux block size default = 0	
} *Ptr_GrAviWfextG723;

typedef struct St_GrAviIdx1 
{
	__u32   Fcc;					//frame fourcc
	__u32   Flags;				//frag - ifrfmae = E_GrAviIdx1FlagIfrm, audio = always key frame
	__u32   Ofs;					//offset from List movi, include offset movi
	__u32   Size;					//frame size - chunk header size not include
}	*Ptr_GrAviIdx1;

//file headers

typedef	struct St_GrAviFhAvih
{
	St_GrAviChunkHd		Hd;
	St_GrAviMainHd		Ct;		//avi main header
}	*Ptr_GrAviFhAvih;

typedef	struct St_GrAviFhStrh
{
	St_GrAviChunkHd		Hd;	
	St_GrAviStrmHd		Ct;	
}	*Ptr_GrAviFhStrh;

typedef	struct St_GrAviFhStrfVdo
{
	St_GrAviChunkHd		Hd;	
	St_GrBmpIfHd			Ct;	
}	*Ptr_GrAviFhStrfVdo;

typedef	struct St_GrAviFhStrfAdo
{
	St_GrAviChunkHd		Hd;	
	St_GrAviWaveformatEx	Ct;	
}	*Ptr_GrAviFhStrfAdo;

typedef	struct St_GrAviFhStrlVdo
{
	St_GrAviListHd		Hd;	
	St_GrAviFhStrh		Strh;
	St_GrAviFhStrfVdo	Strf;
}	*Ptr_GrAviFhStrlVdo;

typedef	struct St_GrAviFhStrlAdo 
{
	St_GrAviListHd		Hd;	
	St_GrAviFhStrh		Strh;
	St_GrAviFhStrfAdo	Strf;
}	*Ptr_GrAviFhStrlAdo;

typedef	struct St_GrAviSegm 
{
	St_GrAviChunkHd	Hd;
	__u16		Ct;
}	*Ptr_GrAviSegm;

typedef	struct St_GrAviFhHdrl
{
	St_GrAviListHd		Hd;
	St_GrAviFhAvih		Avih;
	St_GrAviFhStrlVdo	StrlVdo;
	St_GrAviFhStrlAdo	StrlAdo;
	//St_GrAviSegm			Segm;
}	*Ptr_GrAviFhHdrl;

typedef	struct St_GrAviFhRiff 
{
	St_GrAviListHd		Hd;			//riff header
	St_GrAviFhHdrl		Hdrl;		//contents
}	*Ptr_GrAviFhRiff;

typedef	struct St_GrAviFhSpcJunk 
{
	__u32		Fcc;				//special junk fcc
	__u32		Val;				//value
	Def_StrNum	Str;
}	*Ptr_GrAviFhSpcJunk;

//====================================================================
//class

class Cls_GrAviTool
{
	private:
		Cls_GrFileCtrl*	m_ObjFl;
		BOOL8		m_IsOpen;
		BOOL8		m_IsWrite;

		__u32		m_AdoSmpRate;
		__u32		m_AdoSmpBit;
		__u32		m_AdoBlkSize;

		__u32		m_VfrmMaxSize;
		__u32		m_VfrmCnt;
		__u32		m_AfrmMaxSize;
		__u32		m_AfrmCnt;
		__u32		m_AviSize;

		__u32		m_WtmVal;

		//write part
		__u8		m_VolHd[E_GrAviVolSize];
		St_GrAviListHd	m_WtMovi;			//movi packet
		Ptr_GrAviFhRiff	m_PtrWtHd;		//write header pointer
		Ptr_GrAviIdx1		m_PtrWtIdx1;

		//read part
		void*		m_PtrRdHd;		//read header pointer
		__u32		m_RdHdSize;		//header size
		Ptr_GrAviMainHd	m_PtrRdMh;		//main header pointer
		Ptr_GrAviStrmHd	m_PtrStrhVdo;	//video stream header pointer
		Ptr_GrAviStrmHd	m_PtrStrhAdo;	//audio stream header pointer
		Ptr_GrBmpIfHd		m_PtrRdVf;				//video foramt header pointer
		Ptr_GrAviWaveformatEx	m_PtrRdAf;	//audio foramt header pointer
		__u32						m_FptrRdMovi;			//movi file pointer
		__u32						m_RdMoviSize;					//read movi
		__u32						m_FptrRdIdx1;		//idx1 file pointer
		__u32						m_RdIdx1Cnt;			//idx1 chunk header
		BOOL8						m_IsRdWtmExist;	//watermark value exist
		__u32						m_RdWtmPos;			//read watermakr porcess count
		__u32						m_RdWtmVal;			//read watermark value
		__u32						m_FptrRdFrm;		//read frame positon

	protected:
		void	LcPatch( __u32 A_FccVdo, __u16 A_AdoId );
		void	LcPtcG723( void );
		void	LcAddJunk( __u32 A_UserVal, void* A_PtrChuk, __u32 A_Size );
		void	LcAddOdml( void* A_PtrChuk, __u32 A_Size );
		BOOL8	LcParsAviHd( void );
		BOOL8	LcParsCont( void );
	public:
		Cls_GrAviTool( Cls_GrFileCtrl* A_ObjFile );
		virtual	~Cls_GrAviTool();
		
		//write
		BOOL8	StartWrite( __u32 A_FccVdo, __u32 A_Fps, __u32 A_ResX, __u32 A_ResY,
					__u16 A_AdoId, __u32 A_ChCnt, __u32 A_SmpRate, __u32 A_SmpBit, __u32 A_AdoBlkUnit );
		BOOL8	AddVdoFrm( void* A_PtrData, __u32 A_DataSize, BOOL8 A_IsIfrm );
		BOOL8	AddAdoFrm( void* A_PtrData, __u32 A_DataSize );
		BOOL8	FinishWrite( void );

		//read
		BOOL8	StartRead( void );
		BOOL8	GetFrm( void* A_PtrBuf, __u32* A_PtrRtSize, BOOL8* A_PtrRtIsAdo );
		BOOL8	MoveReadPos( __u32 A_MovePos );
		BOOL8	FinishRead( void );
		BOOL8	IsWtmFinished( void );
		BOOL8	WtmChk( void );							//work frame count
		BOOL8	IsWtmOk( void );
		__u32	WtmPrsCnt( void );				//watermark process count

		//info
		BOOL8	GetVdoInfo( __u32* A_PtrRtFcc, __u32* A_PtrRtFps, 
											__u32* A_PtrRtResX, __u32* A_PtrRtResY );
		BOOL8	GetAdoInfo( __u16* A_PtrRtId, __u32* A_PtrRtCh, 
											__u32* A_PtrSmpRate, __u32* A_PtrRtSmpBit );
		__u32	GetMaxStrmSize( void );
		__u32	GetMovePointCnt( void );
};

//====================================================================
//global function
	
//====================================================================
//out Enviroment conditional
#endif
