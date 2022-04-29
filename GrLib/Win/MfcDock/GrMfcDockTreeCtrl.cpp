/*
MFC docking tree control

*/

//--------------------------------------------------------------------
// uses

#include <Win\MfcDock\GrMfcDockTreeCtrl.h>

#include <GrDebug.h>

/*
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5\GrP5ConBase.h>
*/


//--------------------------------------------------------------------
// local constant

//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var


//====================================================================
//--------------------------------------------------------------------
Cls_GrMfcDockTreeCtrl::Cls_GrMfcDockTreeCtrl() :
CTreeCtrl()
{

}
//--------------------------------------------------------------------
Cls_GrMfcDockTreeCtrl::~Cls_GrMfcDockTreeCtrl()
{

}
//--------------------------------------------------------------------
BEGIN_MESSAGE_MAP(Cls_GrMfcDockTreeCtrl, CTreeCtrl)
	
END_MESSAGE_MAP()
//--------------------------------------------------------------------
void	Cls_GrMfcDockTreeCtrl::ImageSet(UINT A_RsrcId, COLORREF A_ColorKey)
{
	// local -------------------
		CBitmap	Tv_ObjBtm;
		BITMAP	Tv_BtmInfo;
	// code --------------------
		m_ImgList.DeleteImageList();

		if(!Tv_ObjBtm.LoadBitmap(A_RsrcId)) 
		{
			DbgMsgPrint("Cls_GrMfcDockTreeCtrl::ImageSet - Bad resource ID\n");
		}

		Tv_ObjBtm.GetBitmap(&Tv_BtmInfo);

		m_ImgList.Create(16, Tv_BtmInfo.bmHeight, ILC_COLOR24|ILC_MASK, 0, 0);
		m_ImgList.Add(&Tv_ObjBtm, A_ColorKey);

		SetImageList(&m_ImgList, TVSIL_NORMAL);
		
}
//--------------------------------------------------------------------

