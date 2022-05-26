/*
	mdin 200

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_RegMdin200
#define	_EPC_RegMdin200

//====================================================================
// uses
	
//====================================================================
//constance


/* HOST IF CONTROL REGISTERS */
#define	E_Mdin200Ra_CPU_WAIT_TIME 		0x000
#define	E_Mdin200Ra_CPU_BASE_ADDR 		0x002
#define	E_Mdin200Ra_IRQ_ENABLE 			0x00E
#define	E_Mdin200Ra_INT_STATUS 			0x010
#define	E_Mdin200Ra_ENDIAN_SWAP			0x012
#define	E_Mdin200Ra_INT_DETECT_MODE		0x016
#define E_Mdin200Ra_VCLK_DLY_CTRL		0x018 // MDIN200 
#define E_Mdin200Ra_DIGITAL_2X_EN		0x01C // MDIN200
#define E_Mdin200Ra_DAC_PD_DOUT_DIS		0x01E // MDIN200

/* CLOCK CONTROL REGISTERS */
#define E_Mdin200Ra_PLL_DIS 			0x020
#define E_Mdin200Ra_SDRAM_CLK_DLY_SEL	0x022

#define E_Mdin200Ra_VCLK_OUT_DLY_CTRL	0x024 //  MDIN200
#define E_Mdin200Ra_VCLK_2X_DLY_CTRL	0x026 //  MDIN200
#define E_Mdin200Ra_DAC_DLY_CTRL		0x028 //  MDIN200

#define E_Mdin200Ra_PLL_SEL_VCLK		0x02A
#define E_Mdin200Ra_PRE_DIV_VCLK		0x02C	
#define E_Mdin200Ra_POST_DIV_VCLK		0x02E
#define E_Mdin200Ra_POST_SCALE_VCLK		0x030
#define E_Mdin200Ra_PRE_DIV_MCLK		0x032
#define E_Mdin200Ra_POST_DIV_MCLK		0x034

#define E_Mdin200Ra_VCLK_PLL_CPO_EN		0x036	//  MDIN200
#define E_Mdin200Ra_PLL_SEL_VCLK_HACT	0x03A	// MDIN200
#define E_Mdin200Ra_VCLK_VCO_SEL		0x03C	// MDIN200

//#define E_Mdin200Ra_POST_SCALE_MCLK		0x036
#define E_Mdin200Ra_VCLK_SEL		0x03A
#define E_Mdin200Ra_MCLK_SEL		0x03C
#define E_Mdin200Ra_CLK_A_DIV		0x03E
#define E_Mdin200Ra_CLK_B_DIV		0x040
#define E_Mdin200Ra_DAC_CLK_OUT_SEL		0x042
#define E_Mdin200Ra_CLK_A_DLY_SEL		0x044
#define E_Mdin200Ra_CLK_B_DLY_SEL		0x046

/* GRAPHIC ACCELERATION CONTROL REGISTERS */
#define E_Mdin200Ra_GA_PROCESS_EN_DATA	0x04A
#define E_Mdin200Ra_GA_SPRITE_START_RB	0x04C
#define E_Mdin200Ra_GA_SPRITE_X_SIZE	0x04E
#define E_Mdin200Ra_GA_WINDOW_X_POSITION	0x050
#define E_Mdin200Ra_GA_WINDOW_Y_POSITION	0x052
#define E_Mdin200Ra_GA_WINDOW_X_SIZE	0x054
#define E_Mdin200Ra_GA_WINDOW_Y_SIZE	0x056

/* INPUT FORMAT CONTROL REGISTERS */
#define E_Mdin200Ra_IN_FORMAT_CTRL		0x400
#define E_Mdin200Ra_IN_FID_CTRL		0x401
#define E_Mdin200Ra_IN_SYNC_CTRL		0x402
#define E_Mdin200Ra_IN_ACT_OFFSET_A		0x404
#define E_Mdin200Ra_IN_ACT_OFFSET_B		0x405
#define E_Mdin200Ra_IN_SIZE_H_A		0x406
#define E_Mdin200Ra_IN_SIZE_V_A		0x407
#define E_Mdin200Ra_IN_SIZE_H_B		0x408
#define E_Mdin200Ra_IN_SIZE_V_B		0x409

