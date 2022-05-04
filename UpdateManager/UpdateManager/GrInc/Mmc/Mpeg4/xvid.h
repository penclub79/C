/*****************************************************************************
 *
 * XVID MPEG-4 VIDEO CODEC
 * - XviD Main header file -
 *
 *  Copyright(C) 2001-2004 Peter Ross <pross@xvid.org>
 *
 *  This program is free software ; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation ; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY ; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program ; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * $Id: xvid.h,v 1.51.2.2 2005/11/22 21:01:33 edgomez Exp $
 *
 ****************************************************************************/

#ifndef _XVID_H_
#define _XVID_H_


#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * versioning
 ****************************************************************************/

/* versioning
	version takes the form "$major.$minor.$patch"
	$patch is incremented when there is no api change
	$minor is incremented when the api is changed, but remains backwards compatible
	$major is incremented when the api is changed significantly

	when initialising an xvid structure, you must always zero it, and set the version field.
		memset(&struct,0,sizeof(struct));
		struct.version = XVID_VERSION;

	XVID_UNSTABLE is defined only during development.
	*/

#define XVID_MAKE_VERSION(a,b,c) ((((a)&0xff)<<16) | (((b)&0xff)<<8) | ((c)&0xff))
#define XVID_VERSION_MAJOR(a)    ((__s8)(((a)>>16) & 0xff))
#define XVID_VERSION_MINOR(a)    ((__s8)(((a)>> 8) & 0xff))
#define XVID_VERSION_PATCH(a)    ((__s8)(((a)>> 0) & 0xff))

#define XVID_MAKE_API(a,b)       ((((a)&0xff)<<16) | (((b)&0xff)<<0))
#define XVID_API_MAJOR(a)        (((a)>>16) & 0xff)
#define XVID_API_MINOR(a)        (((a)>> 0) & 0xff)

#define XVID_VERSION             XVID_MAKE_VERSION(1,1,0)
#define XVID_API                 XVID_MAKE_API(4, 1)

/* Bitstream Version
 * this will be writen into the bitstream to allow easy detection of xvid
 * encoder bugs in the decoder, without this it might not possible to
 * automatically distinquish between a file which has been encoded with an
 * old & buggy XVID from a file which has been encoded with a bugfree version
 * see the infamous interlacing bug ...
 *
 * this MUST be increased if an encoder bug is fixed, increasing it too often
 * doesnt hurt but not increasing it could cause difficulty for decoders in the
 * future
 */
#define XVID_BS_VERSION 41

/*****************************************************************************
 * error codes
 ****************************************************************************/

	/*	all functions return values <0 indicate error */

#define XVID_ERR_FAIL		-1		/* general fault */
#define XVID_ERR_MEMORY		-2		/* memory allocation error */
#define XVID_ERR_FORMAT		-3		/* file format error */
#define XVID_ERR_VERSION	-4		/* structure version not supported */
#define XVID_ERR_END		-5		/* encoder only; end of stream reached */



/*****************************************************************************
 * xvid_image_t
 ****************************************************************************/

/* colorspace values */

#define XVID_CSP_PLANAR   (1<< 0) /* 4:2:0 planar (==I420, except for pointers/strides) */
#define XVID_CSP_USER	  XVID_CSP_PLANAR
#define XVID_CSP_I420     (1<< 1) /* 4:2:0 planar */
#define XVID_CSP_YV12     (1<< 2) /* 4:2:0 planar */
#define XVID_CSP_YUY2     (1<< 3) /* 4:2:2 packed */
#define XVID_CSP_UYVY     (1<< 4) /* 4:2:2 packed */
#define XVID_CSP_YVYU     (1<< 5) /* 4:2:2 packed */
#define XVID_CSP_BGRA     (1<< 6) /* 32-bit bgra packed */
#define XVID_CSP_ABGR     (1<< 7) /* 32-bit abgr packed */
#define XVID_CSP_RGBA     (1<< 8) /* 32-bit rgba packed */
#define XVID_CSP_ARGB     (1<<15) /* 32-bit argb packed */
#define XVID_CSP_BGR      (1<< 9) /* 24-bit bgr packed */
#define XVID_CSP_RGB555   (1<<10) /* 16-bit rgb555 packed */
#define XVID_CSP_RGB565   (1<<11) /* 16-bit rgb565 packed */
#define XVID_CSP_SLICE    (1<<12) /* decoder only: 4:2:0 planar, per slice rendering */
#define XVID_CSP_INTERNAL (1<<13) /* decoder only: 4:2:0 planar, returns ptrs to internal buffers */
#define XVID_CSP_NULL     (1<<14) /* decoder only: dont output anything */
#define XVID_CSP_VFLIP    (1<<31) /* vertical flip mask */

