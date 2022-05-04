/*
	solo register
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_RegSolo
#define	_EPC_RegSolo

//====================================================================
// uses
#ifndef LINUX_DRV
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#endif
	
//====================================================================
//constance
#define	E_SoloVid										0x9413
#define	E_SoloPid6010								0x6010		/* 6010 product id */
#define	E_SoloPid6110								0x6110		/* 6110 product id */

#define E_SoloPciRegTrdyTimeout			0x0040		/* pci register - TrdyTimeout : byte */
#define E_SoloPciRegRetryTimeout		0x0041		/* pci register - RetryTimeout : byte */

#define	E_SoloLocalMemSize					0x08000000	/* 128 MByte */
#define	E_SoloRegAreaSize						0x01000

#define E_SoloMaxChCnt							16
#define E_SoloVencIdxTblSize				64
#define E_SoloP2mChCnt							4
#define E_SoloVencQueCnt						16
#define E_SoloUartChCnt							2
#define E_SoloI2cChCnt							8
#define E_SoloEncMtnMapSize					256

#define E_SoloVopI									0

#define E_SoloLmemAlignUnit					16
#define E_SoloLmemAlignMask					0xFFFFFFF0

#define E_SoloBaseResX							704
#define E_SoloNtscResY							480
#define E_SoloPalResY								576
#define E_SoloNtscFps								30
#define E_SoloPalFps								25

#define E_SoloUserDispPageCnt				4

#define E_Solo6010CodecPwr					6			/* codec speed */
#define E_Solo6110CodecPwr					9			/* codec speed */
#define E_Solo6110EncPwr						5

#define E_SoloDispBarPosCnt					5			/* bar line display count */

#define E_SoloOsgLmemPitch					2048			/* osg local memory pitch */

#define E_SoloDecRefUnit						0x00140000		/*	decode reference unit size */

#define E_SoloAdoBufPitch						1024			/* audio buffer pitch */
#define E_SoloAdoHwQueCnt						32				/* audio buffer queue count */
#define E_SoloAdoChUnitSize					48				/* audio item unit size */
#define E_SoloAdoDecBufPos					0x8000		/* audio decode buffer positon */

#define E_SoloP2mDescSize						(256*16)	/* solo p2m description size */

#define	E_SoloRa_SYS_CONFIG					0x0000
#define		E_SoloRv_SYS_CONFIG_RESET						(1<<31)		// SDRAM SW RESET
#define		E_SoloRv_SYS_CONFIG_SDRAM64BIT				(1<<30)		// SDRAM data width	select : 0=32 bit,1=64 bit mode
#define		E_SoloRv_SYS_CONFIG_PCLK_DELAY(n)		((n)<<26)
#define		E_SoloRv_SYS_CONFIG_NCLK_DELAY(n)		((n)<<24)
#define		E_SoloRv_SYS_CONFIG_CLOCK_DIVIDE			(1<<19)
#define		E_SoloRv_SYS_CONFIG_INPUTDIV(n)			((n)<<14)
#define		E_SoloRv_SYS_CONFIG_FEEDBACKDIV(n)		((n)<<5)
#define		E_SoloRv_SYS_CONFIG_OUTDIV(n)				((n)<<3)
#define		E_SoloRv_SYS_CONFIG_PLL_PWDN					(1<<2)
#define		E_SoloRv_SYS_CONFIG_PLL_BYPASS				(1<<1)
#define		E_SoloRv_SYS_CONFIG_FOUT_EN					(1<<0)

#define	E_SoloRa_DMA_CTRL					0x0004
#define		E_SoloRv_DMA_CTRL_REFRESH_CYCLE(n)		((n)<<8)	// refresh cycle
#define		E_SoloRv_DMA_CTRL_SDRAM_SIZE(n)			((n)<<6)	// sdram size(32bit/64bit) : 0=16/32MB, 1=32/64MB, 2=64/128MB, 3=128/256MB
#define		E_SoloRv_DMA_CTRL_SDRAM_CLK_INVERT		(1<<5)		// sdram clock invert
#define		E_SoloRv_DMA_CTRL_STROBE_SELECT			(1<<4)		//
#define		E_SoloRv_DMA_CTRL_READ_DATA_SELECT		(1<<3)		//
#define		E_SoloRv_DMA_CTRL_READ_CLK_SELECT		(1<<2)		//
#define		E_SoloRv_DMA_CTRL_LATENCY(n)					((n)<<0)	// latency

#define	E_SoloRa_DMA_EXT					0x0008
#define		E_SoloRv_DMA_EXT_SDRAM_QUESPC				(3<<8)		//unknown function

#define E_SoloRa_SYS_VCLK					0x000C
#define		E_SoloRv_SYS_CONFIG_VOUT_CLK_INVERT		(1<<22)		// video output clock invert
#define		E_SoloRv_SYS_CONFIG_VOUT_CLK_SELECT(n)	((n)<<20)	// video output clock select : 0=sys_clk/4, 1=sys_clk/2, 2=clk_in/2 of system input
#define		E_SoloRv_SYS_CONFIG_VIN1415_DELAY(n)		((n)<<14)	// video input clock delay ch14,15
#define		E_SoloRv_SYS_CONFIG_VIN1213_DELAY(n)		((n)<<12)	// video input clock delay ch12,13
#define		E_SoloRv_SYS_CONFIG_VIN1011_DELAY(n)		((n)<<10)	// video input clock delay ch10,11
#define		E_SoloRv_SYS_CONFIG_VIN0809_DELAY(n)		((n)<<8)	// video input clock delay ch8,9
#define		E_SoloRv_SYS_CONFIG_VIN0607_DELAY(n)		((n)<<6)	// video input clock delay ch6,7
#define		E_SoloRv_SYS_CONFIG_VIN0405_DELAY(n)		((n)<<4)	// video input clock delay ch4,5
#define		E_SoloRv_SYS_CONFIG_VIN0203_DELAY(n)		((n)<<2)	// video input clock delay ch2,3
#define		E_SoloRv_SYS_CONFIG_VIN0001_DELAY(n)		((n)<<0)	// video input clock delay ch0,1

