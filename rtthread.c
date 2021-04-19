 
#include <rtthread.h>
#include <board.h>
#include <rtdevice.h>
#define LED1 GET_PIN(C, 0)
#define LED3 GET_PIN(E,0)
#define KEY3 GET_PIN(B,14)
static rt_thread_t led1_thread =RT_NULL;
static rt_thread_t led3_thread =RT_NULL;
static rt_uint8_t count;
static rt_uint8_t count1;
static void led1_thread_entry(void *parmeter)
{
    for(count =0;;count ++) {
		rt_pin_write(LED1, PIN_HIGH);   
		rt_kprintf("led_on,count: %d\r\n",count);
		rt_thread_mdelay(500);
		rt_pin_write(LED1, PIN_LOW);
		rt_kprintf("led off\r\n");
		rt_thread_mdelay(500);	
		}
}

static void led3_thread_entry(void *parmeter)
{
	while (1){
		
	if( rt_pin_read(KEY3)==0){
	
	 //  LED3=~LED3;
	  rt_pin_write(LED3, PIN_HIGH);	
	 //	rt_thread_mdelay(500);
	}
	else{
	  rt_pin_write(LED3, PIN_LOW);	
		//rt_thread_mdelay(500);
	}	
}
}

int main(void)
{
    //user app entry 		
    rt_pin_mode(LED1, PIN_MODE_OUTPUT);
	  rt_pin_mode(LED3,PIN_MODE_OUTPUT);
	  rt_pin_mode(KEY3,PIN_MODE_INPUT);
	  
	   
  /*  while (1) {
        rt_pin_write(LED1, PIN_HIGH );
        rt_thread_mdelay(500);
        rt_pin_write(LED1, PIN_LOW);
        rt_thread_mdelay(500);
    }  
	*/
	
	 led1_thread =rt_thread_create("led1",led1_thread_entry,RT_NULL,512,3,20);//创建线程
	 led3_thread =rt_thread_create("led3",led3_thread_entry,RT_NULL,512,3,20);
	 if(led1_thread !=RT_NULL)
   rt_thread_startup(led1_thread); //如果获得线程控制块，就启动这个线程
     else
       return -1;			 
	 
		  if(led3_thread !=RT_NULL)
   rt_thread_startup(led3_thread); //如果获得线程控制块，就启动这个线程
     else
       return -1;			 
	 
      return 0;
}
