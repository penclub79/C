/*
	cs351x misc 

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrHwCs351x
#define	_EPC_GrHwCs351x

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
	
//====================================================================
//constance
#define E_GrHwCs351xMaxLedCnt		4

#define E_GrHwCs351xKeyOffFlag	0x80

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class
class Cls_GrHwCs351x
{
	private:
		__s32			m_HndlCir;			//remote contorl driver handle
		__u32		m_CirSeq;				//last index

		__s32			m_HndlGpio;			//gpio driver handle
		__u32		m_LedMap;				//led map
		__u32		m_HwVer;

		__s32			m_HndlUsb[2];			//usb host handle
		__s8*		m_FnTblUsbDev[2];

	protected:

	public:
		Cls_GrHwCs351x( void );
		virtual	~Cls_GrHwCs351x();
		
		__u32	CirNewCodeGet( void );
		void	LedSet( __u32 A_Idx, BOOL8 A_IsOn );
		void	BuzRun( __u32 A_Cnt );
		__u32	HwVersionGet( void );
		__u8	KeypadGet( void );		//key pad value get
		void	UsbBugFixing( void );
		void	VmCacheFlush( void );
		void	SysReboot( void );
};


//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