/* xvid_image_t
	for non-planar colorspaces use only plane[0] and stride[0]
	four plane reserved for alpha*/
typedef struct {
	__s32 csp;				/* [in] colorspace; or with XVID_CSP_VFLIP to perform vertical flip */
	void * plane[4];		/* [in] image plane ptrs */
	__s32 stride[4];			/* [in] image stride; "bytes per row"*/
} xvid_image_t;

/* video-object-sequence profiles */
#define XVID_PROFILE_S_L0    0x08 /* simple */
#define XVID_PROFILE_S_L1    0x01
#define XVID_PROFILE_S_L2    0x02
#define XVID_PROFILE_S_L3    0x03
#define XVID_PROFILE_ARTS_L1 0x91 /* advanced realtime simple */
#define XVID_PROFILE_ARTS_L2 0x92
#define XVID_PROFILE_ARTS_L3 0x93
#define XVID_PROFILE_ARTS_L4 0x94
#define XVID_PROFILE_AS_L0   0xf0 /* advanced simple */
#define XVID_PROFILE_AS_L1   0xf1
#define XVID_PROFILE_AS_L2   0xf2
#define XVID_PROFILE_AS_L3   0xf3
#define XVID_PROFILE_AS_L4   0xf4

/* aspect ratios */
#define XVID_PAR_11_VGA    1 /* 1:1 vga (square), default if supplied PAR is not a valid value */
#define XVID_PAR_43_PAL    2 /* 4:3 pal (12:11 625-line) */
#define XVID_PAR_43_NTSC   3 /* 4:3 ntsc (10:11 525-line) */
#define XVID_PAR_169_PAL   4 /* 16:9 pal (16:11 625-line) */
#define XVID_PAR_169_NTSC  5 /* 16:9 ntsc (40:33 525-line) */
#define XVID_PAR_EXT      15 /* extended par; use par_width, par_height */

/* frame type flags */
#define XVID_TYPE_VOL     -1 /* decoder only: vol was decoded */
#define XVID_TYPE_NOTHING  0 /* decoder only (encoder stats): nothing was decoded/encoded */
#define XVID_TYPE_AUTO     0 /* encoder: automatically determine coding type */
#define XVID_TYPE_IVOP     1 /* intra frame */
#define XVID_TYPE_PVOP     2 /* predicted frame */
#define XVID_TYPE_BVOP     3 /* bidirectionally encoded */
#define XVID_TYPE_SVOP     4 /* predicted+sprite frame */


/*****************************************************************************
 * xvid_global()
 ****************************************************************************/

/* cpu_flags definitions (make sure to sync this with cpuid.asm for ia32) */

#define XVID_CPU_FORCE    (1<<31) /* force passed cpu flags */
#define XVID_CPU_ASM      (1<< 7) /* native assembly */
/* ARCH_IS_IA32 */
#define XVID_CPU_MMX      (1<< 0) /*       mmx : pentiumMMX,k6 */
#define XVID_CPU_MMXEXT   (1<< 1) /*   mmx-ext : pentium2, athlon */
#define XVID_CPU_SSE      (1<< 2) /*       sse : pentium3, athlonXP */
#define XVID_CPU_SSE2     (1<< 3) /*      sse2 : pentium4, athlon64 */
#define XVID_CPU_3DNOW    (1<< 4) /*     3dnow : k6-2 */
#define XVID_CPU_3DNOWEXT (1<< 5) /* 3dnow-ext : athlon */
#define XVID_CPU_TSC      (1<< 6) /*       tsc : Pentium */
/* ARCH_IS_PPC */
#define XVID_CPU_ALTIVEC  (1<< 0) /* altivec */


#define XVID_DEBUG_ERROR     (1<< 0)
#define XVID_DEBUG_STARTCODE (1<< 1)
#define XVID_DEBUG_HEADER    (1<< 2)
#define XVID_DEBUG_TIMECODE  (1<< 3)
#define XVID_DEBUG_MB        (1<< 4)
#define XVID_DEBUG_COEFF     (1<< 5)
#define XVID_DEBUG_MV        (1<< 6)
#define XVID_DEBUG_RC        (1<< 7)
#define XVID_DEBUG_DEBUG     (1<<31)

