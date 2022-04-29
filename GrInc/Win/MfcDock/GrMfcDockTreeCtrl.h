/*
Pane class select

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrMfcDockTreeCtrl
#define	_EPC_GrMfcDockTreeCtrl

//====================================================================
// uses

#include <afxcmn.h>
#include <GrLnxApp.h>
#include <GrTypeBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//global class

class Cls_GrMfcDockTreeCtrl : public CTreeCtrl
{
	private:

	protected:
		CImageList						m_ImgList;


		DECLARE_MESSAGE_MAP()
	public:
		Cls_GrMfcDockTreeCtrl(void);
		virtual	~Cls_GrMfcDockTreeCtrl(void);

		void	ImageSet(UINT A_RsrcId, COLORREF A_ColorKey);

};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

