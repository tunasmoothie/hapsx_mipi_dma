#include "xparameters.h"
#include "xil_printf.h"
#include "sleep.h"
#include "xiic.h"
#include "xil_io.h"

#include "adv7533.h"



int ADV7533_WriteReg(UINTPTR iic_axi_device_addr, u8 iic_map_addr, u8 reg, u8 val){

	// Clear I2C FIFOs & reset
	Xil_Out32((iic_axi_device_addr + 0x100), 0x002);
	// Enable I2C
	Xil_Out32((iic_axi_device_addr + 0x100), 0x001);

	// Start I2C transaction                   Repeated Start bit | Device Address
	Xil_Out32((iic_axi_device_addr + 0x108),         (0x100 | iic_map_addr));
	// Send data byte                           Target Register
	Xil_Out32((iic_axi_device_addr + 0x108), reg);
	// End I2C transaction                       Stop bit | Data to write
	Xil_Out32((iic_axi_device_addr + 0x108), (0x200 | val));
	while ((Xil_In32(iic_axi_device_addr + 0x104) & 0x80) == 0x00) {usleep(1000);}


	usleep(10000);
    return XST_SUCCESS;
}


uint8_t ADV7533_ReadReg(UINTPTR iic_axi_device_addr, u8 iic_map_addr, u8 reg, u8* recv){

	  // Clear I2C FIFOs & reset
	  Xil_Out32((iic_axi_device_addr + 0x100), 0x002);
	  // Enable I2C
	  Xil_Out32((iic_axi_device_addr + 0x100), 0x001);

	  // Start I2C transaction                   Repeated Start bit | Device Address
	  Xil_Out32((iic_axi_device_addr + 0x108),         (0x100 | iic_map_addr));
	  // Send data byte                           Target Register
	  Xil_Out32((iic_axi_device_addr + 0x108), reg);
	  // Next packet                             Repeated Start bit | Device Address
	  Xil_Out32((iic_axi_device_addr + 0x108),         (0x101 | iic_map_addr));
	  // End I2C transaction                       Stop bit | Bytes to read
	  Xil_Out32((iic_axi_device_addr + 0x108), (0x200 | 0x01));
	  // Check & wait for RX FIFO to be filled
	  while ((Xil_In32(iic_axi_device_addr + 0x104) & 0x40) == 0x40) {usleep(1000);}
	  *recv = Xil_In32(iic_axi_device_addr + 0x10C) & 0x00ff;

	  xil_printf("iic_read: addr(%02x) data(%02x)\n\r", reg, *recv);


	  usleep(10000);
	  return XST_SUCCESS;
}

int ADV7533_Whoami(UINTPTR iic_axi_device_addr){

	u8 id_h, id_l;

	xil_printf("Reading ADV7533 Chip ID \n\r");
	xil_printf("Registers @  H:0x01 L:0x00\n\r");

	ADV7533_ReadReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0x00, &id_h);
	ADV7533_ReadReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0x01, &id_l);

	xil_printf("			Chip ID = %x%x \r\n", id_h, id_l);
    if(id_h != 0x75 || id_l != 0x33) {
    	xil_printf("ERROR: ADV7533 chip ID missing or incorrect\r\n");
    	return XST_FAILURE;
    }
    else {
    	xil_printf("ADV7533 chip ID read success\r\n");
    }


    return XST_SUCCESS;
}

int ADV7533_PowerOn(UINTPTR iic_axi_device_addr){
	u8 tmp;
	ADV7533_ReadReg(iic_axi_device_addr, ADV7533_MAIN_I2C_ADDR, 0x41, &tmp);
	tmp &= ~0x40;
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_MAIN_I2C_ADDR, 0x41, tmp);

	return XST_SUCCESS;
}

int ADV7533_PowerOff(UINTPTR iic_axi_device_addr){
	u8 tmp;
	ADV7533_ReadReg(iic_axi_device_addr, ADV7533_MAIN_I2C_ADDR, 0x41, &tmp);
	tmp |= 0x40;
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_MAIN_I2C_ADDR, 0x41, tmp);

	return XST_SUCCESS;
}