/* INPUT CSC CONTROL REGISTERS */
#define E_Mdin200Ra_CSC_F_COEF0		0x40A
#define E_Mdin200Ra_CSC_F_COEF1		0x40B
#define E_Mdin200Ra_CSC_F_COEF2		0x40C
#define E_Mdin200Ra_CSC_F_COEF3		0x40D
#define E_Mdin200Ra_CSC_F_COEF4		0x40E
#define E_Mdin200Ra_CSC_F_COEF5		0x40F
#define E_Mdin200Ra_CSC_F_COEF6		0x410
#define E_Mdin200Ra_CSC_F_COEF7		0x411
#define E_Mdin200Ra_CSC_F_COEF8		0x412
#define E_Mdin200Ra_CSC_F_OFFSET_G		0x413
#define E_Mdin200Ra_CSC_F_OFFSET_B		0x414
#define E_Mdin200Ra_CSC_F_OFFSET_R		0x415
#define E_Mdin200Ra_CSC_F_OFFSET_Y		0x416
#define E_Mdin200Ra_CSC_F_OFFSET_CB		0x417
#define E_Mdin200Ra_CSC_F_OFFSET_CR		0x418
#define E_Mdin200Ra_CSC_F_CTRL		0x419

/* CLAMP REGISTERS */
#define E_Mdin200Ra_CLAMP_START		0x41A
#define E_Mdin200Ra_CLAMP_END		0x41B

/* INPUT SYNC FORCING REGISTERS */
#define E_Mdin200Ra_VSYNC_FORCED_RISING_A	0x420
#define E_Mdin200Ra_VSYNC_FORCED_RISING_B	0x421

/* FRONT NOISE REDUCTION FILTER REGISTERS */
#define E_Mdin200Ra_NR_FLT0			0x422
#define E_Mdin200Ra_NR_FLT1			0x423
#define E_Mdin200Ra_NR_FLT2			0x424
#define E_Mdin200Ra_NR_FLT3			0x425
#define E_Mdin200Ra_NR_FLT4			0x426
#define E_Mdin200Ra_NR_FLT5			0x427
#define E_Mdin200Ra_NR_FLT6			0x428
#define E_Mdin200Ra_NR_FLT7			0x429
#define E_Mdin200Ra_NR_FLT_ON		0x42A
#define E_Mdin200Ra_NR_FLT_DIFF		0x42B
#define E_Mdin200Ra_MEDIAN_FLT_ON		0x42C
#define E_Mdin200Ra_MEDIAN_FLT_DIFF		0x42D
#define E_Mdin200Ra_NR_FLT_DIFF_SEL		0x42E

/* SOURCE SIZE/POSITION REGISTERS */
#define E_Mdin200Ra_SRC_SIZE_H		0x432
#define E_Mdin200Ra_SRC_SIZE_V		0x433
#define E_Mdin200Ra_SRC_POSI_H		0x434
#define E_Mdin200Ra_SRC_POSI_V		0x435

/* DESTINATION SIZE/POSITION REGISTERS */
#define E_Mdin200Ra_DEST_SIZE_H		0x436
#define E_Mdin200Ra_DEST_SIZE_V		0x437
#define E_Mdin200Ra_DEST_POSI_H		0x438
#define E_Mdin200Ra_DEST_POSI_V		0x439

/* ANCILLARY SIZE REGISTERS */
#define E_Mdin200Ra_SRC_SIZE_H2		0x43A
#define E_Mdin200Ra_SRC_SIZE_V2		0x43B
#define E_Mdin200Ra_DEST_SIZE_H2		0x43C
#define E_Mdin200Ra_DEST_SIZE_V2		0x43D