/* XVID_GBL_INIT param1 */
typedef struct {
	__s32 version;
	unsigned __s32 cpu_flags; /* [in:opt] zero = autodetect cpu; XVID_CPU_FORCE|{cpu features} = force cpu features */
	__s32 debug;     /* [in:opt] debug level */
} xvid_gbl_init_t;


/* XVID_GBL_INFO param1 */
typedef struct {
	__s32 version;
	__s32 actual_version; /* [out] returns the actual xvidcore version */
	const __s8 * build; /* [out] if !null, points to description of this xvid core build */
	unsigned __s32 cpu_flags;      /* [out] detected cpu features */
	__s32 num_threads;    /* [out] detected number of cpus/threads */
} xvid_gbl_info_t;


/* XVID_GBL_CONVERT param1 */
typedef struct {
	__s32 version;
	xvid_image_t input;  /* [in] input image & colorspace */
	xvid_image_t output; /* [in] output image & colorspace */
	__s32 width;           /* [in] width */
	__s32 height;          /* [in] height */
	__s32 interlacing;     /* [in] interlacing */
} xvid_gbl_convert_t;


#define XVID_GBL_INIT    0 /* initialize xvidcore; must be called before using xvid_decore, or xvid_encore) */
#define XVID_GBL_INFO    1 /* return some info about xvidcore, and the host computer */
#define XVID_GBL_CONVERT 2 /* colorspace conversion utility */

extern __s32 xvid_global(void *handle, __s32 opt, void *param1, void *param2);


/*****************************************************************************
 * xvid_decore()
 ****************************************************************************/

#define XVID_DEC_CREATE  0 /* create decore instance; return 0 on success */
#define XVID_DEC_DESTROY 1 /* destroy decore instance: return 0 on success */
#define XVID_DEC_DECODE  2 /* decode a frame: returns number of bytes consumed >= 0 */

extern __s32 xvid_decore(void *handle, __s32 opt, void *param1, void *param2);

/* XVID_DEC_CREATE param 1
	image width & height may be specified here when the dimensions are
	known in advance. */
typedef struct {
	__s32 version;
	__s32 width;     /* [in:opt] image width */
	__s32 height;    /* [in:opt] image width */
	void * handle; /* [out]	   decore context handle */
} xvid_dec_create_t;


/* XVID_DEC_DECODE param1 */
/* general flags */
#define XVID_LOWDELAY      (1<<0) /* lowdelay mode  */
#define XVID_DISCONTINUITY (1<<1) /* indicates break in stream */
#define XVID_DEBLOCKY      (1<<2) /* perform luma deblocking */
#define XVID_DEBLOCKUV     (1<<3) /* perform chroma deblocking */
#define XVID_FILMEFFECT    (1<<4) /* adds film grain */
#define XVID_DERINGUV      (1<<5) /* perform chroma deringing, requires deblocking to work */
#define XVID_DERINGY       (1<<6) /* perform luma deringing, requires deblocking to work */

#define XVID_DEC_FAST      (1<<29) /* disable postprocessing to decrease cpu usage *todo* */
#define XVID_DEC_DROP      (1<<30) /* drop bframes to decrease cpu usage *todo* */
#define XVID_DEC_PREROLL   (1<<31) /* decode as fast as you can, don't even show output *todo* */

typedef struct {
	__s32 version;
	__s32 general;         /* [in:opt] general flags */
	void *bitstream;     /* [in]     bitstream (read from)*/
	__s32 length;          /* [in]     bitstream length */
	xvid_image_t output; /* [in]     output image (written to) */
/* ------- v1.1.x ------- */
	__s32 brightness;		 /* [in]	 brightness offset (0=none) */
} xvid_dec_frame_t;


/* XVID_DEC_DECODE param2 :: optional */
typedef struct
{
	__s32 version;

	__s32 type;                   /* [out] output data type */
	union {
		struct { /* type>0 {XVID_TYPE_IVOP,XVID_TYPE_PVOP,XVID_TYPE_BVOP,XVID_TYPE_SVOP} */
			__s32 general;        /* [out] flags */
			__s32 time_base;      /* [out] time base */
			__s32 time_increment; /* [out] time increment */

			/* XXX: external deblocking stuff */
			__s32 * qscale;	    /* [out] pointer to quantizer table */
			__s32 qscale_stride;  /* [out] quantizer scale stride */

		} vop;
		struct {	/* XVID_TYPE_VOL */
			__s32 general;        /* [out] flags */
			__s32 width;          /* [out] width */
			__s32 height;         /* [out] height */
			__s32 par;            /* [out] pixel aspect ratio (refer to XVID_PAR_xxx above) */
			__s32 par_width;      /* [out] aspect ratio width  [1..255] */
			__s32 par_height;     /* [out] aspect ratio height [1..255] */
		} vol;
	} data;
} xvid_dec_stats_t;