/*
*	INTERRUPT
*/

#define	E_SoloRa_INTR_STAT					0x0010
#define E_SoloRa_INTR_ENABLE				0x0014
#define		E_SoloRv_INTR_P2M(n)					(1<<((n)+17))
#define		E_SoloRv_INTR_GPIO						(1<<16)
#define		E_SoloRv_INTR_VIDEO_LOSS			(1<<15)
#define		E_SoloRv_INTR_VIDEO_IN				(1<<14)
#define		E_SoloRv_INTR_MOTION					(1<<13)
#define		E_SoloRv_INTR_ATA_CMD					(1<<12)
#define		E_SoloRv_INTR_ATA_DIR					(1<<11)
#define		E_SoloRv_INTR_PCI_ERR					(1<<10)
#define		E_SoloRv_INTR_PS2_1						(1<<9)
#define		E_SoloRv_INTR_PS2_0						(1<<8)
#define		E_SoloRv_INTR_SPI							(1<<7)
#define		E_SoloRv_INTR_IIC							(1<<6)
#define		E_SoloRv_INTR_UART1						(1<<5)
#define		E_SoloRv_INTR_UART0						(1<<4)
#define		E_SoloRv_INTR_G723						(1<<3)
#define		E_SoloRv_INTR_DECODER					(1<<1)
#define		E_SoloRv_INTR_ENCODER					(1<<0)

/* chip info */
#define	E_SoloRa_TEST_MODE					0x001C

/* 6110 PLL */
#define E_SoloRa_6110_PLL						0x0020
#define		E_SoloRv_6110_PLL_Base				0x00000002
#define		E_SoloRv_6110_PLL_DivF(n)			((n)<<4)
#define		E_SoloRv_6110_PLL_DivQ(n)			((n)<<12)
#define		E_SoloRv_6110_PLL_DivR(n)			((n)<<15)
#define		E_SoloRv_6110_PLL_Range(n)		((n)<<20)

/*
* ERROR
*/
#define E_SoloRa_PCI_ERROR					0x0070
#define		E_SoloRv_PCI_ERROR_FSM2(stat)	(0x1f&((stat)>>24))
#define		E_SoloRv_PCI_ERROR_FSM1(stat)	(0x0f&((stat)>>20))
#define		E_SoloRv_PCI_ERROR_FSM0(stat)	(0x0f&((stat)>>16))
#define		E_SoloRv_PCI_ERROR_P2M_DESC		(1<<6)
#define		E_SoloRv_PCI_ERROR_ATA					(1<<5)
#define		E_SoloRv_PCI_ERROR_P2M					(1<<4)
#define		E_SoloRv_PCI_ERROR_TIMEOUT			(1<<3)
#define		E_SoloRv_PCI_ERROR_TARGET			(1<<2)
#define		E_SoloRv_PCI_ERROR_PARITY			(1<<1)
#define		E_SoloRv_PCI_ERROR_FATAL				(1<<0)

/*
*	P2M
*/
#define E_SoloRa_P2M_BASE					0x0080

#define E_SoloRa_P2M_CONFIG(n)			(0x0080 + ((n)*0x20))
#define		E_SoloRv_P2M_DMA_INTERVAL(n)			((n)<<6)			// N*32 clocks
#define		E_SoloRv_P2M_CSC_BYTE_REORDER		(1<<5)				// BGR -> RGB
#define		E_SoloRv_P2M_CSC_16BIT_565				(1<<4)				// 0:r=[14:10] g=[9:5] b=[4:0], 1:r=[15:11] g=[10:5] b=[4:0]
#define		E_SoloRv_P2M_UV_SWAP							(1<<3)
#define		E_SoloRv_P2M_PCI_MASTER_MODE			(1<<2)
#define		E_SoloRv_P2M_DESC_INTR_OPT				(1<<1)				// 1:Empty Desc, 0:Each Command End
#define		E_SoloRv_P2M_DESC_MODE						(1<<0)

#define E_SoloRa_P2M_DES_ADR(n)				(0x0084 + ((n)*0x20))

#define E_SoloRa_P2M_DESC_ID(n)				(0x0088 + ((n)*0x20))
#define		E_SoloRv_P2M_UPDATE_ID(n)				((n)<<0)

#define E_SoloRa_P2M_STATUS(n)				(0x008C + ((n)*0x20))
#define		E_SoloRv_P2M_COMMAND_DONE				(1<<8)
#define		E_SoloRv_P2M_CURRENT_ID(stat)		(0xff & (stat))

#define E_SoloRa_P2M_CONTROL(n)				(0x0090 + ((n)*0x20))
#define		E_SoloRv_P2M_PCI_INC(n)					((n)<<20)
#define		E_SoloRv_P2M_REPEAT(n)						((n)<<10)
#define		E_SoloRv_P2M_BURST_SIZE(n)				((n)<<7)			// 0:512, 1:256, 2:128, 3:64, 4:32, 5:128(2page)
#define			E_SoloRv_P2M_BURST_512					0
#define			E_SoloRv_P2M_BURST_256					1
#define			E_SoloRv_P2M_BURST_128					2
#define			E_SoloRv_P2M_BURST_64					3
#define			E_SoloRv_P2M_BURST_32					4
#define		E_SoloRv_P2M_CSC_16BIT						(1<<6)				// 0:24bit, 1:16bit
#define		E_SoloRv_P2M_ALPHA_MODE(n)				((n)<<4)			// 0:Y[0]<-0(OFF), 1:Y[0]<-1(ON), 2:Y[0]<-G[0], 3:Y[0]<-Bit[15]
#define		E_SoloRv_P2M_CSC_ON							(1<<3)
#define		E_SoloRv_P2M_INTERRUPT_REQ				(1<<2)
#define		E_SoloRv_P2M_WRITE								(1<<1)
#define		E_SoloRv_P2M_TRANS_ON						(1<<0)

