/**
    \file main.c
    \version 0.0.5
    \date 2014-07-20
    \brief Simple demonstration program. Based on demo.c of 
    Andrew Payne <andy@payne.org>. The spirit of demo is show 
    functionality and create a base for yours projects.
    \author Nelson Lombardo
    \license This file is released under the MIT License.
    \include LICENSE
*/
/**
    \mainpage OpenKL25Z4
    
    Introduction
    ============

    The goal of this project is make a clean and single framework to learn about microcontroller Cortex-M0. 
    The project is based on [bare-metal ARM](https://github.com/payne92/bare-metal-arm). In this moment I work actively in add the funcionality that I want. This is a "bare metal" runtime for the 
    [Freescale Freedom FRDM-KL25Z](http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=FRDM-KL25Z) 
    ARM development board ($13). It builds with the GCC ARM toolchain, with no other external dependencies.

    All about project can be found on [OpenKL25Z4](http://1nv1.github.io/OpenKL25Z4/).

    Quick start on Linux or Mac OS X:
    
    + Clone the repo: `git clonehttps://github.com/1nv1/OpenKL25Z4`
    + Grab and unpack GCC ARM toolchain:
        + On Ubuntu: `sudo apt-get install gcc-arm-none-eabi`
        + On Mac & Linux: `cd bare-metal-arm; make gcc-arm`
    + `make`

    This will create a `main.srec` image file to flash onto the development board.  (If you're using
    the standard bootloader, plug the SDA USB port to a host computer.  On Linux, type `make deploy`.  On other systems,
    copy the .SREC file to the FRDM-KL25Z volume.)  

    If everything is working, the RGB LEB will flash a few times and then be steady green.  You can access the USB 
    SDA serial port (at 115,200 baud) and see the conversion of ADC of input 8 in single mode.

    References:
    
    + [Freescale Freedom FRDM-KL25Z](http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=FRDM-KL25Z)
        + Where to buy: [Newark](http://www.newark.com/jsp/search/productdetail.jsp?SKU=28W5033&CMP=KNC-GPLA&mckv=|pcrid|27090073701|plid|),
    [Digi-Key](http://www.digikey.com/product-detail/en/FRDM-KL25Z/FRDM-KL25Z-ND/3529594?WT.mc_id=PLA_3529594)
    + [KL25 Sub-Family Reference Manual](http://cache.freescale.com/files/32bit/doc/ref_manual/KL25P80M48SF0RM.pdf)
    + [GCC ARM toolchain](https://launchpad.net/gcc-arm-embedded)
    + [Newlib C library](http://sourceware.org/newlib/)

    \license This project is released under the MIT License.
    \include LICENSE
*/

#include "types.h"
#include "freedom.h"
#include "common.h"
#include "driver_ADC.h"
#include "app_menu.h"
#include "pt.h"
#include "pt-sem.h"
#include "main.h"

  
extern char *_sbrk(int len);
uint32_t sysBuffer[7]; 

/* Two flags that the two protothread functions use. */
static uint8_t pthScanAdcFlag, pthScanAccelFlag, pthScanTouchFlag, pthMonitorFlag, pthCollectorFlag, pthStateAppMenuFlag;

static uint8_t change = 0;

static struct pt ptScanAdc, ptScanAccel, ptScanTouch, ptMonitor, ptCollector, ptStateAppMenu;

/* Prototypes */
static uint32_t pthScanAdc      (struct pt *pt);
static uint32_t pthScanAccel    (struct pt *pt);
static uint32_t pthScanTouch    (struct pt *pt);
static uint32_t pthMonitor      (struct pt *pt);
static uint32_t pthCollector    (struct pt *pt);
static uint32_t pthStateAppMenu (struct pt *pt);

