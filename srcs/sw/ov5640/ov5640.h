/*
*
* * <b> Initialization & Usage </b>
*
* The OV5640 requires the following sequence to being transmitting
* valid data:
*
* - Call OV5640_Reset to soft reset OV5640, this will put it on hold.
* - Call OV5640_Init to initialize the device.
* - Call OV5640_SetVidMode with the desired video format.
* - Call OV5640_SetAwb with the desired AWB mode.
* - Call OV5640_Enable to release reset hold and enable the device.
*
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver Who Date     Changes
* --- --- -------- ------------------------------------------------------------
* 1.0 eks 11/28/23 Initial release
*
* </pre>
*
******************************************************************************/

#ifndef OV5640_H_
#define OV5640_H_		/**< Prevent circular inclusions
				  *  by using protection macros */


/***************************** Include Files ********************************/

#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"

/************************** Constant Definitions ****************************/

#define OV5640_I2C_BASEADDR   0x3CU

/**************************** Type Definitions ******************************/

typedef enum {
	MODE_720P_1280_720_60fps,
	MODE_1080P_1920_1080_15fps,
	MODE_1080P_1920_1080_30fps,
	MODE_1080P_1920_1080_30fps_336M_MIPI
} OV5640_vid_mode;

typedef enum {
	AWB_DISABLED,
	AWB_SIMPLE,
	AWB_ADVANCED
}OV5640_awb_mode;


typedef struct {
	uint16_t addr;
	uint8_t data;
} config_word_t;


/************************** Function Prototypes *****************************/

/*
 * IIC register write to OV5640. (2 byte address, 1 byte data)
 * @param     iic_axi_device_addr   is the AXI address of I2C device that carries out the operation.
 * @param     addr                  is 2-byte IIC address to write to.
 * @param     val                   is 1-byte value to write.
 */
int OV5640_WriteReg(UINTPTR iic_axi_device_addr, UINTPTR addr, uint8_t val);


/*
 * IIC register read from OV5640.
 * @param     iic_axi_device_addr   is the AXI address of I2C device that carries out the operation.
 * @param     addr                  is 2-byte IIC address to read from.
 * @param     recv                  is a pointer to the container for received data.
 */
uint8_t OV5640_ReadReg(UINTPTR iic_axi_device_addr, UINTPTR addr, uint8_t* recv);


/*
 * Read registers 0x300A and 0x300B to check for OV5640 IIC Identifier.
 * @param     iic_axi_device_addr   is the AXI address of I2C device that carries out the operation.
 */
int OV5640_Whoami(UINTPTR iic_axi_device_addr);


/*
 * Writes a reset to OV5640 system control register.
 * !! Will halt OV5640 until enable is issued.
 * @param     iic_axi_device_addr   is the AXI address of I2C device that carries out the operation.
 */
void OV5640_Reset(UINTPTR iic_axi_device_addr);


/*
 * Writes an enable to OV5640 system control register.
 * @param     iic_axi_device_addr   is the AXI address of I2C device that carries out the operation.
 */
void OV5640_Enable(UINTPTR iic_axi_device_addr);


/*
 * Loads initialization register words to OV5640.
 * Register words defined in ov5640_cfg_words.h
 * @param     iic_axi_device_addr   is the AXI address of I2C device that carries out the operation.
 */
int OV5640_Init(UINTPTR iic_axi_device_addr);


/*
 * Loads video mode register words to OV5640.
 * Register words defined in ov5640_cfg_words.h
 * Default mode is MODE_1080P_1920_1080_30fps
 * @param     iic_axi_device_addr   is the AXI address of I2C device that carries out the operation.
 * @param     vid_mode              is the desired video mode.
 */
void OV5640_SetVidMode(UINTPTR iic_axi_device_addr, OV5640_vid_mode vid_mode);


/*
 * Loads awb mode register words to OV5640.
 * Register words defined in ov5640_cfg_words.h
 * Defualt mode is AWB_DISABLED
 * @param     iic_axi_device_addr   is the AXI address of I2C device that carries out the operation.
 * @param     vid_mode              is the desired AWB mode.
 */
void OV5640_SetAwb(UINTPTR iic_axi_device_addr, OV5640_awb_mode awb);


#endif