#define E_SoloRa_P2M_EXT_CFG(n)				(0x0094 + ((n)*0x20))
#define		E_SoloRv_P2M_EXT_INC(n)					((n)<<20)
#define		E_SoloRv_P2M_COPY_SIZE(n)				((n)<<0)

#define E_SoloRa_P2M_TAR_ADR(n)				(0x0098 + ((n)*0x20))

#define E_SoloRa_P2M_EXT_ADR(n)				(0x009C + ((n)*0x20))

#define E_SoloRa_P2M_BUFFER						0x2000

/*
* VIN
*/
#define E_SoloRa_VI_CH_SWITCH_0				0x0100
#define E_SoloRa_VI_CH_SWITCH_1				0x0104
#define E_SoloRa_VI_CH_SWITCH_2				0x0108

#define	E_SoloRa_VI_CH_ENA						0x010C
#define	E_SoloRa_VI_CH_FORMAT				0x0110
#define		E_SoloRv_VI_FD_SEL_MASK(n)				((n)<<16)
#define		E_SoloRv_VI_PROG_MASK(n)					((n)<<0)

#define E_SoloRa_VI_FMT_CFG					0x0114
#define		E_SoloRv_VI_FMT_CHECK_VCOUNT				(1<<31)
#define		E_SoloRv_VI_FMT_CHECK_HCOUNT				(1<<30)

#define	E_SoloRa_VI_PAGE_SW					0x0118
#define		E_SoloRv_FI_INV_DISP_LIVE(n)				((n)<<8)
#define		E_SoloRv_FI_INV_DISP_OUT(n)				((n)<<7)
#define		E_SoloRv_DISP_SYNC_FI(n)						((n)<<6)
#define		E_SoloRv_PIP_PAGE_ADD(n)						((n)<<3)
#define		E_SoloRv_NORMAL_PAGE_ADD(n)				((n)<<0)

#define	E_SoloRa_VI_ACT_I_P					0x011C
#define	E_SoloRa_VI_ACT_I_S					0x0120
#define	E_SoloRa_VI_ACT_P						0x0124
#define		E_SoloRv_VI_FI_INVERT					(1<<31)
#define		E_SoloRv_VI_H_START(n)					((n)<<21)
#define		E_SoloRv_VI_V_START(n)					((n)<<11)
#define		E_SoloRv_VI_V_STOP(n)					((n)<<0)

#define E_SoloRa_VI_STATUS0					0x0128
#define E_SoloRa_VI_STATUS1					0x012C

// Video PB to Live
#define E_SoloRa_VI_PB_CONFIG				0x0130
#define		E_SoloRv_VI_PB_USERMODE			(1 << 1)
#define		E_SoloRv_VI_PB_PAL						(1 << 0)	
#define E_SoloRa_VI_PB_RANGE_HV			0x0134
#define		E_SoloRv_VI_PB_RNG_HSIZE(n)	((n)<<12)
#define		E_SoloRv_VI_PB_RNG_VSIZE(n)	((n)<<0)
#define E_SoloRa_VI_PB_ACT_H					0x0138
#define		E_SoloRv_VI_PB_ACT_H_START(n)	((n)<<12)
#define		E_SoloRv_VI_PB_ACT_H_STOP(n)		((n)<<0)
#define E_SoloRa_VI_PB_ACT_V					0x013C
#define		E_SoloRv_VI_PB_ACT_V_START(n)	((n)<<12)
#define		E_SoloRv_VI_PB_ACT_V_STOP(n)		((n)<<0)

// Mosaic
#define	E_SoloRa_VI_MOSAIC(ch)				(0x0140 + ((ch)*4))
#define		E_SoloRv_VI_MOSAIC_SX(x)					((x)<<24)
#define		E_SoloRv_VI_MOSAIC_EX(x)					((x)<<16)
#define		E_SoloRv_VI_MOSAIC_SY(x)					((x)<<8)
#define		E_SoloRv_VI_MOSAIC_EY(x)					((x)<<0)

// Window
#define	E_SoloRa_VI_WIN_CTRL0(ch)		(0x0180 + ((ch)*4))
#define		E_SoloRv_VI_WIN_CHANNEL(n)			((n)<<28)
#define		E_SoloRv_VI_WIN_PIP						(1<<27)		//	27
#define		E_SoloRv_VI_WIN_SCALE(n)				((n)<<24)		//	23..26

#define		E_SoloRv_VI_WIN_SX(x)					((x)<<12)		//	12..23
#define		E_SoloRv_VI_WIN_EX(x)					((x)<<0)		// 	 0..11

#define		E_SoloRv_VI_WIN_SclOff					0
#define		E_SoloRv_VI_WIN_Scl1_1					1
#define		E_SoloRv_VI_WIN_Scl1_2					3
#define		E_SoloRv_VI_WIN_Scl1_3					4
#define		E_SoloRv_VI_WIN_Scl1_4					5
#define		E_SoloRv_VI_WIN_Scl2_3					6
#define		E_SoloRv_VI_WIN_Scl3_4					7

#define	E_SoloRa_VI_WIN_CTRL1(ch)				(0x01C0 + ((ch)*4))
#define		E_SoloRv_VI_WIN_SY(x)								((x)<<12)
#define		E_SoloRv_VI_WIN_EY(x)								((x)<<0)

#define	E_SoloRa_VI_LIVE_ON(ch)					(0x0200 + ((ch)*4))
#define		E_SoloRv_VI_LIVE_LIVE								1
#define		E_SoloRv_VI_LIVE_PLAY								2
#define E_SoloRa_VI_WIN_SW							0x0240
#define E_SoloRa_VI_WIN_LIVE_AUTO_MUTE	0x0244

//expantion
#define E_SoloRa_VO_EXPANSION(id)				(0x0250+((id)*4))