/* SIZE LOAD MODE REGISTER */
#define E_Mdin200Ra_SIZE_LOAD_MODE		0x444

/* MAIN FORMAT CONVERTER CONTROL REGISTERS */
#define E_Mdin200Ra_MFC_CONTROL			0x445	// Added MDIN200
#define E_Mdin200Ra_DRC_ITP_C_FLT0		0x440	// Added MDIN200
#define E_Mdin200Ra_DRC_ITP_C_FLT1		0x441	// Added MDIN200
#define E_Mdin200Ra_DRC_ITP_C_FLT2		0x442	// Added MDIN200
#define E_Mdin200Ra_DRC_ITP_C_FLT3		0x443	// Added MDIN200
#define E_Mdin200Ra_DRC_ITP_Y_FLT0		0x446	// Added MDIN200
#define E_Mdin200Ra_DRC_ITP_Y_FLT1		0x447	// Added MDIN200
#define E_Mdin200Ra_DRC_ITP_Y_FLT2		0x448	// Added MDIN200
#define E_Mdin200Ra_DRC_ITP_Y_FLT3		0x449	// Added MDIN200
#define E_Mdin200Ra_DRC_ITP_Y_FLT4		0x44A	// Added MDIN200
#define E_Mdin200Ra_DRC_ITP_Y_FLT5		0x44B	// Added MDIN200
#define E_Mdin200Ra_DRC_ITP_Y_FLT6		0x44C	// Added MDIN200
#define E_Mdin200Ra_DRC_ITP_Y_FLT7		0x44D	// Added MDIN200

/* MAIN FORMAT CONVERTER COEFFICIENT REGISTERS */ 
#define E_Mdin200Ra_MFC_ITP_COEF_DATA0		0x590	// Added MDIN200
#define E_Mdin200Ra_MFC_ITP_COEF_DATA1		0x591	// Added MDIN200
#define E_Mdin200Ra_MFC_ITP_COEF_DATA2		0x592	// Added MDIN200
#define E_Mdin200Ra_MFC_ITP_COEF_DATA3		0x593	// Added MDIN200
#define E_Mdin200Ra_MFC_ITP_COEF_CMD		0x594	// Added MDIN200

/* MEMORY CONTROL REGISTERS */
#define E_Mdin200Ra_ARBITER_CTRL		0x450
#define E_Mdin200Ra_ARBITER_PRI		0x451
#define E_Mdin200Ra_ARBITER_STARV		0x452
#define E_Mdin200Ra_FCMC_PRI		0x453
#define E_Mdin200Ra_FCMC_STARV		0x454
#define E_Mdin200Ra_MEM_CONFIG		0x455
#define E_Mdin200Ra_MEM_MAP			0x456

/* HORIZIONTAL PEAKING FILTER, LTI & CTI REGISTERS */
#define E_Mdin200Ra_H_PEAKING_FLT0		0x458
#define E_Mdin200Ra_H_PEAKING_FLT1		0x459
#define E_Mdin200Ra_H_PEAKING_FLT2		0x45A
#define E_Mdin200Ra_H_PEAKING_FLT3		0x45B
#define E_Mdin200Ra_H_PEAKING_FLT4		0x45C
#define E_Mdin200Ra_H_PEAKING_FLT5		0x45D
#define E_Mdin200Ra_H_PEAKING_FLT6		0x45E
#define E_Mdin200Ra_H_PEAKING_FLT7		0x45F
#define E_Mdin200Ra_H_PEAKING_FLT_ENABLE	0x460
#define E_Mdin200Ra_H_PEAKING_FLT_CTRL	0x461
#define E_Mdin200Ra_LTI_CTRL1		0x462
#define E_Mdin200Ra_LTI_CTRL2		0x463
#define E_Mdin200Ra_CTI_B_CTRL1		0x464
#define E_Mdin200Ra_CTI_B_CTRL2		0x465
#define E_Mdin200Ra_CTI_R_CTRL1		0x466
#define E_Mdin200Ra_CTI_R_CTRL2		0x467