#define XVID_ZONE_QUANT  (1<<0)
#define XVID_ZONE_WEIGHT (1<<1)

typedef struct
{
	__s32 frame;
	__s32 mode;
	__s32 increment;
	__s32 base;
} xvid_enc_zone_t;


/*----------------------------------------------------------------------------
 * xvid_enc_stats_t structure
 *
 * Used in:
 *  - xvid_plg_data_t structure
 *  - optional parameter in xvid_encore() function
 *
 * .coding_type = XVID_TYPE_NOTHING if the stats are not given
 *--------------------------------------------------------------------------*/

typedef struct {
	__s32 version;

	/* encoding parameters */
	__s32 type;      /* [out] coding type */
	__s32 quant;     /* [out] frame quantizer */
	__s32 vol_flags; /* [out] vol flags (see above) */
	__s32 vop_flags; /* [out] vop flags (see above) */

	/* bitrate */
	__s32 length;    /* [out] frame length */

	__s32 hlength;   /* [out] header length (bytes) */
	__s32 kblks;     /* [out] number of blocks compressed as Intra */
	__s32 mblks;     /* [out] number of blocks compressed as Inter */
	__s32 ublks;     /* [out] number of blocks marked as not_coded */

	__s32 sse_y;     /* [out] Y plane's sse */
	__s32 sse_u;     /* [out] U plane's sse */
	__s32 sse_v;     /* [out] V plane's sse */
} xvid_enc_stats_t;

/*****************************************************************************
  xvid plugin system -- internals

  xvidcore will call XVID_PLG_INFO and XVID_PLG_CREATE during XVID_ENC_CREATE
  before encoding each frame xvidcore will call XVID_PLG_BEFORE
  after encoding each frame xvidcore will call XVID_PLG_AFTER
  xvidcore will call XVID_PLG_DESTROY during XVID_ENC_DESTROY
 ****************************************************************************/


#define XVID_PLG_CREATE  (1<<0)
#define XVID_PLG_DESTROY (1<<1)
#define XVID_PLG_INFO    (1<<2)
#define XVID_PLG_BEFORE  (1<<3)
#define XVID_PLG_FRAME   (1<<4)
#define XVID_PLG_AFTER   (1<<5)

/* xvid_plg_info_t.flags */
#define XVID_REQORIGINAL (1<<0) /* plugin requires a copy of the original (uncompressed) image */
#define XVID_REQPSNR     (1<<1) /* plugin requires psnr between the uncompressed and compressed image*/
#define XVID_REQDQUANTS  (1<<2) /* plugin requires access to the dquant table */


typedef struct
{
	__s32 version;
	__s32 flags;   /* [in:opt] plugin flags */
} xvid_plg_info_t;


typedef struct
{
	__s32 version;

	__s32 num_zones;           /* [out] */
	xvid_enc_zone_t * zones; /* [out] */

	__s32 width;               /* [out] */
	__s32 height;              /* [out] */
	__s32 mb_width;            /* [out] */
	__s32 mb_height;           /* [out] */
	__s32 fincr;               /* [out] */
	__s32 fbase;               /* [out] */

	void * param;            /* [out] */
} xvid_plg_create_t;


typedef struct
{
	__s32 version;

	__s32 num_frames; /* [out] total frame encoded */
} xvid_plg_destroy_t;

typedef struct
{
	__s32 version;

	xvid_enc_zone_t * zone; /* [out] current zone */

	__s32 width;              /* [out] */
	__s32 height;             /* [out] */
	__s32 mb_width;           /* [out] */
	__s32 mb_height;          /* [out] */
	__s32 fincr;              /* [out] */
	__s32 fbase;              /* [out] */

	__s32 min_quant[3];       /* [out] */
	__s32 max_quant[3];       /* [out] */

	xvid_image_t reference; /* [out] -> [out] */
	xvid_image_t current;   /* [out] -> [in,out] */
	xvid_image_t original;  /* [out] after: points the original (uncompressed) copy of the current frame */
	__s32 frame_num;          /* [out] frame number */

	__s32 type;               /* [in,out] */
	__s32 quant;              /* [in,out] */

	__s32 * dquant;           /* [in,out]	pointer to diff quantizer table */
	__s32 dquant_stride;      /* [in,out]	diff quantizer stride */

	__s32 vop_flags;          /* [in,out] */
	__s32 vol_flags;          /* [in,out] */
	__s32 motion_flags;       /* [in,out] */

/* Deprecated, use the stats field instead.
 * Will disapear before 1.0 */
	__s32 length;             /* [out] after: length of encoded frame */
	__s32 kblks;              /* [out] number of blocks compressed as Intra */
	__s32 mblks;              /* [out] number of blocks compressed as Inter */
	__s32 ublks;              /* [out] number of blocks marked not_coded */
	__s32 sse_y;              /* [out] Y plane's sse */
	__s32 sse_u;              /* [out] U plane's sse */
	__s32 sse_v;              /* [out] V plane's sse */
/* End of duplicated data, kept only for binary compatibility */

	__s32 bquant_ratio;       /* [in] */
	__s32 bquant_offset;      /* [in] */

	xvid_enc_stats_t stats; /* [out] frame statistics */
} xvid_plg_data_t;

