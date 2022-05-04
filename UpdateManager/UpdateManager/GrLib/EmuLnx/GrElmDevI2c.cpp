/*
	emulation linux i2c 

*/

//====================================================================
//include

#include <EmuLnx/GrElmDevI2c.h>
#include <EmuLnx/GrElaBase.h>
#include <EmuLnx/GrElmDevBase.h>
#include <GrDumyTool.h>
#include <GrBufTool.h>
#include <GrGdibTool.h>
#include <GrFileCtrl.h>
#include <GrStrTool.h>


//====================================================================
//local const

//====================================================================
//local type


//====================================================================
//global var

extern	St_GrEmuMdlDrv	V_GrEmuDrvTbl[E_GrEmuMdlMaxDrvCnt];

file_operations			V_GrElmDevI2cFo;


//====================================================================
//functions
//DWORD WINAPI GrElmDevSoloProcThrd( LPVOID A_Para );

int GrEmuDevI2c_ioctl (struct inode * A_PtrInode, struct file * A_PtrFile, DWORD A_Cmd, DWORD A_Para);
int GrEmuDevI2c_open (struct inode * A_PtrInode, struct file * A_PtrFile);
int GrEmuDevI2c_release (struct inode * A_PtrInode, struct file * A_PtrFile);

//--------------------------------------------------------------------
int GrEmuDevI2c_ioctl (struct inode * A_PtrInode, struct file * A_PtrFile, DWORD A_Cmd, DWORD A_Para)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		
		switch ( A_Cmd )
		{
			case I2C_RDWR:
				// 
				Tv_Result	=	0;
				break;
			default:
				Tv_Result	=	-EINVAL;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int GrEmuDevI2c_open (struct inode * A_PtrInode, struct file * A_PtrFile)
{
	// local -------------------
	// code --------------------

		return	0;
}
//--------------------------------------------------------------------
int GrEmuDevI2c_release (struct inode * A_PtrInode, struct file * A_PtrFile)
{
	// local -------------------
	// code --------------------
		return	0;
}
//--------------------------------------------------------------------
void	GrElmDevI2cInit( BYTE A_DevIdx )
{
	// local -------------------
		int		Tv_WkIdx;
		Def_StrName	Tv_StrDev;
	// code --------------------
		// regist
		//check able slot
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrEmuMdlMaxDrvCnt;Tv_WkIdx++ )
		{
			//check able
			if ( !V_GrEmuDrvTbl[Tv_WkIdx].IsUse )
			{
				//success
				GrDumyZeroMem( &V_GrElmDevI2cFo, sizeof(V_GrElmDevI2cFo) );
				V_GrElmDevI2cFo.open		=	GrEmuDevI2c_open;
				V_GrElmDevI2cFo.release	=	GrEmuDevI2c_release;
				V_GrElmDevI2cFo.ioctl		=	GrEmuDevI2c_ioctl;

				V_GrEmuDrvTbl[Tv_WkIdx].IsUse	=	TRUE;
				GrStrCopy( Tv_StrDev, "i2c" );
				Tv_StrDev[4]	=	Tv_StrDev[4] + (char)A_DevIdx;
				GrStrCopy( V_GrEmuDrvTbl[Tv_WkIdx].StrName, Tv_StrDev );
				V_GrEmuDrvTbl[Tv_WkIdx].PtrFo	=	&V_GrElmDevI2cFo;
				break;
			}
		}

}
//--------------------------------------------------------------------
void	GrElmDevI2cFinish( BYTE A_DevIdx )
{
	// local -------------------
	// code --------------------
		
}
//--------------------------------------------------------------------
