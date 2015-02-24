/**
    \file main.c
    \version 0.0.6
    \date 2015-02-23
    \brief Simple demonstration program. The spirit of demo is show 
    functionality and create a base for yours projects.
    \author Nelson Lombardo
    \license This file is released under the MIT License.
    \include LICENSE
*/
/**
    \mainpage OpenKL25Z4
    
    Introduction
    ============

    The goal of this project is make a single framework to learn about microcontroller Cortex-M0+. 
    The project is based on [bare-metal ARM](https://github.com/payne92/bare-metal-arm). In this moment I work actively in add the funcionality that I want. This is a "bare metal" runtime for the 
    [Freescale Freedom FRDM-KL25Z](http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=FRDM-KL25Z) 
    ARM development board ($12.95 at ). It builds with the GCC ARM toolchain, with no other external dependencies.

    Quick start on Linux:
    
    + Clone the repo: `git clonehttps://github.com/1nv1/OpenKL25Z4`
    + Grab and unpack GCC ARM toolchain:
        + On Ubuntu/Debian/Fedora: `sudo apt-get install gcc-arm-none-eabi`
    + `make`

    This will create a `main.srec` image file to flash onto the development board.  (If you're using the standard bootloader, plug the SDA USB port to a host computer.  On Linux, type `make deploy` or you can use `make burn`, that command has the same effect that `make clean && make && make deploy`.  On other systems,copy the .SREC file to the FRDM-KL25Z volume.)
    
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
/* For use the debug mode use it:
 * make DEFS=-DDEBUG_MODE 
 */

#include "types.h"
#include "freedom.h"
#include "common.h"
#include "driver_ADC.h"
#include "driver_SYSTICK.h"
#include "app_menu.h"
#include "pt.h"
#include "pt-sem.h"
#include "main.h"

extern char *_sbrk(int len);
uint32_t sysBuffer[11]; 

/* Two flags that the two protothread functions use. */
static uint8_t pthScanAdcFlag, pthScanAccelFlag, pthScanTouchFlag, pthCollectorFlag, pthStateAppMenuFlag;

static struct pt ptScanAdc;
static struct pt ptScanAccel;
static struct pt ptScanTouch;
static struct pt ptCollector;
#ifdef DEBUG_MODE
static struct pt ptMonitor;
#else
static struct pt ptStateAppMenu;
#endif

/* 
 * Prototypes of protothreads 
 */
static uint32_t pthScanAdc      (struct pt *pt);
static uint32_t pthScanAccel    (struct pt *pt);
static uint32_t pthScanTouch    (struct pt *pt);
static uint32_t pthCollector    (struct pt *pt);
static uint32_t pthStateAppMenu (struct pt *pt);
static uint32_t pthMonitor      (struct pt *pt);

/* 
 * Prototype for use SysTick interrupt 
 */
void SysTick_Handler() __attribute__((interrupt("IRQ")));

// Main program
int main(void)
{
    /*
     * Configuring SysTick
     */
    systick_reload_value((CORE_CLOCK/10u) - 1); // 10 Hz
    systick_counter_value(0);
    systick_clock_source(CLOCK_SOURCE_PROCESSOR);
    systick_enable(ON); 
    systick_int(ON);
    
    /* 
     * Configuring ADC
     */
    adc_init();
    adc_int_disable();
    adc_primary_configuration (ADC_POWER_MODE_LOW, ADC_INPUT_CLOCK_DIV_1, ADC_SAMPLE_TIME_LONG, ADC_SINGLE_RESOLUTION_16, ADC_INPUT_BUS_CLK);
    adc_control_1 (ADC_TRIGGER_SOFTWARE, ADC_COMPARE_DISABLE, ADC_GREATER_THAN, ADC_COMP_RANGE_CV1, ADC_DMA_DISABLE, ADC_COMP_REF_VOL_DEFAULT);
    adc_control_2 (ADC_OP_SINGLE, ADC_AVERAGE_DISABLE, ADC_HARDWARE_AVERAGE_4);
    adc_control_channel (ADC_INPUT_SINGLE, ADC_AD8);

    /*
     * Initialization of left modules
     */
    uart_init(115200);
    accel_init();
    touch_init((1 << 9) | (1 << 10));       // Channels 9 and 10
    // usb_init();
    setvbuf(stdin, NULL, _IONBF, 0);        // No buffering

    /*
     * Run tests
     */
    tests();

    /* 
     * Blink the green LED to indicate booting
     */
    unsigned int pattern = 0b1100110011001100;
    while(pattern) {
        // Set GREEN led based on LSB
        RGB_LED(0, pattern & 1 ? 100 : 0, 0);
        pattern >>= 1;
        delay(25);
    }

    /*
     * Initialization of flags
     */
    pthScanAdcFlag      = FALSE;
    pthScanAccelFlag    = FALSE;
    pthScanTouchFlag    = FALSE;
    pthCollectorFlag    = FALSE;
    pthStateAppMenuFlag = FALSE;
    
    /* 
     * Initialize the protothread state variables with PT_INIT()
     */
    PT_INIT(&ptScanAdc);
    PT_INIT(&ptScanAccel);
    PT_INIT(&ptScanTouch);
    #ifdef DEBUG_MODE
    PT_INIT(&ptCollector);
    #else
    PT_INIT(&ptStateAppMenu);
    #endif
    
    /*
     * Initialize state-machine of serial menu
     */
    InitAppMenu ();
    
    /* 
     * Scheduler for protothreads (cooperative context) 
     */
    for (;;) {
        pthScanAdc      (&ptScanAdc);
        pthScanAccel    (&ptScanAccel);
        pthScanTouch    (&ptScanTouch);
        pthCollector    (&ptCollector);
        #ifdef DEBUG_MODE
        pthMonitor      (&ptMonitor);
        #else
        pthStateAppMenu (&ptStateAppMenu);
        #endif
    }
}

