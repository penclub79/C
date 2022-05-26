/*
	Platform 5 Developer manager

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5DvlMng
#define	_EPC_GrP5DvlMng

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>

#include <GrError.h>

#include <P5/GrP5Mng.h>

#include <GrTaskSvr.h>
#include <GrTaskCli.h>
#include <GrLayerFile.h>

//====================================================================
//constance

#define	E_GrP5DvlMngDfltWinSizeX			1024
#define	E_GrP5DvlMngDfltWinSizeY			768



//====================================================================
//global macro


//====================================================================
//global type


//====================================================================
//global class

class Cls_GrP5DvlMng : public Cls_GrP5Mng
{
	private:
		BOOL8	m_DvlIsLoadOk;

	protected:
		void	DvlInitText(void);

		void	DvlnitPrj(void);
		void	DvlScptFreeAll(void);
		void	DvlImageFreeAll(void);

		// project save
		BOOL8	DvlPrjSaveFont(Cls_GrLayerFileSave* A_ObjLf);
		BOOL8	DvlPrjSaveImage(Cls_GrLayerFileSave* A_ObjLf);
		BOOL8	DvlPrjSaveUiScript(Cls_GrLayerFileSave* A_ObjLf);
		BOOL8	DvlPrjSaveText(Cls_GrLayerFileSave* A_ObjLf);

		// project load
		void	DvlPrjLoadFont(Cls_GrLayerFileLoad* A_ObjLf, __u32 A_FontId,__u32 A_Size);
		void	DvlPrjLoadImage(Cls_GrLayerFileLoad* A_ObjLf, __u32 A_ImgId, __u32 A_Size);
		void	DvlPrjLoadWinScript(Cls_GrLayerFileLoad* A_ObjLf, __u32 A_ScptId, __u32 A_Size);
		void	DvlPrjLoadWinInfo(Cls_GrLayerFileLoad* A_ObjLf, __u32 A_Size);
		void	DvlPrjLoadText(Cls_GrLayerFileLoad* A_ObjLf, __u32 A_Size, __u32 A_TextId, __u32 A_Language);

	public:
		Cls_GrP5DvlMng(Cls_GrTaskSvr* A_TaskSvr);
		virtual	~Cls_GrP5DvlMng(void);

		// inherited

		// control

		BOOL8	DvlWindowScriptCreate(__u16 A_ScptId,__u16 A_ClassId = E_GrP5ClassIdWinStd);
		BOOL8	DvlWindowScriptDelete(__u16 A_ScptId);
		BOOL8	DvlWindowScriptSet(__u16 A_ScptId, void* A_PtrScpt, __u32 A_ScptSize);

		BOOL8	DvlFontScriptSet(__u8 A_FontId, void* A_PtrScpt, __u32 A_Size, BOOL8 A_IsAutoScptFree);
		BOOL8	DvlFontScriptSave(__u8 A_FontId,WCHAR* A_StrFn );
		BOOL8	DvlFontDelete(__u8 A_FontId);

		BOOL8	DvlImageSet(__u16 A_ImgId, Cls_GrGdib* A_ObjGdib);
		BOOL8	DvlImageSet(__u16 A_ImgId, Ptr_GrGdib A_PtrGdib);

		void	DvlPrjReset(void);
		BOOL8	DvlPrjSave(WCHAR* A_StrFn, BOOL8 A_IsAddPrjInfo);
		BOOL8	DvlPrjLoad(WCHAR* A_StrFn);

};


//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

