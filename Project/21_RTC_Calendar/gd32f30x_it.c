/*!
    \file  gd32f30x_it.c
    \brief interrupt service routines
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-06-23, V1.0.0, demo for GD32F30x
*/

#include "gd32f30x_it.h"
#include "gd32f303c_eval.h"
#include "rtc.h"

extern __IO uint32_t timedisplay;
/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief      this function handles RTC global interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void RTC_IRQHandler(void)
{
    if (rtc_flag_get(RTC_FLAG_SECOND) != RESET){
        /* clear the RTC second interrupt flag*/
        rtc_flag_clear(RTC_FLAG_SECOND);    
        /* enable time update */
        timedisplay = 1;

        /* reset RTC counter when time is 23:59:59 */
        if (rtc_counter_get() == 0x00015180){        
            /* wait until last write operation on RTC registers has finished */
            rtc_lwoff_wait();
            rtc_counter_set(0x0);
            /* wait until last write operation on RTC registers has finished */
            rtc_lwoff_wait();
        }
    }
    if (rtc_flag_get(RTC_FLAG_ALARM) != RESET){
        /* clear the RTC alarm interrupt flag*/
        rtc_flag_clear(RTC_FLAG_ALARM); 
        timedisplay = 0;
        printf("\r\n==============RTC Alarm and turn on LED=====================================\r\n");
        if(rtc_counter_get()>0)
        time_display(rtc_counter_get()-1);
        gd_eval_led_on(LED2);
        gd_eval_led_on(LED3);
        gd_eval_led_on(LED4);
        gd_eval_led_on(LED5);      
    }
}