/* COLOR ENHANCEMENT FILTER REGISTERS */
#define E_Mdin200Ra_COLOR_ENH_FLT0			0x468
#define E_Mdin200Ra_COLOR_ENH_FLT1			0x469
#define E_Mdin200Ra_COLOR_ENH_FLT2			0x46A
#define E_Mdin200Ra_COLOR_ENH_FLT3			0x46B
#define E_Mdin200Ra_COLOR_ENH_FLT_ENABLE	0x46C

/* COLOR ENHANCEMENT FILTER REGISTERS */
#define E_Mdin200Ra_SURFACE_FLT0		0x46D
#define E_Mdin200Ra_SURFACE_FLT1		0x46E
#define E_Mdin200Ra_SURFACE_FLT2		0x46F
#define E_Mdin200Ra_SURFACE_FLT3		0x470
#define E_Mdin200Ra_SURFACE_FLT_ENABLE	0x471

/* OUTPUT CSC CONTROL REGISTERS */
#define E_Mdin200Ra_CSC_COEF0			0x472
#define E_Mdin200Ra_CSC_COEF1			0x473
#define E_Mdin200Ra_CSC_COEF2			0x474
#define E_Mdin200Ra_CSC_COEF3			0x475
#define E_Mdin200Ra_CSC_COEF4			0x476
#define E_Mdin200Ra_CSC_COEF5			0x477
#define E_Mdin200Ra_CSC_COEF6			0x478
#define E_Mdin200Ra_CSC_COEF7			0x479
#define E_Mdin200Ra_CSC_COEF8			0x47A
#define E_Mdin200Ra_CSC_OFFSET_Y		0x47B
#define E_Mdin200Ra_CSC_OFFSET_CB		0x47C
#define E_Mdin200Ra_CSC_OFFSET_CR		0x47D
#define E_Mdin200Ra_CSC_OFFSET_G		0x47E
#define E_Mdin200Ra_CSC_OFFSET_B		0x47F
#define E_Mdin200Ra_CSC_OFFSET_R		0x480
#define E_Mdin200Ra_CSC_CTRL			0x481

/* POST DEMONSTRATION CONTROL REGISTER */
#define E_Mdin200Ra_POST_DEMO_CTRL		0x482

/* VDPULSE CONTROL REGISTERS */
#define E_Mdin200Ra_HDPULSE_H		0x484	// Added MDIN200
#define E_Mdin200Ra_VDPULSE_V		0x485
#define E_Mdin200Ra_VDPULSE_POS_BOT	0x486	// Added MDIN200

/* OUTPUT SYNC CONTROL REGISTERS */
#define E_Mdin200Ra_HTOTAL_SIZE		0x488
#define E_Mdin200Ra_HACTIVE_START	0x489
#define E_Mdin200Ra_HACTIVE_END		0x48A
#define E_Mdin200Ra_HSYNC_START		0x48B
#define E_Mdin200Ra_HSYNC_END		0x48C
#define E_Mdin200Ra_VTOTAL_SIZE		0x48D
#define E_Mdin200Ra_VACTIVE_START	0x48E
#define E_Mdin200Ra_VACTIVE_END		0x48F
#define E_Mdin200Ra_VSYNC_START		0x490
#define E_Mdin200Ra_VSYNC_END		0x491

/* OUTPUT SYNC CONTROL INTERLACED OUTPUT REGISTERS (NEW) */
#define E_Mdin200Ra_VACTIVE_START_BOT	0x492	// Added MDIN200
#define E_Mdin200Ra_VACTIVE_END_BOT		0x493	// Added MDIN200
#define E_Mdin200Ra_VSYNC_START_BOT		0x494	// Added MDIN200
#define E_Mdin200Ra_VSYNC_END_BOT		0x495	// Added MDIN200
#define E_Mdin200Ra_VSYNC_BOT_FLD_POS	0x496	// Added MDIN200
#define E_Mdin200Ra_INTERLACED_OUT_EN	0x497	// Added MDIN200

