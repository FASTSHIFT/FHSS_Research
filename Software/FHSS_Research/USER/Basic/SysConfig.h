#ifndef __SYSCONFIG_H
#define __SYSCONFIG_H

#define SYS_NAME              "FHSS_Research"
#define SYS_HARDWARE_VERSION  "v1.0"
#define SYS_SOFTWARE_VERSION  "v1.1"

/*¿ÕÒý½Å*/
#define NULL_PIN            PD0

/*ÆÁÄ»*/
#define SCREEN_SCLK_Pin     PA5
#define SCREEN_MOSI_Pin     PA7
#define SCREEN_BLK_Pin      PA6
#define SCREEN_DC_Pin       PA3
#define SCREEN_RES_Pin      PA4
#define SCREEN_CS_Pin       NULL_PIN

/*°´¼ü*/
#define KEY_Pin             PB9

/*NRF24L01-1*/
#define NRF1_MOSI_Pin       PA0
#define NRF1_MISO_Pin       PC13
#define NRF1_SCK_Pin        PC15
#define NRF1_CE_Pin         PA1
#define NRF1_CSN_Pin        PA2
#define NRF1_IRQ_Pin        PC14

/*NRF24L01-2*/
#define NRF2_MOSI_Pin       PB2
#define NRF2_MISO_Pin       PB1
#define NRF2_SCK_Pin        PB10
#define NRF2_CE_Pin         PB12
#define NRF2_CSN_Pin        PB11
#define NRF2_IRQ_Pin        PB0

/*LED*/
#define LED_TX_Pin          PA8
#define LED_RX_Pin          PB15
#define LED_STA1_Pin        PB14
#define LED_STA2_Pin        PB13

#endif
