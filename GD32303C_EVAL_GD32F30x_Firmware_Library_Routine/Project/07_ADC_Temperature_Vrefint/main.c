/*!
    \file    main.c
    \brief   ADC_software_trigger_regular_channel_polling

    \version 2017-02-10, V1.0.0, firmware for GD32F30x
    \version 2018-10-10, V1.1.0, firmware for GD32F30x
    \version 2018-12-25, V2.0.0, firmware for GD32F30x
*/

/*
    Copyright (c) 2018, GigaDevice Semiconductor Inc.

*/

#include "gd32f30x.h"
#include "systick.h"
#include <stdio.h>
#include "gd32f303c_eval.h"

float adc_value[16];
float vref_value ;
void rcu_config(void);
void gpio_config(void);
void adc_config(void);
uint16_t adc_channel_sample(uint8_t channel);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/

int main(void)
{
	  uint16_t  i;
    /* system clocks configuration */
    rcu_config();
    /* systick configuration */
    systick_config();  
    /* GPIO configuration */
    gpio_config();
    /* ADC configuration */
    adc_config();
    gd_eval_com_init(EVAL_COM1);
    while(1){
			  adc_value[0]=adc_channel_sample(ADC_CHANNEL_0);
        adc_value[1]=adc_channel_sample(ADC_CHANNEL_1);
        adc_value[2]=adc_channel_sample(ADC_CHANNEL_2);
        adc_value[3]=adc_channel_sample(ADC_CHANNEL_3);
        adc_value[4]=adc_channel_sample(ADC_CHANNEL_4);
			  adc_value[5]=adc_channel_sample(ADC_CHANNEL_5);
			  adc_value[6]=adc_channel_sample(ADC_CHANNEL_6);
			  adc_value[7]=adc_channel_sample(ADC_CHANNEL_7);
			  adc_value[8]=adc_channel_sample(ADC_CHANNEL_8);
			  adc_value[9]=adc_channel_sample(ADC_CHANNEL_9);
			  adc_value[10]=adc_channel_sample(ADC_CHANNEL_10);
			  adc_value[11]=adc_channel_sample(ADC_CHANNEL_11);
			  adc_value[12]=adc_channel_sample(ADC_CHANNEL_12);
			  adc_value[13]=adc_channel_sample(ADC_CHANNEL_13);
			  adc_value[14]=adc_channel_sample(ADC_CHANNEL_14);
			  adc_value[15]=adc_channel_sample(ADC_CHANNEL_15);
			  for ( i =0;i<16;i++) {
				   vref_value=adc_value[i]*3.3/4096;
				  printf("第%d个通道的值是%5.3f\r\n",i,vref_value);
				
				}
    }
}

/*!
    \brief      configure the different system clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    /* enable GPIOA clock */
    rcu_periph_clock_enable(RCU_GPIOA);
	  rcu_periph_clock_enable(RCU_GPIOB);
	  rcu_periph_clock_enable(RCU_GPIOC);
    /* enable ADC clock */
    rcu_periph_clock_enable(RCU_ADC0);
    /* config ADC clock */
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6);
}

/*!
    \brief      configure the GPIO peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* config the GPIO as analog mode */
	  gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_0);
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_1);
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_2);
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_3);
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_4);
	  gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_5);
	  gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_6);
	  gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_7);
	  gpio_init(GPIOB, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_0);
	  gpio_init(GPIOB, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_1);
	  gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_0);
	  gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_1);
	  gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_2);
    gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_3);
	  gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_4);
	  gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_5);
	  
	  
}

/*!
    \brief      configure the ADC peripheral_OS_
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_config(void)
{
    /* ADC mode config */
    adc_mode_config(ADC_MODE_FREE);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    /* ADC channel length config */
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 1U);
    
    /* ADC trigger config */
    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE); 
    /* ADC external trigger config */
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);

    /* enable ADC interface */
    adc_enable(ADC0);
    delay_1ms(1U);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);
}


/*!
    \brief      ADC channel sample
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint16_t adc_channel_sample(uint8_t channel)
{
    /* ADC regular channel config */
    adc_regular_channel_config(ADC0, 0U, channel, ADC_SAMPLETIME_7POINT5);
    /* ADC software trigger enable */
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);

    /* wait the end of conversion flag */
    while(!adc_flag_get(ADC0, ADC_FLAG_EOC));
    /* clear the end of conversion flag */
    adc_flag_clear(ADC0, ADC_FLAG_EOC);
    /* return regular channel sample value */
    return (adc_regular_data_read(ADC0));
}
int fputc(int ch,FILE *f){
    
usart_data_transmit(EVAL_COM1,(uint8_t)ch);
	while (RESET==usart_flag_get(EVAL_COM1,USART_FLAG_TBE));
	return ch ;

}