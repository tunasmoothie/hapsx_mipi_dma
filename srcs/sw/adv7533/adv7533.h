#ifndef ADV7533_H_
#define ADV7533_H_		/**< Prevent circular inclusions
				  *  by using protection macros */

/***************************** Include Files ********************************/

#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"

/************************** Constant Definitions ****************************/

#define  ADV7533_MAIN_I2C_ADDR           0x7A
#define  ADV7533_EDID_I2C_ADDR           0x7E
#define  ADV7533_DSI_CEC_I2C_ADDR        0x78

/**************************** Type Definitions ******************************/





/************************** Function Prototypes *****************************/


/*
 * IIC register write to ADV7533. (2 byte address, 1 byte data)
 * @param     iic_axi_device_addr   is the AXI address of I2C device that carries out the operation.
 * @param     addr                  is 2-byte I2C address to write to.
 * @param     val                   is 1-byte value to write.
 */
int ADV7533_WriteReg(UINTPTR iic_axi_device_addr, u8 iic_map_addr, u8 reg, uint8_t val);


/*
 * IIC register read from ADV7533.
 * @param     iic_axi_device_addr   is the AXI address of I2C device that carries out the operation.
 * @param     addr                  is 2-byte I2C address to read from.
 * @param     recv                  is a pointer to the container for received data.
 */
uint8_t ADV7533_ReadReg(UINTPTR iic_axi_device_addr, u8 iic_map_addr, u8 reg, uint8_t* recv);

/*
 * Read registers 0x300A and 0x300B to check for ADV7533 I2C Identifier.
 * @param     iic_axi_device_addr   is the AXI address of I2C device that carries out the operation.
 */
int ADV7533_Whoami(UINTPTR iic_axi_device_addr);

int ADV7533_PowerOn(UINTPTR iic_axi_device_addr);
int ADV7533_PowerOff(UINTPTR iic_axi_device_addr);

int ADV7533_Init(UINTPTR iic_axi_device_addr);
int ADV7533_ConfigVideoOutput(UINTPTR iic_axi_device_addr);
int ADV7533_EnableCEC(UINTPTR iic_axi_device_addr);