/*****************************************************************************
  xvid plugin system -- external

  the application passes xvid an array of "xvid_plugin_t" at XVID_ENC_CREATE. the array
  indicates the plugin function pointer and plugin-specific data.
  xvidcore handles the rest. example:

  xvid_enc_create_t create;
  xvid_enc_plugin_t plugins[2];

  plugins[0].func = xvid_psnr_func;
  plugins[0].param = NULL;
  plugins[1].func = xvid_cbr_func;
  plugins[1].param = &cbr_data;

  create.num_plugins = 2;
  create.plugins = plugins;

 ****************************************************************************/

typedef __s32 (xvid_plugin_func)(void * handle, __s32 opt, void * param1, void * param2);

typedef struct
{
	xvid_plugin_func * func;
	void * param;
} xvid_enc_plugin_t;


extern xvid_plugin_func xvid_plugin_single;   /* single-pass rate control */
extern xvid_plugin_func xvid_plugin_2pass1;   /* two-pass rate control: first pass */
extern xvid_plugin_func xvid_plugin_2pass2;   /* two-pass rate control: second pass */

extern xvid_plugin_func xvid_plugin_lumimasking;  /* lumimasking */

extern xvid_plugin_func xvid_plugin_psnr;	/* write psnr values to stdout */
extern xvid_plugin_func xvid_plugin_dump;	/* dump before and after yuvpgms */


/* single pass rate control
 * CBR and Constant quantizer modes */
typedef struct
{
	__s32 version;

	__s32 bitrate;               /* [in] bits per second */
	__s32 reaction_delay_factor; /* [in] */
	__s32 averaging_period;      /* [in] */
	__s32 buffer;                /* [in] */
} xvid_plugin_single_t;


typedef struct {
	__s32 version;

	__s8 * filename;
} xvid_plugin_2pass1_t;


#define XVID_PAYBACK_BIAS 0 /* payback with bias */
#define XVID_PAYBACK_PROP 1 /* payback proportionally */

typedef struct {
	__s32 version;

	__s32 bitrate;                  /* [in] target bitrate (bits per second) */
	__s8 * filename;              /* [in] first pass stats filename */

	__s32 keyframe_boost;           /* [in] keyframe boost percentage: [0..100] */
	__s32 curve_compression_high;   /* [in] percentage of compression performed on the high part of the curve (above average) */
	__s32 curve_compression_low;    /* [in] percentage of compression performed on the low  part of the curve (below average) */
	__s32 overflow_control_strength;/* [in] Payback delay expressed in number of frames */
	__s32 max_overflow_improvement; /* [in] percentage of allowed range for a frame that gets bigger because of overflow bonus */
	__s32 max_overflow_degradation; /* [in] percentage of allowed range for a frame that gets smaller because of overflow penalty */

	__s32 kfreduction;              /* [in] maximum bitrate reduction applied to an iframe under the kfthreshold distance limit */
	__s32 kfthreshold;              /* [in] if an iframe is closer to the next iframe than this distance, a quantity of bits
								   *      is substracted from its bit allocation. The reduction is computed as multiples of
								   *      kfreduction/kthreshold. It reaches kfreduction when the distance == kfthreshold,
								   *      0 for 1<distance<kfthreshold */

	__s32 container_frame_overhead; /* [in] How many bytes the controller has to compensate per frame due to container format overhead */

/* ------- v1.1.x ------- */
	__s32 vbv_size;                 /* [in] buffer size (bits) */
	__s32 vbv_initial;              /* [in] initial buffer occupancy (bits) */
	__s32 vbv_maxrate;              /* [in] max processing bitrate (bits per second) */
	__s32 vbv_peakrate;             /* [in:opt] max average bitrate over 3 seconds (bits per second) */

}xvid_plugin_2pass2_t;

