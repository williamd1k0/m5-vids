/*----------------------------------------------------------------------------/
/ TJpgDec - Tiny JPEG Decompressor include file               (C)ChaN, 2019
/-----------------------------------------------------------------------------/
/  modify by lovyan03
/ May 29, 2019 Tweak for ArduinoESP32
/----------------------------------------------------------------------------*/

#ifndef _TJPGDEC_H_
#define _TJPGDEC_H_
/*---------------------------------------------------------------------------*/
/* System Configurations */

#define	TJPGD_SZBUF		1426	/* Size of stream input buffer */
#define JD_FORMAT		1	/* Output pixel format 0:RGB888 (3 BYTE/pix), 1:RGB565 (1 WORD/pix) */
#define JD_TBLCLIP		0	/* Use table for saturation (might be a bit faster but increases 1K bytes of code size) */

/*---------------------------------------------------------------------------*/


#if defined(_WIN32)	/* Main development platform */
typedef unsigned char	uint8_t;
typedef unsigned short	uint16_t;
typedef short			int16_t;
typedef unsigned long	uint32_t;
typedef long			int32_t;
#else
#include "stdint.h"
#endif

/* Decompressor object structure */
typedef struct TJpgD TJpgD;
struct TJpgD {
	/* Error code */
	typedef enum {
		JDR_OK = 0,	/* 0: Succeeded */
		JDR_INTR,	/* 1: Interrupted by output function */	
		JDR_INP,	/* 2: Device error or wrong termination of input stream */
		JDR_MEM1,	/* 3: Insufficient memory pool for the image */
		JDR_MEM2,	/* 4: Insufficient stream input buffer */
		JDR_PAR,	/* 5: Parameter error */
		JDR_FMT1,	/* 6: Data format error (may be damaged data) */
		JDR_FMT2,	/* 7: Right format but not supported */
		JDR_FMT3	/* 8: Not supported JPEG standard */
	} JRESULT;

	/* Rectangular structure */
	typedef struct {
		int_fast16_t left, right, top, bottom;
	} JRECT;

	uint8_t* dptr;				/* Current data read ptr */
	uint8_t* dpend;				/* data end ptr */
	uint8_t* inbuf;				/* Bit stream input buffer */
	uint_fast8_t dmsk;			/* Current bit in the current read byte */
	uint_fast8_t bayer;			/* Output bayer gain */
	uint_fast8_t msx, msy;		/* MCU size in unit of block (width, height) */
	uint_fast8_t qtid[3];		/* Quantization table ID of each component */
	int_fast16_t dcv[3];		/* Previous DC element of each component */
	uint_fast16_t nrst;			/* Restart inverval */
	int32_t width, height;		/* Size of the input image (pixel) */
	uint8_t* huffbits[2][2];	/* Huffman bit distribution tables [id][dcac] */
	uint_fast16_t* huffcode[2][2];	/* Huffman code word tables [id][dcac] */
	uint8_t* huffdata[2][2];	/* Huffman decoded data tables [id][dcac] */
	int32_t* qttbl[4];			/* Dequantizer tables [id] */
	void* pool;					/* Pointer to available memory pool */
	uint_fast16_t sz_pool;			/* Size of momory pool (bytes available) */
	uint32_t (*infunc)(TJpgD*, uint8_t*, uint32_t);/* Pointer to jpeg stream input function */
	void* device;				/* Pointer to I/O device identifiler for the session */


	JRESULT prepare (uint32_t(*)(TJpgD*,uint8_t*,uint32_t), void*);
	JRESULT decomp (uint32_t(*)(TJpgD*,void*,JRECT*), uint32_t(*)(TJpgD*,uint32_t,uint32_t) = 0, uint32_t = 0);
	JRESULT decomp_multitask (uint32_t(*)(TJpgD*,void*,JRECT*), uint32_t(*)(TJpgD*,uint32_t,uint32_t) = 0, uint32_t = 0);
	static void multitask_begin ();
	static void multitask_end ();
};



#endif /* _TJPGDEC */