/* 
 * This protothread scan all analog inputs on the board 
 * (arduino compatible pins) 
 */
static uint32_t pthScanAdc (struct pt *pt)
{
    /* A protothread function must begin with PT_BEGIN() which takes a
     pointer to a struct pt. */
    PT_BEGIN(pt);
    /* Wait until the other protothread has set its flag. */
    PT_WAIT_UNTIL(pt, pthScanAdcFlag == FALSE);
    adc_channel (ADC_AD8); // Software event - Channel A0 
    
    PT_WAIT_UNTIL(pt, adc_data_is_ready() == TRUE);
    sysBuffer[idxADC0] = adc_data_get();
    adc_channel (ADC_AD9); // Software event - Channel A1
    
    PT_WAIT_UNTIL(pt, adc_data_is_ready() == TRUE);
    sysBuffer[idxADC1] = adc_data_get();
    adc_channel (ADC_AD12); // Software event - Channel A2         
    
    PT_WAIT_UNTIL(pt, adc_data_is_ready() == TRUE);
    sysBuffer[idxADC2] = adc_data_get();
    adc_channel (ADC_AD13); // Software event - Channel A3 
    
    PT_WAIT_UNTIL(pt, adc_data_is_ready() == TRUE);
    sysBuffer[idxADC3] = adc_data_get();
    adc_channel (ADC_AD11); // Software event - Channel A4
    
    PT_WAIT_UNTIL(pt, adc_data_is_ready() == TRUE);
    sysBuffer[idxADC4] = adc_data_get();
    adc_channel (ADC_AD15); // Software event - Channel A5 
    
    PT_WAIT_UNTIL(pt, adc_data_is_ready() == TRUE);
    sysBuffer[idxADC5] = adc_data_get();
    
    pthScanAdcFlag = TRUE;
    PT_END(pt);
}

/*
 * This protothread scan the accelerometer 
 */
static uint32_t pthScanAccel (struct pt *pt)
{
    /* A protothread function must begin with PT_BEGIN() which takes a
     pointer to a struct pt. */
    PT_BEGIN(pt);
    /* Wait until the other protothread has set its flag. */
    PT_WAIT_UNTIL(pt, pthScanAccelFlag == FALSE);
    sysBuffer[idxAccelX] = accel_x();
    sysBuffer[idxAccelY] = accel_y();
    sysBuffer[idxAccelZ] = accel_z();
    pthScanAccelFlag = TRUE;
    PT_END(pt);
}

/*
 * This protothread scan the capacitive touch over the board 
 */
static uint32_t pthScanTouch (struct pt *pt)
{
    /* A protothread function must begin with PT_BEGIN() which takes a
     pointer to a struct pt. */
    PT_BEGIN(pt);
    /* Wait until the other protothread has set its flag. */
    PT_WAIT_UNTIL(pt, pthScanTouchFlag == FALSE);
    sysBuffer[idxTouchDataX] = touch_data(9);
    sysBuffer[idxTouchDataY] = touch_data(10);
    pthScanTouchFlag = TRUE;
    PT_END(pt);
}

/*
 * This protothread make possible use capacitive touch like as two 
 * buttons over the board 
 */