// Motion
#define	E_SoloRa_VI_MOT_ADR							0x0260
#define		E_SoloRv_VI_MOTION_EN(mask)					(mask<<16)
#define	E_SoloRa_VI_MOT_CTRL						0x0264
#define		E_SoloRv_VI_MOTION_FRAME_COUNT(n)		((n)<<24)
#define		E_SoloRv_VI_MOTION_SAMPLE_LENGTH(n)	((n)<<16)
#define		E_SoloRv_VI_MOTION_INTR_START_STOP	(1<<15)
#define		E_SoloRv_VI_MOTION_FREEZE_DATA			(1<<14)
#define		E_SoloRv_VI_MOTION_SAMPLE_COUNT(n)	((n)<<0)

#define E_SoloRa_VI_MOT_CLEAR						0x0268
#define	E_SoloRa_VI_MOT_STATUS					0x026C
#define		E_SoloRv_VI_MOTION_CNT(n)						(n<<0)
#define E_SoloRa_VI_MOTION_BORDER				0x0270
#define E_SoloRa_VI_MOTION_BAR					0x0274

/*
*	VIDEO OUTPUT
*/
#define	E_SoloRa_VO_FMT_ENC							0x0300
#define		E_SoloRv_VO_SCAN_MODE_PROGRESSIVE		(1<<31)
#define		E_SoloRv_VO_FMT_TYPE_PAL						(1<<30)
#define		E_SoloRv_VO_USER_SET								(1<<29)
#define		E_SoloRv_VO_FI_CHANGE								(1<<20)
#define		E_SoloRv_VO_USER_COLOR_SET_VSYNC		(1<<19)
#define		E_SoloRv_VO_USER_COLOR_SET_HSYNC		(1<<18)
#define		E_SoloRv_VO_USER_COLOR_SET_NAV			(1<<17)
#define		E_SoloRv_VO_USER_COLOR_SET_NAH			(1<<16)
#define		E_SoloRv_VO_NA_COLOR_Y(Y)						((Y)<<8)
#define		E_SoloRv_VO_NA_COLOR_CB(CB)					((CB) & 0xF0)
#define		E_SoloRv_VO_NA_COLOR_CR(CR)					((CR)>>4)

#define	E_SoloRa_VO_ACT_H								0x0304
#define		E_SoloRv_VO_H_BLANK(n)							((n)<<22)
#define		E_SoloRv_VO_H_START(n)							((n)<<11)
#define		E_SoloRv_VO_H_STOP(n)								((n)<<0)

#define	E_SoloRa_VO_ACT_V								0x0308
#define		E_SoloRv_VO_V_BLANK(n)							((n)<<22)
#define		E_SoloRv_VO_V_START(n)							((n)<<11)
#define		E_SoloRv_VO_V_STOP(n)								((n)<<0)

#define	E_SoloRa_VO_RANHE_HV						0x030C
#define		E_SoloRv_VO_SYNC_INVERT							(1<<24)
#define		E_SoloRv_VO_HSYNC_INVERT						(1<<23)
#define		E_SoloRv_VO_VSYNC_INVERT						(1<<22)
#define		E_SoloRv_VO_H_LEN(n)								((n)<<11)
#define		E_SoloRv_VO_V_LEN(n)								((n)<<0)

#define	E_SoloRa_VO_DISP_CTRL						0x0310
#define		E_SoloRv_VO_DISP_ON									(1<<31)
#define		E_SoloRv_VO_DISP_ERASE_COUNT(n)			((n)<<24)	// auto erase page count
#define		E_SoloRv_VO_DISP_DOUBLE_SCAN				(1<<22)		// max page set to 2
#define		E_SoloRv_VO_DISP_SINGLE_PAGE				(1<<21)
#define		E_SoloRv_VO_DISP_BASE(n)						(((n)>>16) & 0xffff)	// n * 64KB (display memory page offset)

#define E_SoloRa_VO_DISP_ERASE					0x0314
#define		E_SoloRv_VO_DISP_ERASE_ON						(1<<0)

#define	E_SoloRa_VO_ZOOM_CTRL				0x0318
#define		E_SoloRv_VO_ZOOM_VER_ON					(1<<24)
#define		E_SoloRv_VO_ZOOM_HOR_ON					(1<<23)
#define		E_SoloRv_VO_ZOOM_V_COMP					(1<<22)		// vertical compensation when vertical zoon
#define		E_SoloRv_VO_ZOOM_SX(h)						(((h)/2)<<11)	// zoom offset of horizontal *2
#define		E_SoloRv_VO_ZOOM_SY(v)						(((v)/2)<<0)	// zoom vertical offset

#define E_SoloRa_VO_FREEZE_CTRL				0x031C
#define		E_SoloRv_VO_FREEZE_ON						(1<<1)
#define		E_SoloRv_VO_FREEZE_INTERPOLATION	(1<<0)

#define	E_SoloRa_VO_BKG_COLOR				0x0320
#define		E_SoloRv_BG_Y(y)									((y)<<16)
#define		E_SoloRv_BG_U(u)									((u)<<8)
#define		E_SoloRv_BG_V(v)									((v)<<0)

#define	E_SoloRa_VO_DEINTERLACE				0x0324
#define		E_SoloRv_VO_DEINTERLACE_THRESHOLD(n)		((n)<<8)
#define		E_SoloRv_VO_DEINTERLACE_EDGE_VALUE(n)	((n)<<0)

#define E_SoloRa_VO_BORDER_LINE_COLOR			0x0330
#define E_SoloRa_VO_BORDER_FILL_COLOR			0x0334
#define E_SoloRa_VO_BORDER_LINE_MASK			0x0338
#define E_SoloRa_VO_BORDER_FILL_MASK			0x033c

#define E_SoloRa_VO_BORDER_X(n)						(0x0340+((n)*4))
#define E_SoloRa_VO_BORDER_Y(n)						(0x0354+((n)*4))

#define E_SoloRa_VO_CELL_EXT_SET					0x0368
#define E_SoloRa_VO_CELL_EXT_START				0x036c
#define E_SoloRa_VO_CELL_EXT_STOP					0x0370