/*****************************************************************************
 *                             ENCODER API
 ****************************************************************************/

/*----------------------------------------------------------------------------
 * Encoder operations
 *--------------------------------------------------------------------------*/

#define XVID_ENC_CREATE  0 /* create encoder instance; returns 0 on success */
#define XVID_ENC_DESTROY 1 /* destroy encoder instance; returns 0 on success */
#define XVID_ENC_ENCODE  2 /* encode a frame: returns number of ouput bytes
                            * 0 means this frame should not be written (ie. encoder lag) */


/*----------------------------------------------------------------------------
 * Encoder entry point
 *--------------------------------------------------------------------------*/

extern __s32 xvid_encore(void *handle, __s32 opt, void *param1, void *param2);

/* Quick API reference
 *
 * XVID_ENC_CREATE operation
 *  - handle: ignored
 *  - opt: XVID_ENC_CREATE
 *  - param1: address of a xvid_enc_create_t structure
 *  - param2: ignored
 *
 * XVID_ENC_ENCODE operation
 *  - handle: an instance returned by a CREATE op
 *  - opt: XVID_ENC_ENCODE
 *  - param1: address of a xvid_enc_frame_t structure
 *  - param2: address of a xvid_enc_stats_t structure (optional)
 *            its return value is asynchronous to what is written to the buffer
 *            depending on the delay introduced by bvop use. It's display
 *            ordered.
 *
 * XVID_ENC_DESTROY operation
 *  - handle: an instance returned by a CREATE op
 *  - opt: XVID_ENC_DESTROY
 *  - param1: ignored
 *  - param2: ignored
 */


/*----------------------------------------------------------------------------
 * "Global" flags
 *
 * These flags are used for xvid_enc_create_t->global field during instance
 * creation (operation XVID_ENC_CREATE)
 *--------------------------------------------------------------------------*/

#define XVID_GLOBAL_PACKED            (1<<0) /* packed bitstream */
#define XVID_GLOBAL_CLOSED_GOP        (1<<1) /* closed_gop:	was DX50BVOP dx50 bvop compatibility */
#define XVID_GLOBAL_EXTRASTATS_ENABLE (1<<2)
#if 0
#define XVID_GLOBAL_VOL_AT_IVOP       (1<<3) /* write vol at every ivop: WIN32/divx compatibility */
#define XVID_GLOBAL_FORCE_VOL         (1<<4) /* when vol-based parameters are changed, insert an ivop NOT recommended */
#endif
#define XVID_GLOBAL_DIVX5_USERDATA    (1<<5) /* write divx5 userdata string 
                                                this is implied if XVID_GLOBAL_PACKED is set */

/*----------------------------------------------------------------------------
 * "VOL" flags
 *
 * These flags are used for xvid_enc_frame_t->vol_flags field during frame
 * encoding (operation XVID_ENC_ENCODE)
 *--------------------------------------------------------------------------*/

#define XVID_VOL_MPEGQUANT      (1<<0) /* enable MPEG type quantization */
#define XVID_VOL_EXTRASTATS     (1<<1) /* enable plane sse stats */
#define XVID_VOL_QUARTERPEL     (1<<2) /* enable quarterpel: frames will encoded as quarterpel */
#define XVID_VOL_GMC            (1<<3) /* enable GMC; frames will be checked for gmc suitability */
#define XVID_VOL_REDUCED_ENABLE (1<<4) /* enable reduced resolution vops: frames will be checked for rrv suitability */
									   /* NOTE:  the reduced resolution feature is not supported anymore. This flag will have no effect! */
#define XVID_VOL_INTERLACING    (1<<5) /* enable interlaced encoding */


/*----------------------------------------------------------------------------
 * "VOP" flags
 *
 * These flags are used for xvid_enc_frame_t->vop_flags field during frame
 * encoding (operation XVID_ENC_ENCODE)
 *--------------------------------------------------------------------------*/

