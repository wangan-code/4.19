/*!
    \file  main.c
    \brief GPIO keyboard interrupt demo
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-06-23, V1.0.0, demo for GD32F30x
*/

#include <stdio.h>
#include "gd32f30x.h"
#include "systick.h"
#include "gd25qxx.h"
#include "gd32f303c_eval.h"

#define BUFFER_SIZE                    256
#define TX_BUFFER_SIZE                 BUFFER_SIZE
#define RX_BUFFER_SIZE                 BUFFER_SIZE

#define  VERSION_ID                    "1.0.0"
#define  SFLASH_ID                     0xC84015
#define  FLASH_WRITE_ADDRESS           0x000000
#define  FLASH_READ_ADDRESS            FLASH_WRITE_ADDRESS

uint32_t int_device_serial[3];
uint8_t  count;
uint8_t  tx_buffer[TX_BUFFER_SIZE];
uint8_t  rx_buffer[TX_BUFFER_SIZE];
uint32_t flash_id = 0;
uint16_t i = 0;
uint8_t  is_successful = 0;

static void get_chip_serial_num(void);
void test_status_led_init(void);
static void turn_on_led(uint8_t led_num);
ErrStatus memory_compare(uint8_t* src,uint8_t* dst,uint16_t length);
int fputc(int ch,FILE *f);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* systick configuration*/
    systick_config();

    /* configure the led GPIO */
    test_status_led_init();

    /* USART parameter configuration */
    gd_eval_com_init(EVAL_COM1);

    /* configure SPI0 GPIO and parameter */
    spi_flash_init();

    /* GD32303C-EVAL-V1.0 Start up */
    printf("\n\r###############################################################################\n\r");
    printf("\n\rGD32303C-EVAL-V1.0 System is Starting up...\n\r");
    printf("\n\rGD32303C-EVAL-V1.0 Flash:%dK\n\r",*(__IO uint16_t*)(0x1FFFF7E0));
  
    /* get chip serial number */
    get_chip_serial_num();
  
    /* printf CPU unique device id */
    printf("\n\rGD32303C-EVAL-V1.0 The CPU Unique Device ID:[%X-%X-%X]\n\r",int_device_serial[2],int_device_serial[1],int_device_serial[0]);                       

    printf("\n\rGD32303C-EVAL-V1.0 SPI Flash:GD25Q16 configured...\n\r");

    /* get flash id */
    flash_id = spi_flash_read_id();
    printf("\n\rThe Flash_ID:0x%X\n\r",flash_id);

    /* flash id is correct */
    if(SFLASH_ID == flash_id){
        printf("\n\r\n\rWrite to tx_buffer:\n\r\n\r");

        /* printf tx_buffer value */
        for(i = 0; i < BUFFER_SIZE; i ++){
            tx_buffer[i] = i;
            printf("0x%02X ",tx_buffer[i]);

            if(15 == i%16)
                printf("\n\r");
        }

        printf("\n\r\n\rRead from rx_buffer:\n\r\n\r");

        /* erases the specified flash sector */
        spi_flash_sector_erase(FLASH_WRITE_ADDRESS);
        /* write tx_buffer data to the flash */ 
        spi_flash_buffer_write(tx_buffer,FLASH_WRITE_ADDRESS,TX_BUFFER_SIZE);

        delay_1ms(10);

        /* read a block of data from the flash to rx_buffer */
        spi_flash_buffer_read(rx_buffer,FLASH_READ_ADDRESS,RX_BUFFER_SIZE);
        /* printf rx_buffer value */
        for(i = 0; i <= 255; i ++){
            printf("0x%02X ", rx_buffer[i]);
            if(15 == i%16)
                printf("\n\r");
        }

        if(ERROR == memory_compare(tx_buffer,rx_buffer,256)){
            printf("Err:Data Read and Write aren't Matching.\n\r");
            is_successful = 1;
        }

        /* spi flash test passed */
        if(0 == is_successful){
            printf("\n\rSPI-GD25Q16 Test Passed!\n\r");
        }
    }else{
        /* spi flash read id fail */
        printf("\n\rSPI Flash: Read ID Fail!\n\r");
    }

    while(1){
        /* turn off all leds */
        gd_eval_led_off(LED2);
        gd_eval_led_off(LED3);
        gd_eval_led_off(LED4);
        gd_eval_led_off(LED5);

        /* turn on a led */
        turn_on_led(count % 4);
        count ++;
        if(4 <= count)
           count = 0;

        delay_1ms(500);
    }
}

/*!
    \brief      get chip serial number
    \param[in]  none
    \param[out] none
    \retval     none
*/
void get_chip_serial_num(void)
{
    int_device_serial[0] = *(__IO uint32_t*)(0x1FFFF7E0);
    int_device_serial[1] = *(__IO uint32_t*)(0x1FFFF7EC);
    int_device_serial[2] = *(__IO uint32_t*)(0x1FFFF7F0);
}

/*!
    \brief      test status led initialize
    \param[in]  none
    \param[out] none
    \retval     none
*/
void test_status_led_init(void)
{
    /* initialize the leds */
    gd_eval_led_init(LED2);
    gd_eval_led_init(LED3);
    gd_eval_led_init(LED4);
    gd_eval_led_init(LED5);

    /* close all of leds */
    gd_eval_led_off(LED2);
    gd_eval_led_off(LED3);
    gd_eval_led_off(LED4);
    gd_eval_led_off(LED5);
}

/*!
    \brief      turn on led
    \param[in]  led_num: led number
    \param[out] none
    \retval     none
*/
static void turn_on_led(uint8_t led_num)
{
    switch(led_num){
    case 0:
        /* turn on LED2 */
        gd_eval_led_on(LED2);
        break;
    case 1:
        /* turn on LED3 */
        gd_eval_led_on(LED3);
        break;
    case 2:
        /* turn on LED4 */
        gd_eval_led_on(LED4);
        break;
    case 3:
        /* turn on LED5 */
        gd_eval_led_on(LED5);
        break;
    default:
        /* turn on all leds */
        gd_eval_led_on(LED2);
        gd_eval_led_on(LED3);
        gd_eval_led_on(LED4);
        gd_eval_led_on(LED5); 
        break;
    }
}

/*!
    \brief      memory compare function
    \param[in]  src: source data pointer
    \param[in]  dst: destination data pointer
    \param[in]  length: the compare data length
    \param[out] none
    \retval     ErrStatus: ERROR or SUCCESS
*/
ErrStatus memory_compare(uint8_t* src, uint8_t* dst, uint16_t length) 
{
    while(length --){
        if(*src++ != *dst++)
            return ERROR;
    }
    return SUCCESS;
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM1,(uint8_t)ch);
    while (RESET == usart_flag_get(EVAL_COM1, USART_FLAG_TC));
    return ch;
}