#define E_Mdin200Ra_SYNC_CTRL			0x498
#define E_Mdin200Ra_OUTPUT_SYNC_RESET	0x499

/* TIMER INTERRUPT CONTROL REGISTERS */
#define E_Mdin200Ra_TMR_INTR_CTRL		0x4A0

/* OUT CONTROL REGISTERS */
#define E_Mdin200Ra_BG_COLOR_Y		 	0x49A	// Added MDIN200
#define E_Mdin200Ra_BG_COLOR_CBCR		0x49B	// Added MDIN200
#define E_Mdin200Ra_OUT_CTRL			0x4A2
#define E_Mdin200Ra_DAC_CTRL			0x4A3	// Added MDIN200
#define E_Mdin200Ra_BLACK_LEVEL_CTRL	0x4A4

/* BLACK/WHITE LEVEL EXTENSION REGISTERS */
#define E_Mdin200Ra_BW_EXT_X1_POS		0x4A6
#define E_Mdin200Ra_BW_EXT_X2_POS		0x4A7
#define E_Mdin200Ra_BW_EXT_X3_POS		0x4A8
#define E_Mdin200Ra_BW_EXT_X4_POS		0x4A9
#define E_Mdin200Ra_BW_EXT_P1_OFFSET	0x4AA
#define E_Mdin200Ra_BW_EXT_P2_OFFSET	0x4AB
#define E_Mdin200Ra_BW_EXT_P3_OFFSET	0x4AC
#define E_Mdin200Ra_BW_EXT_P4_OFFSET	0x4AD

#define E_Mdin200Ra_BW_EXT_CTRL		0x4AF

/* DISPLAY MODE CONTROL REGISTER */
#define E_Mdin200Ra_DISPLAY_MODE		0x4B0

/* OUTPUT SYNC MODE REGISTER */
#define E_Mdin200Ra_OUT_SYNC_CTRL		0x4B4

/* BORDER CONTROL REGISTER */
#define E_Mdin200Ra_MAIN_BORDER		0x4B6

/* INPUT TEST PATTERN REGISTER */
#define E_Mdin200Ra_IN_TEST_PTRN		0x4C0

/* OUTPUT TEST PATTERN REGISTER */
#define E_Mdin200Ra_OUT_TEST_PTRN		0x4C2

/* LOCAL REGISTER UPDATE REGISTER */
#define E_Mdin200Ra_LOCAL_REG_UPDATE	0x4C4

/* LOCAL REGISTER ACCESS CONTROL REGISTER */
#define E_Mdin200Ra_REG_OEN_SEL		0x4C6

/* VACTIVE STATUS REGISTER */
#define E_Mdin200Ra_VACTIVE_OUT		0x4C8

/* INPUT AUTO DETECT CONTROL REGISTER */
#define E_Mdin200Ra_IN_DETECT_MODE		0x4D0
#define E_Mdin200Ra_IN_SYNC_SEL		0x4D1
#define E_Mdin200Ra_IN_PIXEL_THRES		0x4D2
#define E_Mdin200Ra_LD_HSTART_POS		0x4D3
#define E_Mdin200Ra_LD_HEND_POS		0x4D4
#define E_Mdin200Ra_LD_VSTART_POS		0x4D5
#define E_Mdin200Ra_LD_VEND_POS		0x4D6
#define E_Mdin200Ra_LD_UPPER_THRES		0x4D7
#define E_Mdin200Ra_LD_LOWER_THRES		0x4D8

