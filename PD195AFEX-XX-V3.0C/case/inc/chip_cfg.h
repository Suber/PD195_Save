#ifndef CHIP_CFG_H
#define CHIP_CFG_H

#define CHIP_VOLUME_MAX 40

#define LITHIUM_LOW  0x54
#define 	 SDCMD_GPIO_REG			0xaf
#define      SDCMD_GPIO_MASK		0xfd
#define 	 SDCLK_GPIO_REG			0xaf
#define      SDCLK_GPIO_MASK		0xf7
   
#define 	 SDDET_GPIO_OUTPUT_EN	0xaf 
#define 	 SDDET_GPIO_INPUT_EN	0xb0 
#define 	 SDDET_GPIO_INPUT_DAT	0xb1  
#define      CARD_CD_SET			0x10	
#define      CARD_CD_CLR			0xef	  

#define      SDVCC_GIO_EN_MFP_REG   0xf3 
#define      SDVCC_GIO_EN_MFP_BIT   0xbf
#define      SDVCC_GIO_EN_REG       0xb5   //GPIO_C2 (Output) 
#define      SDVCC_GIO_INTPUT_REG   0xb6   //GPIO_C2 (Output)
#define      SDVCC_GIO_DATA_REG     0xb7
#define      SDVCC_SET_BIT          0x20
#define      SDVCC_CLR_BIT          0xDF  

#define      GPIOF_OUTPUT_EN		0xB2
#define      GPIOF_OUTPUT_8LINE_CLR 0xF0
#define      GPIOF_OUTPUT_8LINE_SET 0x0F

#define      GPIOF_DATA_EN			0xB4
#define      GPIOF_DATA_SET         0xF0
#define      GPIOF_DATA_CLR         0x0F

#define     DETECT_INPUT_EN         0xB6 
#define     DETECT_IN_SET			0x20 
#define     DETECT_IN_CLR			0xDF 
#define     DETECT_DATA				0xB7 
#define     DETECT_DATA_SET			0x20 

///////////////////////PLL SELECT/////////////////
#define PLL_6MHZ        0x80
#define PLL_12MHZ		0x40
#define PLL_24MHZ		0x00
#define PLL_28MHZ		0x0e
#define PLL_32MHZ		0x10
#define PLL_36MHZ		0x12
#define PLL_40MHZ		0x14
#define PLL_44MHZ	    0x16
#define PLL_48MHZ	    0x18
#define PLL_52MHZ		0x1a
#define PLL_56MHZ		0x1c
#define PLL_60MHZ		0x1e
#define PLL_64MHZ	    0x20
#define PLL_68MHZ	    0x22
#define PLL_72MHZ		0x24
#define PLL_76MHZ		0x26
#define PLL_80MHZ	    0x28
#define PLL_84MHZ	    0x2a
#define PLL_88MHZ	    0x2c
#define PLL_92MHZ		0x2e
#define PLL_96MHZ		0x30
#define PLL_104MHZ	    0x32
#define PLL_108MHZ	    0x34
#define PLL_112MHZ		0x36
#define PLL_116MHZ		0x38
#define PLL_120MHZ	    0x3a
#define PLL_124MHZ	    0x3e
#define PLL_ENABLE		0x01
#define VDDSET_1_3V 0x0
#define VDDSET_1_4V 0x2
#define VDDSET_1_5V 0x4
#define VDDSET_1_6V 0x6
#define VDDSET_1_7V 0x8

#endif