#define E_SoloRa_VO_CELL_EXT_SET2					0x0374
#define E_SoloRa_VO_CELL_EXT_START2				0x0378
#define E_SoloRa_VO_CELL_EXT_STOP2				0x037c

#define E_SoloRa_VO_RECTANGLE_CTRL(n)		(0x0368+((n)*12))
#define E_SoloRa_VO_RECTANGLE_START(n)		(0x036c+((n)*12))
#define E_SoloRa_VO_RECTANGLE_STOP(n)		(0x0370+((n)*12))

#define E_SoloRa_VO_CURSOR_POS					(0x0380)
#define		E_SoloRv_VO_CURSOR_POS_X(n)		((n) << 11)
#define		E_SoloRv_VO_CURSOR_POS_Y(n)		((n) << 0)
#define E_SoloRa_VO_CURSOR_CLR					(0x0384)
#define E_SoloRa_VO_CURSOR_CLR2				(0x0388)
#define E_SoloRa_VO_CURSOR_MASK(id)		(0x0390+((id)*4))
#define		E_SoloRv_VO_CURSOR_MASKMAP(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15)	\
	((a0<<30)|(a1<<28)|(a2<<26)|(a3<<24)|(a4<<22)|(a5<<20)|(a6<<18)|(a7<<16)|	\
	(a8<<14)|(a9<<12)|(a10<<10)|(a11<<8)|(a12<<6)|(a13<<4)|(a14<<2)|(a15<<0))

/*
* OSG
*/
#define	E_SoloRa_OSG_CONFIG						0x03E0
#define		E_SoloRv_VO_OSG_ON									(1<<31)
#define		E_SoloRv_VO_OSG_COLOR_MUTE					(1<<28)
#define		E_SoloRv_VO_OSG_ALPHA_RATE(n)			((n)<<22)
#define		E_SoloRv_VO_OSG_ALPHA_BG_RATE(n)		((n)<<16)
#define		E_SoloRv_VO_OSG_BASE(offset)				(((offset)>>16)&0xffff)

#define E_SoloRa_OSG_ERASE					0x03E4
#define		E_SoloRv_OSG_ERASE_ON							(1<<0)

#define E_SoloRa_VO_OSG_BLINK				0x03E8
#define		E_SoloRv_VO_OSG_BLINK_ON						(1<<1)
#define		E_SoloRv_VO_OSG_BLINK_INTREVAL18		(1<<0)			// OSG blink interval 0=15frame, 1=18frame

/*
*	VIDEO CAPTURE REGISTER
*/
#define E_SoloRa_CAP_BASE						0x0400
#define		E_SoloRv_CAP_MAX_PAGE(n)						((n)<<16)
#define		E_SoloRv_CAP_BASE_ADDR(n)						((n)<<0)
#define E_SoloRa_CAP_BTW						0x0404
#define		E_SoloRv_CAP_AUTOWAIT								(1<<17)
#define		E_SoloRv_CAP_PROG_BANDWIDTH(n)			((n)<<8)
#define		E_SoloRv_CAP_MAX_BANDWIDTH(n)				((n)<<0)

#define E_SoloRa_DIM_SCALE1					0x0408
#define E_SoloRa_DIM_SCALE2					0x040C
#define E_SoloRa_DIM_SCALE3					0x0410
#define E_SoloRa_DIM_SCALE4					0x0414
#define E_SoloRa_DIM_SCALE5					0x0418
#define		E_SoloRv_DIM_V_MB_NUM_FRAME(n)			((n)<<16)
#define		E_SoloRv_DIM_V_MB_NUM_FIELD(n)			((n)<<8)
#define		E_SoloRv_DIM_H_MB_NUM(n)						((n)<<0)

#define E_SoloRa_DIM_PROG						0x041C
#define E_SoloRa_CAP_STATUS					0x0420

#define E_SoloRa_CAP_CH_SCALE(ch)				(0x0440+((ch)*4))
#define		E_SoloRv_CAP_SCL_NONE						0
#define		E_SoloRv_CAP_SCL_H1_V1					1
#define		E_SoloRv_CAP_SCL_H1_2V1					2
#define		E_SoloRv_CAP_SCL_H1_2V1_2				3
#define		E_SoloRv_CAP_SCL_H1_3V1_3				4
#define		E_SoloRv_CAP_SCL_H1_4V1_2				5
#define		E_SoloRv_CAP_SCL_FRAMEMODE				(1<<3)

#define E_SoloRa_CAP_CH_COMP_ENA_E(ch)		(0x0480+((ch)*4))
#define E_SoloRa_CAP_CH_INTV(ch)					(0x04C0+((ch)*4))
#define E_SoloRa_CAP_CH_INTV_E(ch)				(0x0500+((ch)*4))

/*
*	MPEG4 ENCODER REGISTER
*/

#define E_SoloRa_VE_CFG0						0x0610
#define		E_SoloRv_VE_TWO_PAGE_MODE				(1<<31)
#define		E_SoloRv_VE_INTR_CTRL(n)					((n)<<24)	// interrpt moe : every 0=1,1=2 ,2=4 ,3=8 ,4=16 th frames
#define		E_SoloRv_VE_BLOCK_SIZE(n)				((n)<<16)	// 64KB block
#define		E_SoloRv_VE_BLOCK_BASE(n)				((n)<<0)	// 64KB block

#define E_SoloRa_VE_CFG1						0x0614
#define		E_SoloRv_VE_MPEG_SIZE_H(n)				((n)<<28)	//(6110only)
#define		E_SoloRv_VE_BYTE_ALIGN(n)					((n)<<24)	// byte align:0=8,1=16.2=32,3=64
#define		E_SoloRv_VE_JPEG_SIZE_H(n)				((n)<<20)	//(6110only)
#define		E_SoloRv_VE_JPEG_INDEX						(1<<19)		//(6110only)
#define		E_SoloRv_VE_INSERT_INDEX					(1<<18)
#define		E_SoloRv_VE_MOTION_MODE(n)				((n)<<16)	// insert motion flag data :  0=off.1=after ndex 256 bytes,2=user assigned
#define		E_SoloRv_VE_MOTION_BASE(n)				((n)<<0)