// Main program
int main(void)
{
    // Initialize all modules
    adc_init();
    uart_init(115200);
    accel_init();
    touch_init((1 << 9) | (1 << 10));       // Channels 9 and 10
    // usb_init();
    setvbuf(stdin, NULL, _IONBF, 0);        // No buffering

    // Run tests
    tests();

    // Blink the green LED to indicate booting
    unsigned int pattern = 0b1100110011001100;
    while(pattern) {
        RGB_LED(0, pattern & 1 ? 100 : 0, 0);           // Set GREEN led based on LSB
        pattern >>= 1;
        delay(25);
    }
    
    adc_init();
    // Configure the ADC
    adc_int_disable();
    adc_primary_configuration (ADC_POWER_MODE_LOW, ADC_INPUT_CLOCK_DIV_1, ADC_SAMPLE_TIME_LONG, ADC_SINGLE_RESOLUTION_16, ADC_INPUT_BUS_CLK);

    adc_control_1 (ADC_TRIGGER_SOFTWARE, ADC_COMPARE_DISABLE, ADC_GREATER_THAN, ADC_COMP_RANGE_CV1, ADC_DMA_DISABLE, ADC_COMP_REF_VOL_DEFAULT);
    adc_control_2 (ADC_OP_SINGLE, ADC_AVERAGE_DISABLE, ADC_HARDWARE_AVERAGE_4);
    adc_control_channel (ADC_INPUT_SINGLE, ADC_AD8);
    
    char i;
    char *heap_end;
    
    /* This banner is from the original framework */
    // Welcome banner
    //iprintf("\033[2J\033[1;1H");
    //iprintf("\r\n\r\n====== Freescale Freedom FRDM-KL25Z\r\n");
    //iprintf("Built: %s %s\r\n\r\n", __DATE__, __TIME__);
    //heap_end = _sbrk(0);
    //iprintf("Reset code: 0x%02x 0x%02x\r\n", RCM_SRS0, RCM_SRS1);
    //iprintf("Heap: %p to %p (%d bytes used)\r\n", __heap_start, heap_end,
    //heap_end - (char *)__heap_start);
    //iprintf("Stack: %p to %p (%d bytes used)\r\n", &i, __StackTop,
    //(char *)__StackTop - &i);
    //iprintf("%d bytes free\r\n", &i - heap_end);
    
    pthScanAdcFlag      = FALSE;
    pthScanAccelFlag    = FALSE;
    pthScanTouchFlag    = FALSE;
    pthMonitorFlag      = FALSE;
    pthCollectorFlag    = FALSE;
    pthStateAppMenuFlag = FALSE;
    
    /* Initialize the protothread state variables with PT_INIT(). */
    PT_INIT(&ptScanAdc);
    PT_INIT(&ptScanAccel);
    PT_INIT(&ptScanTouch);
    PT_INIT(&ptMonitor);
    PT_INIT(&ptCollector);
    PT_INIT(&ptStateAppMenu);
    
    InitAppMenu ();
    
    /*
     * Scheduler for protothreads (cooperative context)
     */
    for (;;) {
        pthScanAdc      (&ptScanAdc);
        pthScanAccel    (&ptScanAccel);
        pthScanTouch    (&ptScanTouch);
        pthCollector    (&ptCollector);
        pthStateAppMenu (&ptStateAppMenu);
        //pthMonitor      (&ptMonitor); /* Special PT for test*/
    }
}

static uint32_t pthScanAdc (struct pt *pt)
{
    /* A protothread function must begin with PT_BEGIN() which takes a
     pointer to a struct pt. */
    PT_BEGIN(pt);

    /* We loop forever here. */
    for(;;) {
        /* Wait until the other protothread has set its flag. */
        PT_WAIT_UNTIL(pt, pthScanAdcFlag == FALSE);
        PT_WAIT_UNTIL(pt, adc_data_is_ready() == TRUE);
        /* Trigger ADC when write the SC1 (in this case channel select) */
        switch (change) {
            case 0: 
                sysBuffer[idxADC1] = adc_data_get();
                adc_channel (ADC_AD8); // Software event - Channel A0 
                break;
            case 1: 
                sysBuffer[idxADC0] = adc_data_get();
                adc_channel (ADC_AD9); // Software event - Channel A0 
                break;
        }
        change ++;
        if (change > 1) change = 0;
        pthScanAdcFlag = TRUE;
    /* And we loop. */
    }
    /* All protothread functions must end with PT_END() which takes a
     pointer to a struct pt. */
    PT_END(pt);
}

static uint32_t pthScanAccel (struct pt *pt)
{
    /* A protothread function must begin with PT_BEGIN() which takes a
     pointer to a struct pt. */
    PT_BEGIN(pt);

    /* We loop forever here. */
    for(;;) {
        /* Wait until the other protothread has set its flag. */
        PT_WAIT_UNTIL(pt, pthScanAccelFlag == FALSE);
        sysBuffer[idxAccelX] = accel_x();
        sysBuffer[idxAccelY] = accel_y();
        sysBuffer[idxAccelZ] = accel_z();
        pthScanAccelFlag = TRUE;
    /* And we loop. */
    }
    /* All protothread functions must end with PT_END() which takes a
     pointer to a struct pt. */
    PT_END(pt);
}

static uint32_t pthScanTouch (struct pt *pt)
{
    /* A protothread function must begin with PT_BEGIN() which takes a
     pointer to a struct pt. */
    PT_BEGIN(pt);

    /* We loop forever here. */
    for(;;) {
        /* Wait until the other protothread has set its flag. */
        PT_WAIT_UNTIL(pt, pthScanTouchFlag == FALSE);
        sysBuffer[idxTouchDataX] = touch_data(9);
        sysBuffer[idxTouchDataY] = touch_data(10);
        pthScanTouchFlag = TRUE;
    /* And we loop. */
    }
    /* All protothread functions must end with PT_END() which takes a
     pointer to a struct pt. */
    PT_END(pt);
}