int ADV7533_Init(UINTPTR iic_axi_device_addr){

	u8 tmp;

	//ADV7533_PowerOff(iic_axi_device_addr);

	ADV7533_ReadReg(iic_axi_device_addr, ADV7533_MAIN_I2C_ADDR, 0x16, &tmp);
		tmp &= ~0x3E;
		tmp |= 0x20;
		ADV7533_WriteReg(iic_axi_device_addr, ADV7533_MAIN_I2C_ADDR, 0x16, tmp);

	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_MAIN_I2C_ADDR, 0x9A, 0xE0);

	ADV7533_ReadReg(iic_axi_device_addr, ADV7533_MAIN_I2C_ADDR, 0xBA, &tmp);
		tmp &= ~0xF8;
		tmp |= 0x70;
		ADV7533_WriteReg(iic_axi_device_addr, ADV7533_MAIN_I2C_ADDR, 0xBA, tmp);

	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_MAIN_I2C_ADDR, 0xDE, 0x82);

	ADV7533_ReadReg(iic_axi_device_addr, ADV7533_MAIN_I2C_ADDR, 0xE4, &tmp);
		tmp |= 0x40;
		ADV7533_WriteReg(iic_axi_device_addr, ADV7533_MAIN_I2C_ADDR, 0xE4, tmp);

	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_MAIN_I2C_ADDR, 0xE5, 0x80);


	ADV7533_ReadReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0x15, &tmp);
		tmp &= ~0x30;
		tmp |= 0x10;
		ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0x15, tmp);

	ADV7533_ReadReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0x17, &tmp);
		tmp &= ~0xF0;
		tmp |= 0xD0;
		ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0x17, tmp);

	ADV7533_ReadReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0x24, &tmp);
		tmp &= ~0x10;
		ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0x24, tmp);

	ADV7533_ReadReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0x57, &tmp);
	    tmp |= 0x11;
		ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0x57, tmp);


	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0x1C, 0x20);

	return XST_SUCCESS;
}

int ADV7533_ConfigVideoOutput(UINTPTR iic_axi_device_addr){
	u8 tmp;

	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_CEC_DSI_I2C_ADDR, 0x28);




	ADV7533_ReadReg(iic_axi_device_addr, ADV7533_MAIN_I2C_ADDR, 0xAF, &tmp);
	    tmp |= 0x02;
		ADV7533_WriteReg(iic_axi_device_addr, ADV7533_MAIN_I2C_ADDR, 0xAF, tmp);

	ADV7533_ReadReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0x03, &tmp);
	    tmp |= 0x80;
		ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0x03, tmp);

	ADV7533_ReadReg(iic_axi_device_addr, ADV7533_MAIN_I2C_ADDR, 0x40, &tmp);
		   tmp |= 0x80;
		ADV7533_WriteReg(iic_axi_device_addr, ADV7533_MAIN_I2C_ADDR, 0x40, tmp);

	ADV7533_ReadReg(iic_axi_device_addr, ADV7533_MAIN_I2C_ADDR, 0x4C, &tmp);
		tmp &= ~0x0F;
		tmp |= 0x04;
		ADV7533_WriteReg(iic_axi_device_addr, ADV7533_MAIN_I2C_ADDR, 0x4C, tmp);

	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_MAIN_I2C_ADDR, 0x49, 0xFC);

	return XST_SUCCESS;
}


int ADV7533_EnableCEC(UINTPTR iic_axi_device_addr){
	u8 tmp;


	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0x05, 0xC8);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xBE, 0x01);


	/* TG programming for 19.2MHz, divider 25 */
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xBE, 0x61);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xC1, 0x0D);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xC2, 0x80);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xC3, 0x0C);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xC4, 0x9A);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xC5, 0x0E);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xC6, 0x66);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xC7, 0x0B);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xC8, 0x1A);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xC9, 0x0A);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xCA, 0x33);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xCB, 0x0C);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xCC, 0x00);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xCD, 0x07);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xCE, 0x33);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xCF, 0x05);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xD0, 0xDA);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xD1, 0x08);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xD2, 0x8D);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xD3, 0x01);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xD4, 0xCD);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xD5, 0x04);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xD6, 0x80);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xD7, 0x05);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xD8, 0x66);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xD9, 0x03);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xDA, 0x26);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xDB, 0x0A);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xDC, 0xCD);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xDE, 0x00);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xDF, 0xC0);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xE1, 0x00);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xE2, 0xE6);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xE3, 0x02);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xE4, 0xB3);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xE5, 0x03);
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xE6, 0x9A);


	/* cec */
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_MAIN_I2C_ADDR, 0xE2, 0x00);
	/* hpd override */
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_MAIN_I2C_ADDR, 0xD6, 0x48);
	/* edid reread */
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_MAIN_I2C_ADDR, 0xC9, 0x13);
	/* read all CEC Rx Buffers */
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xBA, 0x08);
	/* logical address0 0x04 */
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xBC, 0x04);
	/* select logical address0 */
	ADV7533_WriteReg(iic_axi_device_addr, ADV7533_DSI_CEC_I2C_ADDR, 0xBB, 0x10);

	return XST_SUCCESS;
}