#define E_SoloRa_VE_WMRK_INIT_KEY		0x061C
#define E_SoloRa_VE_WMRK_POLY				0x0620
#define E_SoloRa_VE_WMRK_STRL				0x0624
#define E_SoloRa_VE_ENCRYP_POLY			0x0628
#define E_SoloRa_VE_ENCRYP_INIT			0x062C
#define E_SoloRa_VE_ATTR							0x0630
#define		E_SoloRv_VE_LITTLE_ENDIAN				(1<<31)
#define		E_SoloRv_COMP_ATTR_RN						(1<<30)
#define		E_SoloRv_COMP_ATTR_FCODE(n)			((n)<<27)
#define		E_SoloRv_COMP_TIME_INC(n)				((n)<<25)
#define		E_SoloRv_COMP_TIME_WIDTH(n)			((n)<<21)
#define		E_SoloRv_DCT_INTERVAL(n)					((n)<<16)
#define E_SoloRa_VE_MTNMAPCFG				0x0634
#define		E_SoloRv_VE_MTNMAPCFG_Base			0x00040000

#define E_SoloRa_VE_STATE(n)					(0x0640+((n)*4))
#define		E_SoloRv_VE_QUEIDXMAKS					0x0F

#define E_SoloRa_VE_JPEG_QP_TBL			0x0670
#define E_SoloRa_VE_JPEG_QP_CH_L			0x0674
#define E_SoloRa_VE_JPEG_QP_CH_H			0x0678
#define E_SoloRa_VE_JPEG_CFG					0x067C
#define E_SoloRa_VE_JPEG_CTRL				0x0680
#define E_SoloRa_VE_JPEG_LIMITE			0x688

#define E_SoloRa_VE_WMRK_ENABLE			0x068C

#define E_SoloRa_VE_OSD_CH						0x0690
#define E_SoloRa_VE_OSD_BASE					0x0694
#define E_SoloRa_VE_OSD_CLR					0x0698
#define E_SoloRa_VE_OSD_OPT					0x069C

#define E_SoloRa_VE_CH_INTL(ch)			(0x0700+((ch)*4))
#define		E_SoloRv_VE_CH_INTL_INTERACE		(1<<0)
#define		E_SoloRv_VE_CH_INTL_INTRABASE	(1<<1)
#define E_SoloRa_VE_CH_MOT(ch)				(0x0740+((ch)*4))
#define E_SoloRa_VE_CH_QP(ch)				(0x0780+((ch)*4))
#define E_SoloRa_VE_CH_QP_E(ch)			(0x07C0+((ch)*4))
#define E_SoloRa_VE_CH_GOP(ch)				(0x0800+((ch)*4))
#define E_SoloRa_VE_CH_GOP_E(ch)			(0x0840+((ch)*4))
#define E_SoloRa_VE_CH_REF_BASE(ch)	(0x0880+((ch)*4))
#define E_SoloRa_VE_CH_REF_BASE_E(ch)		(0x08C0+((ch)*4))

#define E_SoloRa_VE_MPEG4_QUE(n)			(0x0A00+((n)*8))
#define E_SoloRa_VE_JPEG_QUE(n)			(0x0A04+((n)*8))

/*
*	MPEG4 DECODER REGISTER
*/
#define E_SoloRa_VD_CFG0						0x0900
#define		E_SoloRv_VD_CFG_NO_DISP_ERR			(1<<24)
#define		E_SoloRv_VD_CFG_BUSY_WIAT_CODE	(1<<23)
#define		E_SoloRv_VD_CFG_BUSY_WIAT_REF		(1<<22)
#define		E_SoloRv_VD_CFG_BUSY_WIAT_RES		(1<<21)
#define		E_SoloRv_VD_CFG_BUSY_WIAT_MS			(1<<20)
#define		E_SoloRv_VD_CFG_SINGLE_MODE			(1<<18)
#define		E_SoloRv_VD_CFG_SCAL_MANUAL			(1<<17)
#define		E_SoloRv_VD_CFG_USER_PAGE_CTRL		(1<<16)
#define		E_SoloRv_VD_CFG_LITTLE_ENDIAN		(1<<15)
#define		E_SoloRv_VD_CFG_START_FI					(1<<14)
#define		E_SoloRv_VD_CFG_ERR_LOCK					(1<<13)
#define		E_SoloRv_VD_CFG_ERR_INT_ENA			(1<<12)
#define		E_SoloRv_VD_CFG_TIME_WIDTH(n)		((n)<<8)
#define		E_SoloRv_VD_CFG_DCT_INTERVAL(n)	((n)<<0)

#define E_SoloRa_VD_CFG1						0x0904
#define		E_SoloRv_VD_CFG_MAX_MBH(n)			((n)<<26)	//(6110only)
#define		E_SoloRv_VD_CFG_MAX_MBV(n)			((n)<<20)	//(6110only)
#define		E_SoloRv_VD_CFG_TIMEOUT(n)			((n)<<16)	//(6110only)
#define		E_SoloRv_VD_CFG_DEC2LIVE_ADDR(n)		((n)>>16)

#define	E_SoloRa_VD_DEINTERLACE		0x0908
#define		E_SoloRv_VD_DEINTERLACE_THRESHOLD(n)		((n)<<8)
#define		E_SoloRv_VD_DEINTERLACE_EDGE_VALUE(n)	((n)<<0)

#define E_SoloRa_VD_CODE_ADR					0x090C

#define E_SoloRa_VD_CTRL							0x0910
#define		E_SoloRv_VD_OPER_ON							(1<<31)
#define		E_SoloRv_VD_MAX_ITEM(n)					((n)<<0)

