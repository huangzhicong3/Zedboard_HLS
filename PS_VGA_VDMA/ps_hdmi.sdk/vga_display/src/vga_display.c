/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include "display_demo.h"
#include "display_ctrl/display_ctrl.h"
#include <stdio.h>
#include "math.h"
#include <ctype.h>
#include <stdlib.h>
#include "xil_types.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "xuartps.h"
#include "xil_printf.h"
/*
 * XPAR redefines
 */
#define DYNCLK_BASEADDR XPAR_AXI_DYNCLK_0_BASEADDR
#define VGA_VDMA_ID XPAR_AXIVDMA_0_DEVICE_ID
#define DISP_VTC_ID XPAR_VTC_0_DEVICE_ID
#define VID_VTC_IRPT_ID XPS_FPGA3_INT_ID
#define VID_GPIO_IRPT_ID XPS_FPGA4_INT_ID
#define SCU_TIMER_ID XPAR_SCUTIMER_DEVICE_ID
#define UART_BASEADDR XPAR_PS7_UART_1_BASEADDR

#define UART_COM_ADDR                 XPAR_XUARTPS_1_BASEADDR
#define UART_DEVICE_ADDR              XPAR_XUARTPS_0_BASEADDR
#define UART_COM_DEVICE               XPAR_XUARTPS_1_DEVICE_ID
#define UART_DEVICE_DEVICE            XPAR_XUARTPS_0_DEVICE_ID

#define EXPAND					13
#define SHIFT(x0,y0)            (x0*4 + DEMO_STRIDE*y0)
/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

/*
 * Display Driver structs
 */
DisplayCtrl dispCtrl;
XAxiVdma vdma;

/*
 * Framebuffers for video data
 */
u8 frameBuf[DISPLAY_NUM_FRAMES][DEMO_MAX_FRAME] __attribute__ ((aligned(64)));
u8 *pFrames[DISPLAY_NUM_FRAMES]; //array of pointers to the frame buffers



int main()
{
	short *p0 = (short*)malloc(sizeof(short) * 32*EXPAND*24*EXPAND);
    u8  low,high,start,r,g,b;
    u32  temp;
    u32 i,j,t1,t2;

    u8 Status;
    XAxiVdma_Config *vdmaConfig;

    /*
    * Initialize an array of pointers to the 3 frame buffers
    */
    for (i = 0; i < DISPLAY_NUM_FRAMES; i++)
    {
    	pFrames[i] = frameBuf[i];
    }

	/*
	 * Initialize VDMA driver
	 */
	vdmaConfig = XAxiVdma_LookupConfig(VGA_VDMA_ID);
	if (!vdmaConfig)
	{
		xil_printf("No video DMA found for ID %d\r\n", VGA_VDMA_ID);
	}
	Status = XAxiVdma_CfgInitialize(&vdma, vdmaConfig, vdmaConfig->BaseAddress);
	if (Status != XST_SUCCESS)
	{
		xil_printf("VDMA Configuration Initialization failed %d\r\n", Status);
	}

	/*
	* Initialize the Display controller and start it
	*/
	Status = DisplayInitialize(&dispCtrl, &vdma, DISP_VTC_ID, DYNCLK_BASEADDR, pFrames, DEMO_STRIDE);
	if (Status != XST_SUCCESS)
	{
		xil_printf("Display Ctrl initialization failed during demo initialization%d\r\n", Status);
	}
	Status = DisplayStart(&dispCtrl);
	if (Status != XST_SUCCESS)
	{
		xil_printf("Couldn't start display during demo initialization%d\r\n", Status);
	}

	memset(pFrames[dispCtrl.curFrame], 0, 1920 * 1080 * 4);

	while (1)
	{
	if (XUartPs_RecvByte(UART_DEVICE_ADDR)==0x5a)
	if (XUartPs_RecvByte(UART_DEVICE_ADDR)==0x5a)
	if (XUartPs_RecvByte(UART_DEVICE_ADDR)==0x02)
	if (XUartPs_RecvByte(UART_DEVICE_ADDR)==0x06)
	{
		start = 1;
		xil_printf("start\n\r");
	}
	if (start)
	{
	for(j=0;j<24;j++)
	for(i=0;i<32;i++)
	{
		r = 0x0;
		g = 0x0;
		b = 0xF;
		low = XUartPs_RecvByte(UART_DEVICE_ADDR);
		high = XUartPs_RecvByte(UART_DEVICE_ADDR);
		temp = (low & 0xFF) + ((high & 0xFF)<<8);
		if (temp > 2800)
		if (temp < 2960)
		g = (temp-2800)/10 -1;
		else if (temp < 3120)
		{
			g = 0xF;
			b = 15 - (temp-2960)/10;
		}
		else if (temp < 3280)
		{
			g = 0xF;
			b = 0;
			r = (temp-3120)/10 -1;
		}
		else if (temp < 3440)
		{
			r = 0xF;
			b = 0;
			g = 15 - (temp-3280)/10;
		}
		else
		{
			r = 0xF;
			b = 0;
		}


		for(t2=0;t2<EXPAND;t2++)
		{
		for(t1=0;t1<EXPAND;t1++)
		{
			pFrames[dispCtrl.curFrame][SHIFT(450,250)+EXPAND*dispCtrl.stride*j +EXPAND*4*i + t1*4 +dispCtrl.stride*t2 +0] = g<<4;
			pFrames[dispCtrl.curFrame][SHIFT(450,250)+EXPAND*dispCtrl.stride*j +EXPAND*4*i + t1*4 +dispCtrl.stride*t2 +1] = r;
			pFrames[dispCtrl.curFrame][SHIFT(450,250)+EXPAND*dispCtrl.stride*j +EXPAND*4*i + t1*4 +dispCtrl.stride*t2 +1] += b<<4;
		}


		}
	}
	start = 0;
	Xil_DCacheFlushRange((unsigned int) pFrames[dispCtrl.curFrame], DEMO_MAX_FRAME);
	}
	}
}