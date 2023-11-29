#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "sleep.h"
#include "xparameters.h"
#include "xiic.h"
#include "xil_io.h"

#include "ov5640.h"
#include "ov5640_cfg_wrds.h"


#define OV5640_IIC_BASEADDR   0x3C


int OV5640_SetReg(uint16_t addr, uint8_t val){
	unsigned Status;
	uint8_t iic_send[4];

    iic_send[0] = addr >> 8;
    iic_send[1] = addr & 0x00FF;
    iic_send[2] = val;

    //xil_printf("0x%02x%02x : 0x%02x\r\n", iic_send[0], iic_send[1], iic_send[2] );
    Status = XIic_Send(XPAR_AXI_IIC_CSI_BASEADDR, OV5640_IIC_BASEADDR, iic_send, 3, XIIC_STOP);
    if(Status == XST_FAILURE){
    	xil_printf("ERROR: Unable to complete IIC operation\r\n");
    	return XST_FAILURE;
    }

    return Status;
}


uint8_t OV5640_GetReg(uint16_t addr){
	unsigned Status;
	uint8_t iic_send[4];
	uint8_t iic_recv[4];

    iic_send[0] = addr >> 8;
    iic_send[1] = addr & 0x00FF;

	Status = XIic_Send(XPAR_AXI_IIC_CSI_BASEADDR, OV5640_IIC_BASEADDR, iic_send, 2, XIIC_REPEATED_START);
    if(Status == XST_FAILURE){
    	xil_printf("ERROR: Unable to complete IIC operation\r\n");
    	return XST_FAILURE;
    }

    XIic_Recv(XPAR_AXI_IIC_CSI_BASEADDR, OV5640_IIC_BASEADDR, iic_recv, 1, XIIC_STOP);

    return iic_recv[0];
}


void OV5640_Reset(){
	uint8_t iic_send[4];
	iic_send[0] = 0x30;
	iic_send[1] = 0x08;
	iic_send[2] = 0x82;
	XIic_Send(XPAR_AXI_IIC_CSI_BASEADDR, OV5640_IIC_BASEADDR, iic_send, 3, XIIC_STOP);

	xil_printf("OV5640 reset & stopped. Waiting for enable signal... \r\n");
}


void OV5640_Enable(){
	uint8_t iic_send[4];
	iic_send[0] = 0x30;
	iic_send[1] = 0x08;
	iic_send[2] = 0x02;
	XIic_Send(XPAR_AXI_IIC_CSI_BASEADDR, OV5640_IIC_BASEADDR, iic_send, 3, XIIC_STOP);

	xil_printf("OV5640 enabled. \r\n");
}


void OV5640_Whoami(){
	uint8_t id_h, id_l;

	xil_printf("Reading OV5640 Chip ID \n\r");
	xil_printf("Registers @  H:0x300A L:0x300B\n\r");

	id_h = OV5640_GetReg(0x300A);
	id_l = OV5640_GetReg(0x300B);

	xil_printf("			Chip ID = %x%x \r\n", id_h, id_l);
    if(id_h != 0x56 || id_l != 0x40) {
    	xil_printf("ERROR: OV5640 chip ID missing or incorrect\r\n");
    	return;
    }
    else {
    	xil_printf("OV5640 chip ID read success\r\n");
    }
}


void OV5640_Init(){
	xil_printf("Loading initial config...\n\r");

	for (int i = 0; i < sizeof(cfg_init_) / sizeof(cfg_init_[0]); i++){
		OV5640_SetReg(cfg_init_[i].addr, cfg_init_[i].data);
		usleep(10000);
	}

	xil_printf("			Done \n\r\n\r");
}


void OV5640_SetVidMode(int vid_mode){

	config_word_t *cfg_;
	int cnt;

    switch (vid_mode)
    {
    	case MODE_720P_1280_720_60fps:
    		cfg_ = (config_word_t*)cfg_720p_60fps_;
    		cnt = sizeof(cfg_720p_60fps_) / sizeof(cfg_720p_60fps_[0]);
    		break;
    	case MODE_1080P_1920_1080_15fps:
    		cfg_ = (config_word_t*)cfg_1080p_15fps_;
    		cnt = sizeof(cfg_1080p_15fps_) / sizeof(cfg_1080p_15fps_[0]);
			break;
    	case MODE_1080P_1920_1080_30fps:
    		cfg_ = (config_word_t*)cfg_1080p_30fps_;
    		cnt = sizeof(cfg_1080p_30fps_) / sizeof(cfg_1080p_30fps_[0]);
    		break;
    	case MODE_1080P_1920_1080_30fps_336M_MIPI:
    		cfg_ = (config_word_t*)cfg_1080p_30fps_336M_mipi_;
    		cnt = sizeof(cfg_1080p_30fps_336M_mipi_) / sizeof(cfg_1080p_30fps_336M_mipi_[0]);
    		break;
    	default:
    		cfg_ = (config_word_t*)cfg_1080p_30fps_;
    		cnt = sizeof(cfg_1080p_30fps_) / sizeof(cfg_1080p_30fps_[0]);
    }

	xil_printf("Loading video config...\n\r");

	for (int i = 0; i < cnt; i++){
		OV5640_SetReg(cfg_[i].addr, cfg_[i].data);
		usleep(10000);
	}


	xil_printf("			Done \n\r\n\r");
}


void OV5640_SetAwb(int awb_mode){

	config_word_t *cfg_;
	int cnt;

    switch (awb_mode)
    {
    	case AWB_DISABLED:
    		cfg_ = (config_word_t*)cfg_disable_awb_;
    		cnt = sizeof(cfg_disable_awb_) / sizeof(cfg_disable_awb_[0]);
    		break;
    	case AWB_SIMPLE:
    		cfg_ = (config_word_t*)cfg_simple_awb_;
    		cnt = sizeof(cfg_simple_awb_) / sizeof(cfg_simple_awb_[0]);
			break;
    	case AWB_ADVANCED:
    		cfg_ = (config_word_t*)cfg_advanced_awb_;
    		cnt = sizeof(cfg_simple_awb_) / sizeof(cfg_simple_awb_[0]);
    		break;
    	default:
    		cfg_ = (config_word_t*)cfg_disable_awb_;
    		cnt = sizeof(cfg_disable_awb_) / sizeof(cfg_disable_awb_[0]);
    }

	xil_printf("Loading awb config...\n\r");

	for (int i = 0; i < cnt; i++){
		OV5640_SetReg(cfg_[i].addr, cfg_[i].data);
		usleep(10000);
	}


	xil_printf("			Done \n\r\n\r");
}