#define E_SoloRa_VD_STATUS0					0x0920
#define		E_SoloRv_VD_STATUS0_MBSIZE_ERR	(1<<24)	//(6110only)
#define		E_SoloRv_VD_STATUS0_TIMEOUT			(1<<23)	//(6110only)
#define		E_SoloRv_VD_STATUS0_ERR			(1<<20)
#define E_SoloRa_VD_STATUS1					0x0924
#define E_SoloRa_VD_STATUS2					0x0928

#define E_SoloRa_VD_IDX0							0x0930
#define		E_SoloRv_VD_IDX_INTERLACE				(1<<30)
#define		E_SoloRv_VD_IDX_CHANNEL(n)				((n)<<24)
#define		E_SoloRv_VD_IDX_VOP(n)						((n)<<22)
#define		E_SoloRv_VD_IDX_NODISP					(1<<21)
#define		E_SoloRv_VD_IDX_SIZE(n)					((n)<<0)

#define E_SoloRa_VD_IDX1						0x0934
#define		E_SoloRv_VD_IDX_SRC_SCALE(n)			((n)<<28)
#define		E_SoloRv_VD_IDX_WINDOW(n)				((n)<<24)
#define		E_SoloRv_VD_IDX_DEINTERLACE			(1<<16)
#define		E_SoloRv_VD_IDX_H_BLOCK(n)				((n)<<8)
#define		E_SoloRv_VD_IDX_V_BLOCK(n)				((n)<<0)

#define E_SoloRa_VD_IDX2						0x0938
#define		E_SoloRv_VD_IDX_REF_BASE_SIDE		(1<<31)
#define		E_SoloRv_VD_IDX_REF_BASE(n)			(((n)>>16)&0xffff)

#define E_SoloRa_VD_IDX3						0x093C
#define		E_SoloRv_VD_IDX_DISP_SCALE(n)			((n)<<28)
#define		E_SoloRv_VD_IDX_INTERLACE_WR				(1<<27)
#define		E_SoloRv_VD_IDX_INTERPOL						(1<<26)
#define		E_SoloRv_VD_IDX_HOR2X							(1<<25)
#define		E_SoloRv_VD_IDX_OFFSET_X(n)				((n)<<12)
#define		E_SoloRv_VD_IDX_OFFSET_Y(n)				((n)<<0)

#define E_SoloRa_VD_IDX4						0x0940		// code base
#define		E_SoloRv_VD_IDX_DEC_WR_PAGE(n)			((n)<<8)
#define		E_SoloRv_VD_IDX_DISP_RD_PAGE(n)		((n)<<0)

#define E_SoloRa_VD_IDX5						0x0944		// next address

/*
*	GPIO REGISTER
*/
#define E_SoloRa_GPIO_CONFIG_0				0x0B00
#define		E_SoloRv_GPIO_CONFIG_0(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15)	\
	((a15<<30)|(a14<<28)|(a13<<26)|(a12<<24)|(a11<<22)|(a10<<20)|(a9<<18)|(a8<<16)|	\
	(a7<<14)|(a6<<12)|(a5<<10)|(a4<<8)|(a3<<6)|(a2<<4)|(a1<<2)|(a0<<0))
#define E_SoloRa_GPIO_CONFIG_1				0x0B04
#define E_SoloRa_GPIO_DATA_OUT				0x0B08
#define E_SoloRa_GPIO_DATA_IN				0x0B0C
#define E_SoloRa_GPIO_INT_ACK_STA		0x0B10
#define E_SoloRa_GPIO_INT_ENA				0x0B14
#define E_SoloRa_GPIO_INT_CFG_0			0x0B18
#define E_SoloRa_GPIO_INT_CFG_1			0x0B1C
#define		E_SoloRv_GPIO_INT_RISING			0
#define		E_SoloRv_GPIO_INT_FALLING		1
#define		E_SoloRv_GPIO_INT_BOTHEDGE		2
#define		E_SoloRv_GPIO_INT_LOWLEVEL		3

/*
*	IIC REGISTER
*/
#define E_SoloRa_IIC_CFG						0x0B20
#define		E_SoloRv_IIC_ENABLE							(1<<8)
#define		E_SoloRv_IIC_PRESCALE(n)					((n)<<0)	// (SysClk / 5) / ({4'b0,N[7:0],4'b0}+1) = iic clock
// 317.647 KHz (1), 163.636 KHz (2), 110.204 KHz (3), 83.076 KHz (4),  66.666 KHz (5), 1.3 KHz   (FF)

#define E_SoloRa_IIC_CTRL					0x0B24
#define		E_SoloRv_IIC_AUTO_CLEAR					(1<<20)
#define		E_SoloRv_IIC_STAT_RX_ACK					(1<<19)
#define		E_SoloRv_IIC_STAT_BUSY						(1<<18)
#define		E_SoloRv_IIC_STAT_SIG_ERR				(1<<17)
#define		E_SoloRv_IIC_STAT_TRNS						(1<<16)
#define		E_SoloRv_IIC_CH(n)								((n)<<5)
#define		E_SoloRv_IIC_ACK_EN							(1<<4)
#define		E_SoloRv_IIC_START								(1<<3)
#define		E_SoloRv_IIC_STOP								(1<<2)
#define		E_SoloRv_IIC_READ								(1<<1)
#define		E_SoloRv_IIC_WRITE								(1<<0)

#define E_SoloRa_IIC_TXD						0x0B28
#define E_SoloRa_IIC_RXD						0x0B2C