/* INPUT AUTO DETECT STATUS REGISTER */
#define E_Mdin200Ra_IN_SYNC_LOST		0x4E0
#define E_Mdin200Ra_IN_HSYNC		0x4E2
#define E_Mdin200Ra_IN_SCAN_I		0x4E4
#define E_Mdin200Ra_IN_HSYNC_HI		0x4E6
#define E_Mdin200Ra_IN_HSYNC_LO		0x4E7
#define E_Mdin200Ra_IN_VSYNC_HI		0x4E8
#define E_Mdin200Ra_IN_VSYNC_LO		0x4E9
#define E_Mdin200Ra_IN_HACT_START		0x4EE
#define E_Mdin200Ra_IN_HACT_END		0x4EF
#define E_Mdin200Ra_IN_VACT_START		0x4F0
#define E_Mdin200Ra_IN_VACT_END		0x4F1
#define E_Mdin200Ra_LD_UPPER_EX_PIXEL	0x4F2
#define E_Mdin200Ra_LD_LOWER_EX_PIXEL	0x4F3

/* OSD CONTROL REGISTERS */
#define E_Mdin200Ra_OSD_CTRL		0x540
#define E_Mdin200Ra_OSD_BACKGND_G		0x541
#define E_Mdin200Ra_OSD_BACKGND_B_R		0x542
#define E_Mdin200Ra_OSD_S0_CTRL		0x543
#define E_Mdin200Ra_OSD_S0_DISP_POSI_X	0x544
#define E_Mdin200Ra_OSD_S0_DISP_POSI_Y	0x545
#define E_Mdin200Ra_OSD_S0_IMG_SIZE_X	0x546
#define E_Mdin200Ra_OSD_S0_IMG_SIZE_Y	0x547
#define E_Mdin200Ra_OSD_S0_BMP_ROW		0x548
#define E_Mdin200Ra_OSD_S0_BMP_BANK_COL	0x549
#define E_Mdin200Ra_OSD_S1_CTRL		0x54A
#define E_Mdin200Ra_OSD_S1_DISP_POSI_X	0x54B
#define E_Mdin200Ra_OSD_S1_DISP_POSI_Y	0x54C
#define E_Mdin200Ra_OSD_S1_IMG_SIZE_X	0x54D
#define E_Mdin200Ra_OSD_S1_IMG_SIZE_Y	0x54E
#define E_Mdin200Ra_OSD_S1_BMP_ROW		0x54F
#define E_Mdin200Ra_OSD_S1_BMP_BANK_COL	0x550
#define E_Mdin200Ra_OSD_S2_CTRL		0x551
#define E_Mdin200Ra_OSD_S2_DISP_POSI_X	0x552
#define E_Mdin200Ra_OSD_S2_DISP_POSI_Y	0x553
#define E_Mdin200Ra_OSD_S2_IMG_SIZE_X	0x554
#define E_Mdin200Ra_OSD_S2_IMG_SIZE_Y	0x555
#define E_Mdin200Ra_OSD_S2_BMP_ROW		0x556
#define E_Mdin200Ra_OSD_S2_BMP_BANK_COL	0x557
#define E_Mdin200Ra_OSD_S3_CTRL		0x558
#define E_Mdin200Ra_OSD_S3_DISP_POSI_X	0x559
#define E_Mdin200Ra_OSD_S3_DISP_POSI_Y	0x55A
#define E_Mdin200Ra_OSD_S3_IMG_SIZE_X	0x55B
#define E_Mdin200Ra_OSD_S3_IMG_SIZE_Y	0x55C
#define E_Mdin200Ra_OSD_S3_BMP_ROW		0x55D
#define E_Mdin200Ra_OSD_S3_BMP_BANK_COL	0x55E

/* OSD PALETTE REGISTERS */
#define E_Mdin200Ra_OSD_PAL_DATA_MSB	0x580
#define E_Mdin200Ra_OSD_PAL_DATA_LSB	0x581
#define E_Mdin200Ra_OSD_PAL_CMD		0x582

// Output DAC control
#define E_Mdin200Ra_DAC_CTRL_DATA		0x588
#define E_Mdin200Ra_DAC_CTRL_CMD		0x589

#define E_Mdin200Ra_SOFT_RESET		0x070
#define E_Mdin200Ra_HARD_RESET		0x072

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