///* Includes ------------------------------------------------------------------*/
////#include "../Common/audio.h"
//
///** @addtogroup BSP
//  * @{
//  */
//
///** @addtogroup Components
//  * @{
//  */
//
///** @addtogroup adv7533
//  * @{
//  */
//
///** @addtogroup ADV7533_Exported_Types
//  * @{
//  */
///*
//#define unsigned char unsigned char
//#define short int short int
//#define unsigned int unsigned int
//#define  u32 unsigned int
//*/
//
//typedef struct {
//  unsigned char       DSI_LANES;
//  short int      HACT;
//  short int      HSYNC;
//  short int      HBP;
//  short int      HFP;
//  short int      VACT;
//  short int      VSYNC;
//  short int      VBP;
//  short int      VFP;
//  unsigned char       ASPECT_RATIO;
//} adv7533ConfigTypeDef;
//
///** @defgroup ADV7533_Exported_Constants
//  * @{
//  */
//
///**
//  * @brief HDMI audio output DEVICE
//  */
//#define OUTPUT_DEVICE_ADV7533_HDMI       ((short int)0x1000)
//
///**
//  * @brief  ADV7533 I2C Addresses 0x7A / 0x78
//  */
//#define  ADV7533_MAIN_I2C_ADDR           ((unsigned char)0x7A>>1)
//#define  ADV7533_CEC_DSI_I2C_ADDR        ((unsigned char)0x78>>1)
//
///**
//  * @brief  ADV7533 Aspect ratio
//  */
//#define ADV7533_ASPECT_RATIO_16_9        ((unsigned char)0x00)
//#define ADV7533_ASPECT_RATIO_4_3         ((unsigned char)0x01)
//
///**
//  * @brief  ADV7533 Aspect ratio
//  */
//#define ADV7533_MODE_HDMI                0x0
//#define ADV7533_MODE_DVI                 0x1
//
///**
//  * @brief  ADV7533 Main Registers
//  */
//#define  ADV7533_MAIN_SYNC_REG           ((unsigned char)0x17)
//#define  ADV7533_MAIN_POWER_DOWN_REG     ((unsigned char)0x41)
//#define  ADV7533_MAIN_HPG_REG            ((unsigned char)0x42)
//
///**
// * @brief   ADV7533 Main Features Parameters
// */
//
///**
//  * @brief ADV7533 CEC DSI Registers
//  */
//#define ADV7533_CEC_DSI_INTERNAL_TIMING_REG  ((unsigned char)0x27)
//#define ADV7533_CEC_DSI_TOTAL_WIDTH_H_REG    ((unsigned char)0x28)
//#define ADV7533_CEC_DSI_TOTAL_WIDTH_L_REG    ((unsigned char)0x29)
//#define ADV7533_CEC_DSI_HSYNC_H_REG          ((unsigned char)0x2A)
//#define ADV7533_CEC_DSI_HSYNC_L_REG          ((unsigned char)0x2B)
//#define ADV7533_CEC_DSI_HFP_H_REG            ((unsigned char)0x2C)
//#define ADV7533_CEC_DSI_HFP_L_REG            ((unsigned char)0x2D)
//#define ADV7533_CEC_DSI_HBP_H_REG            ((unsigned char)0x2E)
//#define ADV7533_CEC_DSI_HBP_L_REG            ((unsigned char)0x2F)
//
//#define ADV7533_CEC_DSI_TOTAL_HEIGHT_H_REG   ((unsigned char)0x30)
//#define ADV7533_CEC_DSI_TOTAL_HEIGHT_L_REG   ((unsigned char)0x31)
//#define ADV7533_CEC_DSI_VSYNC_H_REG          ((unsigned char)0x32)
//#define ADV7533_CEC_DSI_VSYNC_L_REG          ((unsigned char)0x33)
//#define ADV7533_CEC_DSI_VFP_H_REG            ((unsigned char)0x34)
//#define ADV7533_CEC_DSI_VFP_L_REG            ((unsigned char)0x35)
//#define ADV7533_CEC_DSI_VBP_H_REG            ((unsigned char)0x36)
//#define ADV7533_CEC_DSI_VBP_L_REG            ((unsigned char)0x37)
//
///** @Brief adv7533 ID
//  */
//#define ADV7533_ID  0x7533
//
///** @Brief device ID register
//  */
//#define   ADV7533_CHIPID_ADDR0   0x00
//#define   ADV7533_CHIPID_ADDR1   0x01
//
///* MUTE commands */
//#define AUDIO_MUTE_ON                 1
//#define AUDIO_MUTE_OFF                0
//
///* AUDIO FREQUENCY */
//#define AUDIO_FREQUENCY_192K           ((unsigned int)192000)
//#define AUDIO_FREQUENCY_176K           ((unsigned int)176400)
//#define AUDIO_FREQUENCY_96K            ((unsigned int)96000)
//#define AUDIO_FREQUENCY_88K            ((unsigned int)88200)
//#define AUDIO_FREQUENCY_48K            ((unsigned int)48000)
//#define AUDIO_FREQUENCY_44K            ((unsigned int)44100)
//#define AUDIO_FREQUENCY_32K            ((unsigned int)32000)
//
///**
//  * @}
//  */
//
///* Exported macro ------------------------------------------------------------*/
//
///** @defgroup ADV7533_Exported_Macros ADV7533 Exported Macros
//  * @{
//  */
//
///**
//  * @}
//  */
//
///* Exported functions --------------------------------------------------------*/
//
///** @addtogroup ADV7533_Exported_Functions
//  * @{
//  */
//
///*------------------------------------------------------------------------------
//                           HDMI video functions
//------------------------------------------------------------------------------*/
//unsigned char ADV7533_Init(void);
//void ADV7533_PowerOn(void);
//void ADV7533_PowerDown(void);
//void ADV7533_Configure();//adv7533ConfigTypeDef * config);
//void ADV7533_PatternEnable(void);
//void ADV7533_PatternDisable(void);
//
///*------------------------------------------------------------------------------
//                           HDMI Audio functions
//------------------------------------------------------------------------------*/
//unsigned int adv7533_AudioInit(short int DeviceAddr, short int OutputDevice, unsigned char Volume,unsigned int AudioFreq);
//void adv7533_DeInit(void);
//unsigned int adv7533_ReadID(short int DeviceAddr);
//unsigned int adv7533_Play(short int DeviceAddr, short int* pBuffer, short int Size);
//unsigned int adv7533_Stop(short int DeviceAddr,unsigned int Cmd);
//unsigned int adv7533_Pause(short int DeviceAddr);
//unsigned int adv7533_Resume(short int DeviceAddr);
//unsigned int adv7533_SetMute(short int DeviceAddr, unsigned int Cmd);
//unsigned int adv7533_SetVolume(short int, unsigned char);
//unsigned int adv7533_Reset(short int DeviceAddr);
//unsigned int adv7533_SetOutputMode(short int DeviceAddr, unsigned char Output);
//unsigned int adv7533_SetFrequency(short int DeviceAddr, unsigned int AudioFreq);
//
//void adv7533_init_setup(void);
//void adv7533_video_on(void);
//void adv7533_video_setup(void);
//void adv7533_cec_enable(void);
//
///* HDMI IO functions */
//void     HDMI_IO_Init(void);
//void     HDMI_IO_Write(unsigned char addr, unsigned char reg, unsigned char value);
//unsigned char HDMI_IO_Read(unsigned char addr, unsigned char reg);
//void     HDMI_IO_Delay(unsigned int delay);
//void     AUDIO_IO_DeInit(void);
//
///**
//  * @}
//  */
//
///* HDMI Audio driver structure */
////extern AUDIO_DrvTypeDef adv7533_drv;
//
//#endif /* __ADV7533_H */
///**
//  * @}
//  */
//
///**
//  * @}
//  */
//
///**
//  * @}
//  */
//
///************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


#endif