//uart
#define	E_SoloRa_UART_CTRL(n) 			(0x00000BA0 + ((n) * 0x20))	/*	R/W RS485 Control. 0x00f0_0000	*/
#define		E_SoloRv_UART_CTRL_PARITY(n)		((n)<<0)		/*	parity bit */
#define		E_SoloRv_UART_CTRL_PARITY_NONE		0						/*	parity bit */
#define		E_SoloRv_UART_CTRL_PARITY_EVEN		2						/*	parity bit */
#define		E_SoloRv_UART_CTRL_PARITY_ODD		3						/*	parity bit */
#define		E_SoloRv_UART_CTRL_STOP(n)			((n)<<2)		/*	stop bit */
#define		E_SoloRv_UART_CTRL_STOP_1BIT		0						/*	stop bit */
#define		E_SoloRv_UART_CTRL_STOP_2BIT		1						/*	stop bit */
#define		E_SoloRv_UART_CTRL_STOP_1_5BIT	2						/*	stop bit */
#define		E_SoloRv_UART_CTRL_DATA(n)			((n)<<4)		/*	data bit */
#define		E_SoloRv_UART_CTRL_BAUD0(n)		((n)<<6)		/*	baud 0 */
#define		E_SoloRv_UART_CTRL_BAUD1(n)		((n)<<9)		/*	baud 1 */
#define		E_SoloRv_UART_CTRL_LOOP				(1<<11)			/*	loopback */
#define		E_SoloRv_UART_CTRL_HALF				(1<<12)			/*	half duplex */
#define		E_SoloRv_UART_CTRL_TX_EN				(1<<13)			/*	tx enable */
#define		E_SoloRv_UART_CTRL_RX_EN				(1<<14)			/*	rx enable */
#define		E_SoloRv_UART_CTRL_TX_IRQ			(1<<15)			/*	tx interrupt enable */
#define		E_SoloRv_UART_CTRL_RX_IRQ			(1<<16)			/*	rx interrupt enable */
#define		E_SoloRv_UART_CTRL_ERR_IRQ			(1<<17)			/*	error interrupt enable */
#define		E_SoloRv_UART_CTRL_PAR_IRQ			(1<<18)			/*	parity error interrupt enable */
#define		E_SoloRv_UART_CTRL_RTS_CTS			(1<<20)			/*	cts rts enable */
#define		E_SoloRv_UART_CTRL_DIV(n)			((n)<<24)			/*	divide number */

#define	E_SoloRa_UART_STA(n) 			(0x00000BA4+((n)*0x20)) 		/*	R RS485 Status. -	*/
#define	E_SoloRa_UART_TXD(n) 			(0x00000BA8+((n)*0x20)) 		/*	R/W RS485 Transmit Data. 0x0000_0000	*/
#define		E_SoloRv_UART_TXD_PUSH			(1 << 8)
#define	E_SoloRa_UART_RXD(n) 			(0x00000BAC+((n)*0x20)) 		/*	R RS485 Receive Data. -	*/
#define		E_SoloRv_UART_RXD_POP			(1 << 8)

/*	timer			*/
#define E_SoloRa_TIMER_CLK_NUM				0x0BE0
#define E_SoloRa_TIMER_WATCHDOG				0x0BE4
#define		E_SoloRv_TIMER_WATCHDOG_ENABLE		(1<<8)
#define		E_SoloRv_TIMER_WATCHDOG_SEC(n)		((n)<<0)
#define E_SoloRa_TIMER_USEC						0x0BE8
#define E_SoloRa_TIMER_SEC						0x0BEC

/*
*	G723 CODEC REGISTER
*/
#define E_SoloRa_AUDIO_CONTROL				0x0D00
#define		E_SoloRv_AUDIO_ENABLE						(1<<31)
#define		E_SoloRv_AUDIO_MASTER_MODE				(1<<30)
#define		E_SoloRv_AUDIO_I2S_MODE					(1<<29)		// 0 : u-Law, 1 : I2S
#define		E_SoloRv_AUDIO_I2S_LR_SWAP				(1<<27)
#define		E_SoloRv_AUDIO_I2S_8BIT					(1<<26)
#define		E_SoloRv_AUDIO_I2S_MULTI(n)			((n)<<24)	// 0: 2ch, 1: 4ch, 2: 8ch, 3: 16ch (I2S slave only)
#define		E_SoloRv_AUDIO_MIX_9TO0					(1<<23)
#define		E_SoloRv_AUDIO_DEC_9TO0_VOL(n)		((n)<<20)
#define		E_SoloRv_AUDIO_MIX_19TO10				(1<<19)
#define		E_SoloRv_AUDIO_DEC_19TO10_VOL(n)	((n)<<16)
#define		E_SoloRv_AUDIO_MODE(n)						((n)<<0)	// Ch9~0(PCM) / Ch19,18~1,0(I2S), 1:Dec, 0:Enc
#define E_SoloRa_AUDIO_SAMPLE				0x0D04
#define		E_SoloRv_AUDIO_EE_MODE_ON				(1<<30)		// for testing
#define		E_SoloRv_AUDIO_EE_ENC_CH(ch)			((ch)<<25)	// for testing
#define		E_SoloRv_AUDIO_BITRATE(n)				((n)<<16)	// Sampling rate = Clock Freq / (n*a) Hz (master only) (i2s:a=2, pcm:a=1)
#define		E_SoloRv_AUDIO_CLK_DIV(n)				((n)<<0)	// Clock Freq = SYS_CLK / (n*2) Hz (master only)
#define E_SoloRa_AUDIO_FDMA_INTR				0x0D08
#define		E_SoloRv_AUDIO_FDMA_INTERVAL(n)		((n)<<19)
#define		E_SoloRv_AUDIO_INTR_ORDER(n)				((n)<<16)	// 0~4 available, 2^n pages
#define		E_SoloRv_AUDIO_FDMA_BASE(n)				((n)<<0)
#define E_SoloRa_AUDIO_EVOL_0				0x0D0C
#define E_SoloRa_AUDIO_EVOL_1				0x0D10
#define		E_SoloRv_AUDIO_EVOL(ch, value)			((value)<<((ch)%10))
#define E_SoloRa_AUDIO_STA					0x0D14

/* time adjust */
#define	E_SoloRa_TIME_USECLIM				0x0D20		/* usec limite lsb */


//====================================================================
//global macro


//====================================================================
//global type


//====================================================================
//class


//====================================================================
//global function
	
//====================================================================
//out Enviroment conditional
#endif