/* Always valid */
#define XVID_VOP_DEBUG                (1<< 0) /* print debug messages in frames */
#define XVID_VOP_HALFPEL              (1<< 1) /* use halfpel interpolation */
#define XVID_VOP_INTER4V              (1<< 2) /* use 4 motion vectors per MB */
#define XVID_VOP_TRELLISQUANT         (1<< 3) /* use trellis based R-D "optimal" quantization */
#define XVID_VOP_CHROMAOPT            (1<< 4) /* enable chroma optimization pre-filter */
#define XVID_VOP_CARTOON              (1<< 5) /* use 'cartoon mode' */
#define XVID_VOP_GREYSCALE            (1<< 6) /* enable greyscale only mode (even for  color input material chroma is ignored) */
#define XVID_VOP_HQACPRED             (1<< 7) /* high quality ac prediction */
#define XVID_VOP_MODEDECISION_RD      (1<< 8) /* enable DCT-ME and use it for mode decision */
#define XVID_VOP_FAST_MODEDECISION_RD (1<<12) /* use simplified R-D mode decision */
#define XVID_VOP_RD_BVOP              (1<<13) /* enable rate-distortion mode decision in b-frames */

/* Only valid for vol_flags|=XVID_VOL_INTERLACING */
#define XVID_VOP_TOPFIELDFIRST        (1<< 9) /* set top-field-first flag  */
#define XVID_VOP_ALTERNATESCAN        (1<<10) /* set alternate vertical scan flag */

/* only valid for vol_flags|=XVID_VOL_REDUCED_ENABLED */
#define XVID_VOP_REDUCED              (1<<11) /* reduced resolution vop */
											  /* NOTE: reduced resolution feature is not supported anymore. This flag will have no effect! */

/*----------------------------------------------------------------------------
 * "Motion" flags
 *
 * These flags are used for xvid_enc_frame_t->motion field during frame
 * encoding (operation XVID_ENC_ENCODE)
 *--------------------------------------------------------------------------*/

/* Motion Estimation Search Patterns */
#define XVID_ME_ADVANCEDDIAMOND16     (1<< 0) /* use advdiamonds instead of diamonds as search pattern */
#define XVID_ME_ADVANCEDDIAMOND8      (1<< 1) /* use advdiamond for XVID_ME_EXTSEARCH8 */
#define XVID_ME_USESQUARES16          (1<< 2) /* use squares instead of diamonds as search pattern */
#define XVID_ME_USESQUARES8           (1<< 3) /* use square for XVID_ME_EXTSEARCH8 */

/* SAD operator based flags */
#define XVID_ME_HALFPELREFINE16       (1<< 4)
#define XVID_ME_HALFPELREFINE8        (1<< 6)
#define XVID_ME_QUARTERPELREFINE16    (1<< 7)
#define XVID_ME_QUARTERPELREFINE8     (1<< 8)
#define XVID_ME_GME_REFINE            (1<< 9)
#define XVID_ME_EXTSEARCH16           (1<<10) /* extend PMV by more searches */
#define XVID_ME_EXTSEARCH8            (1<<11) /* use diamond/square for extended 8x8 search */
#define XVID_ME_CHROMA_PVOP           (1<<12) /* also use chroma for P_VOP/S_VOP ME */
#define XVID_ME_CHROMA_BVOP           (1<<13) /* also use chroma for B_VOP ME */
#define XVID_ME_FASTREFINE16          (1<<25) /* use low-complexity refinement functions */
#define XVID_ME_FASTREFINE8           (1<<29) /* low-complexity 8x8 sub-block refinement */

/* Rate Distortion based flags
 * Valid when XVID_VOP_MODEDECISION_RD is enabled */
#define XVID_ME_HALFPELREFINE16_RD    (1<<14) /* perform RD-based halfpel refinement */
#define XVID_ME_HALFPELREFINE8_RD     (1<<15) /* perform RD-based halfpel refinement for 8x8 mode */
#define XVID_ME_QUARTERPELREFINE16_RD (1<<16) /* perform RD-based qpel refinement */
#define XVID_ME_QUARTERPELREFINE8_RD  (1<<17) /* perform RD-based qpel refinement for 8x8 mode */
#define XVID_ME_EXTSEARCH_RD          (1<<18) /* perform RD-based search using square pattern enable XVID_ME_EXTSEARCH8 to do this in 8x8 search as well */
#define XVID_ME_CHECKPREDICTION_RD    (1<<19) /* always check vector equal to prediction */

/* Other */
#define XVID_ME_DETECT_STATIC_MOTION  (1<<24) /* speed-up ME by detecting stationary scenes */
#define XVID_ME_SKIP_DELTASEARCH      (1<<26) /* speed-up by skipping b-frame delta search */
#define XVID_ME_FAST_MODEINTERPOLATE  (1<<27) /* speed-up by partly skipping interpolate mode */
#define XVID_ME_BFRAME_EARLYSTOP      (1<<28) /* speed-up by early exiting b-search */

