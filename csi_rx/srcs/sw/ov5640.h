#ifndef OV5640_H_
#define OV5640_H_

#include <stdio.h>

//--------------------------- Typedefs -------------------------------

typedef struct {
	uint16_t addr;
	uint8_t data;
} config_word_t;


enum vid_mode_t {
	MODE_720P_1280_720_60fps = 0,
	MODE_1080P_1920_1080_15fps,
	MODE_1080P_1920_1080_30fps,
	MODE_1080P_1920_1080_30fps_336M_MIPI,
	MODE_END
};

enum awb_t {
	AWB_DISABLED = 0,
	AWB_SIMPLE,
	AWB_ADVANCED,
	AWB_END
};


//extern config_word_t const cfg_init_[];
//extern config_word_t const cfg_720p_60fps_[];
//extern config_word_t const cfg_1080p_15fps_[];
//extern config_word_t const cfg_1080p_30fps_[];
//extern config_word_t const cfg_1080p_30fps_336M_mipi_[];
//extern config_word_t const cfg_advanced_awb_[];
//extern config_word_t const cfg_simple_awb_[];
//extern config_word_t const cfg_disable_awb_[];

//------------------------- Function Defs ----------------------------

int OV5640_set_reg(uint16_t addr, uint8_t val);
uint8_t OV5640_get_reg(uint16_t addr);

void OV5640_init();
void OV5640_reset();
void OV5640_enable();
void OV5640_whoami();
void OV5640_set_vid_mode(int vid_mode);
void OV5640_set_awb(int awb);


#endif

