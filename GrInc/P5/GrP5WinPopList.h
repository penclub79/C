/*
Platform 5 Window standard

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5WinPopList
#define	_EPC_GrP5WinPopList

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5WinPopItemBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance

#define	E_GrP5WinPopListItemHeightMin		8

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//global class

class Cls_GrP5WinPopList	: public Cls_GrP5WinPopItemBase
{
	protected:

		__u16	m_HostObjId;

		// event system
		virtual	void	EvtClickItem(__u32 A_ItmIdx);


		virtual	void	EvtScptDecDone(void);
		virtual	BOOL8	EvtKeyUp(__u8 A_Key);
		
#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5WinPopList(__u8 A_ConIdx, __u8 A_LayerIdx, Ptr_GrP5RelRect A_PtrRelRect, __u16 A_HostObjId, __u32 A_ViewItemIdx );
		virtual	~Cls_GrP5WinPopList(void);

		// basic information

		// runtime

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoAction(__u32 A_Action);
		virtual	void	DvlPreDelete(void);		// prepare delete
#endif

};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