/* Unused */
#define XVID_ME_UNRESTRICTED16        (1<<20) /* unrestricted ME, not implemented */
#define XVID_ME_OVERLAPPING16         (1<<21) /* overlapping ME, not implemented */
#define XVID_ME_UNRESTRICTED8         (1<<22) /* unrestricted ME, not implemented */
#define XVID_ME_OVERLAPPING8          (1<<23) /* overlapping ME, not implemented */


/*----------------------------------------------------------------------------
 * xvid_enc_create_t structure definition
 *
 * This structure is passed as param1 during an instance creation (operation
 * XVID_ENC_CREATE)
 *--------------------------------------------------------------------------*/

typedef struct {
	__s32 version;

	__s32 profile;                 /* [in] profile@level; refer to XVID_PROFILE_xxx */
	__s32 width;                   /* [in] frame dimensions; width, pixel units */
	__s32 height;                  /* [in] frame dimensions; height, pixel units */

	__s32 num_zones;               /* [in:opt] number of bitrate zones */
	xvid_enc_zone_t * zones;     /*          ^^ zone array */

	__s32 num_plugins;             /* [in:opt] number of plugins */
	xvid_enc_plugin_t * plugins; /*          ^^ plugin array */

	__s32 num_threads;             /* [in:opt] number of threads */
	__s32 max_bframes;             /* [in:opt] max sequential bframes (0=disable bframes) */

	__s32 global;                  /* [in:opt] global flags; controls encoding behavior */

	/* --- vol-based stuff; included here for convenience */
	__s32 fincr;                   /* [in:opt] framerate increment; set to zero for variable framerate */
	__s32 fbase;                   /* [in] framerate base frame_duration = fincr/fbase seconds*/
    /* ---------------------------------------------- */

	/* --- vop-based; included here for convenience */
	__s32 max_key_interval;        /* [in:opt] the maximum interval between key frames */

	__s32 frame_drop_ratio;        /* [in:opt] frame dropping: 0=drop none... 100=drop all */

	__s32 bquant_ratio;            /* [in:opt] bframe quantizer multipier/offeset; used to decide bframes quant when bquant==-1 */
	__s32 bquant_offset;           /* bquant = (avg(past_ref_quant,future_ref_quant)*bquant_ratio + bquant_offset) / 100 */

	__s32 min_quant[3];            /* [in:opt] */
	__s32 max_quant[3];            /* [in:opt] */
	/* ---------------------------------------------- */

	void *handle;                /* [out] encoder instance handle */
} xvid_enc_create_t;


/*----------------------------------------------------------------------------
 * xvid_enc_frame_t structure definition
 *
 * This structure is passed as param1 during a frame encoding (operation
 * XVID_ENC_ENCODE)
 *--------------------------------------------------------------------------*/

/* out value for the frame structure->type field
 * unlike stats output in param2, this field is not asynchronous and tells
 * the client app, if the frame written into the stream buffer is an ivop
 * usually used for indexing purpose in the container */
#define XVID_KEYFRAME (1<<1)

/* The structure */
typedef struct {
	__s32 version;

	/* VOL related stuff
	 * unless XVID_FORCEVOL is set, the encoder will not react to any changes
	 * here until the next VOL (keyframe). */

	__s32 vol_flags;                     /* [in] vol flags */
	unsigned __s8 *quant_intra_matrix; /* [in:opt] custom intra qmatrix */
	unsigned __s8 *quant_inter_matrix; /* [in:opt] custom inter qmatrix */

	__s32 par;                           /* [in:opt] pixel aspect ratio (refer to XVID_PAR_xxx above) */
	__s32 par_width;                     /* [in:opt] aspect ratio width */
	__s32 par_height;                    /* [in:opt] aspect ratio height */

	/* Other fields that can change on a frame base */

	__s32 fincr;                         /* [in:opt] framerate increment, for variable framerate only */
	__s32 vop_flags;                     /* [in] (general)vop-based flags */
	__s32 motion;                        /* [in] ME options */

	xvid_image_t input;                /* [in] input image (read from) */

	__s32 type;                          /* [in:opt] coding type */
	__s32 quant;                         /* [in] frame quantizer; if <=0, automatic (ratecontrol) */
	__s32 bframe_threshold;

	void *bitstream;                   /* [in:opt] bitstream ptr (written to)*/
	__s32 length;                        /* [in:opt] bitstream length (bytes) */

	__s32 out_flags;                     /* [out] bitstream output flags */
} xvid_enc_frame_t;

#ifdef __cplusplus
}
#endif

#endif