static uint32_t pthCollector (struct pt *pt) 
{
    PT_BEGIN(pt);
    PT_WAIT_UNTIL(pt, 
        (pthScanTouchFlag == TRUE) &&
        (pthScanAccelFlag == TRUE) &&
        (pthScanAdcFlag == TRUE)
    );
    if ((sysBuffer[idxTouchDataY] > 500) && (sysBuffer[idxTouchDataX] < 500) && (sysBuffer[idxTouchDataX] > 30))
    {
        if (EventAppMenu != eUp)
        {
            EventAppMenu = eUp;
            pthCollectorFlag = TRUE;
        }
    }
    else if ((sysBuffer[idxTouchDataY] > 30) && (sysBuffer[idxTouchDataY] < 500) && (sysBuffer[idxTouchDataX] > 500))
    {
        if (EventAppMenu != eSelect)
        {
            EventAppMenu = eSelect;
            pthCollectorFlag = TRUE;
        }
    }
    else
    {
        EventAppMenu = eNoEvent; /* Event reset */
        PT_EXIT(pt);
    }
    pthScanAdcFlag    = FALSE;
    pthScanAccelFlag  = FALSE;
    pthScanTouchFlag  = FALSE;
    PT_END(pt);
} 

/*
 * The state-machine for serial-menu 
 */
static uint32_t pthStateAppMenu (struct pt *pt)
{
    PT_BEGIN(pt);
    PT_WAIT_UNTIL(pt, pthCollectorFlag == TRUE);
    pthCollectorFlag = FALSE; /* Reset */
    if (StateAppMenu == MENU)
    {
        if (EventAppMenu == eSelect)
        {
            vItem = 0;
            aPrint (AppStringSubmenu[vSubmenu], vItem);
            StateAppMenu = SUBMENU;
        }
        else if (EventAppMenu == eUp)
        {
            if (chkLast(AppStringMenu, vSubmenu))
            {
                vSubmenu = 0;
            }
            else
            {
                vSubmenu ++;
            }
            aPrint (AppStringMenu, vSubmenu);
        }
    }
    else if (StateAppMenu == SUBMENU)
    {
        if (EventAppMenu == eSelect)
        {
            if (chkLast(AppStringSubmenu[vSubmenu], vItem) == TRUE)
            {
                vSubmenu = 0;
                aPrint (AppStringMenu, vSubmenu);
                StateAppMenu = MENU;
            }
            else
            {
                aTask (AppStringSubmenu[vSubmenu], vSubmenu, vItem);
                StateAppMenu = TASK;
            }
        }
        else if (EventAppMenu == eUp) 
        {
            if (chkLast(AppStringSubmenu[vSubmenu], vItem) == TRUE) 
            {
                vItem = 0;
            }
            else
            {
                vItem ++;
            }
            aPrint (AppStringSubmenu[vSubmenu], vItem);
        }
    }
    else if (StateAppMenu == TASK) 
    {
        if (EventAppMenu == eUp) 
        {
            StateAppMenu = SUBMENU;
        }
        else if (EventAppMenu == eSelect) {
            aTask (AppStringSubmenu[vSubmenu], vSubmenu, vItem);
        }
    }
    PT_END(pt);
}

/*
 * The interrupt handler for SysTick module on ARM core 
 */
void SysTick_Handler() {
    static uint8_t change;
    change = ~change;
    if (change) RGB_LED (0x00, 0x00, 0xFF);
    else RGB_LED (0x00, 0xFF, 0x00);
    pthScanAdcFlag    = FALSE;
    pthScanAccelFlag  = FALSE;
    pthScanTouchFlag  = FALSE;
    //pthCollectorFlag  = FALSE;
}

#ifdef DEBUG_MODE
static uint32_t pthMonitor (struct pt *pt)
{
    /* A protothread function must begin with PT_BEGIN() which takes a
     pointer to a struct pt. */
    PT_BEGIN(pt);

    /* Wait until the other protothread has set its flag. */
    PT_WAIT_UNTIL(pt, pthScanAdcFlag    == TRUE );
    PT_WAIT_UNTIL(pt, pthScanAccelFlag  == TRUE );
    PT_WAIT_UNTIL(pt, pthScanTouchFlag  == TRUE );
    if ((int32_t) (sysBuffer[idxTouchDataY]) > 10) {
        iprintf("\033[2J\033[1;1H");
        iprintf("> Request: \r\n");
        iprintf("  Analog: A0 = %5d A1 = %5d A2 = %5d A3 = %5d A4 = %5d A5 = %5d\r\n", sysBuffer[idxADC0], sysBuffer[idxADC1], sysBuffer[idxADC2], sysBuffer[idxADC3], sysBuffer[idxADC4], sysBuffer[idxADC5]);
        iprintf("  Inputs:  X = %5d  Y = %5d  Z = %5d\r\n", sysBuffer[idxAccelX], sysBuffer[idxAccelY], sysBuffer[idxAccelZ]);
        iprintf("  Touch:       %5d      %5d\r\n", sysBuffer[idxTouchDataX], sysBuffer[idxTouchDataY]);
        blink (FAULT_SLOW_BLINK, 10);
        RGB_LED(0, 0b1100110011001100, 0); 
    }
    pthScanAdcFlag    = FALSE;
    pthScanAccelFlag  = FALSE;
    pthScanTouchFlag  = FALSE;
    PT_END(pt);
}  
#endif  
/* End of main.c */
