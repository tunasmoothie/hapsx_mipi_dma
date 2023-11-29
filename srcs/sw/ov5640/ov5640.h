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


/***************************** Include Files *********************************/

#include <stdio.h>


/************************** Constant Definitions *****************************/

#define MODE_720P_1280_720_60fps   0
#define MODE_1080P_1920_1080_15fps 1
#define MODE_1080P_1920_1080_30fps 2
#define MODE_1080P_1920_1080_30fps_336M_MIPI 3

#define AWB_DISABLED   0
#define AWB_SIMPLE     1
#define AWB_ADVANCED   2

/******************************************************************************/

typedef struct {
	uint16_t addr;
	uint8_t data;
} config_word_t;

/************************* Function Definitions *******************************/

/*
 * IIC register write to OV5640. (2 byte address, 1 byte data)
 * @param     addr is 2-byte IIC address to write to.
 * @param     val  is 1-byte value to write.
 */
int OV5640_SetReg(uint16_t addr, uint8_t val);


/*
 * IIC register read from OV5640.
 * @param     addr is 2-byte IIC address to read from.
 */
uint8_t OV5640_GetReg(uint16_t addr);


/*
 * Read registers 0x300A and 0x300B to check for OV5640 IIC Identifier.
 */
void OV5640_Whoami();


/*
 * Writes a reset to OV5640 system control register.
 * !! Will halt OV5640 until enable is issued.
 */
void OV5640_Reset();


/*
 * Writes an enable to OV5640 system control register.
 */
void OV5640_Enable();


/*
 * Loads initialization register words to OV5640.
 * Register words defined in ov5640_cfg_words.h
 */
void OV5640_Init();


/*
 * Loads video mode register words to OV5640.
 * Register words defined in ov5640_cfg_words.h
 * Default mode is MODE_1080P_1920_1080_30fps
 */
void OV5640_SetVidMode(int vid_mode);


/*
 * Loads awb mode register words to OV5640.
 * Register words defined in ov5640_cfg_words.h
 * Defualt mode is AWB_DISABLED
 */
void OV5640_SetAwb(int awb);


#endif