static uint32_t pthMonitor (struct pt *pt)
{
    /* A protothread function must begin with PT_BEGIN() which takes a
     pointer to a struct pt. */
    PT_BEGIN(pt);

    /* We loop forever here. */
    for(;;) {
        /* Wait until the other protothread has set its flag. */
        PT_WAIT_UNTIL(pt, pthScanAdcFlag    == TRUE );
        PT_WAIT_UNTIL(pt, pthScanAccelFlag  == TRUE );
        PT_WAIT_UNTIL(pt, pthScanTouchFlag  == TRUE );
        if ((int32_t) (sysBuffer[idxTouchDataY]) > 10) {
            iprintf("\033[2J\033[1;1H");
            iprintf("> Request: \r\n");
            iprintf("  Analog: A0 = %5d A1 = %5d\r\n", sysBuffer[idxADC0], sysBuffer[idxADC1]);
            iprintf("  Inputs:  X = %5d  Y = %5d  Z = %5d\r\n", sysBuffer[idxAccelX], sysBuffer[idxAccelY], sysBuffer[idxAccelZ]);
            iprintf("  Touch:       %5d      %5d\r\n", sysBuffer[idxTouchDataX], sysBuffer[idxTouchDataY]);
            blink (FAULT_SLOW_BLINK, 10);
            RGB_LED(0, 0b1100110011001100, 0); 
        }
        pthScanAdcFlag    = FALSE;
        pthScanAccelFlag  = FALSE;
        pthScanTouchFlag  = FALSE;
    /* And we loop. */
    }
    /* All protothread functions must end with PT_END() which takes a
     pointer to a struct pt. */
    PT_END(pt);
}

static uint32_t pthCollector (struct pt *pt) {
    PT_BEGIN(pt);
    for(;;) {
        PT_WAIT_UNTIL(pt, pthStateAppMenuFlag == FALSE);
        if ((sysBuffer[idxTouchDataY] > 500) && (sysBuffer[idxTouchDataX] < 500) && (sysBuffer[idxTouchDataX] > 30)) {
            if (EventAppMenu != eUp) {
                EventAppMenu = eUp;
                pthCollectorFlag = FALSE;
            } else {
                pthCollectorFlag = TRUE;
            }
        }
        else if ((sysBuffer[idxTouchDataY] > 30) && (sysBuffer[idxTouchDataY] < 500) && (sysBuffer[idxTouchDataX] > 500)) {
            if (EventAppMenu != eSelect) {
                EventAppMenu = eSelect;
                pthCollectorFlag = FALSE;
            } else {
                pthCollectorFlag = TRUE;
            }
        }
        else {
            EventAppMenu = eNoEvent; /* Event reset */
            pthCollectorFlag = TRUE;
        }
        pthScanAdcFlag    = FALSE;
        pthScanAccelFlag  = FALSE;
        pthScanTouchFlag  = FALSE;
        PT_EXIT(pt);
    }
    PT_END(pt);
} 

static uint32_t pthStateAppMenu (struct pt *pt)
{
    PT_BEGIN(pt);
    for (;;) {
        PT_WAIT_UNTIL(pt, pthCollectorFlag == FALSE);
        switch (StateAppMenu) {
            case MENU:
                switch (EventAppMenu) {
                    case eSelect:
                        vItem = 0;
                        aPrint (AppStringSubmenu[vSubmenu], vItem);
                        StateAppMenu = SUBMENU;
                        break;
                    case eUp:
                        if (chkLast(AppStringMenu, vSubmenu)) {
                            vSubmenu = 0;
                        } else {
                            vSubmenu ++;
                        }
                        aPrint (AppStringMenu, vSubmenu);
                        break;
                }
                break;
            case SUBMENU:
                switch (EventAppMenu) {
                    case eSelect:
                        if (chkLast(AppStringSubmenu[vSubmenu], vItem) == TRUE){
                            vSubmenu = 0;
                            aPrint (AppStringMenu, vSubmenu);
                            StateAppMenu = MENU;
                        } else {
                            aTask (AppStringSubmenu[vSubmenu], vSubmenu, vItem);
                            StateAppMenu = TASK;
                        }
                        break;
                    case eUp:
                        if (chkLast(AppStringSubmenu[vSubmenu], vItem) == TRUE) {
                            vItem = 0;
                        } else {
                            vItem ++;
                        }
                        aPrint (AppStringSubmenu[vSubmenu], vItem);
                        break;
                }
                break;
            case TASK:
                switch (EventAppMenu){
                    case eUp:
                        StateAppMenu = SUBMENU;
                        break;
                    case eSelect:
                        aTask (AppStringSubmenu[vSubmenu], vSubmenu, vItem);
                        break;
                }
                break;
        }
        pthStateAppMenuFlag = FALSE;
        pthCollectorFlag = TRUE;
        PT_EXIT(pt);
    }
    PT_END(pt);
}
