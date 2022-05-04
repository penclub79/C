/*
product regist server

*/

//====================================================================
//include

#include <Gl/GrGlBase.h>

#include <GrDebug.h>
#include <GrError.h>

#include <GrDumyTool.h>

#include <Gl/glut.h>

//====================================================================
//local const


//====================================================================
//local type

//====================================================================
//global var

St_GrGlEnv	V_GrGlEnv;

//====================================================================
//functions

//--------------------------------------------------------------------
Def_GrErrCode	GrGlInit(void)
{
	// local -------------------
		HRESULT	Tv_RtCode;
		__u32		Tv_DbgFlag;
	// code --------------------
		// initialize
		Tv_DbgFlag	=	0;
#ifdef _DEBUG
		//Tv_DbgFlag	=	;
#endif

		glShadeModel(GL_SMOOTH);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
		glClearDepth(1.0f); // Depth Buffer Setup
		glEnable(GL_DEPTH_TEST); // Enables Depth Testing
		glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really Nice Perspective Calculations

		return	E_GrErrNone;
}
//--------------------------------------------------------------------

