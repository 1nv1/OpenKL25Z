#include "app_menu.h"

char *AppStringMenu[] = { 
    "Edit Preferences",
    "Print Sensor",
    "Info",
    ""
};

char *AppStringSubmenu1[] = {
    "Nothing to do",
    "Back",
    ""
};  

char *AppStringSubmenu2[] = {
    "A0",
    "A1",
    "A2",
    "A3",
    "A4",
    "A5",
    "AccX",
    "AccY",
    "AccZ",
    "TchX",
    "TchY",
    "Back",
    ""
};

char *AppStringSubmenu3[] = {
    "About",
    "Test LED",
    "Debug",
    "Back",
    ""
};

char *(*AppStringSubmenu[3]);

uint8_t StateAppMenu;   // Estado actual
uint8_t EventAppMenu;   // Evento actual

uint8_t vSubmenu;
uint8_t vItem;

void InitAppMenu (void)
{
    StateAppMenu = STATE_START;
    EventAppMenu = eUp;
    vItem = 0;
    vSubmenu = 0;
    /* Submenu load */
    AppStringSubmenu[0] = AppStringSubmenu1;
    AppStringSubmenu[1] = AppStringSubmenu2;
    AppStringSubmenu[2] = AppStringSubmenu3;
    aPrint(AppStringMenu, vItem);
}

void aPrint (char **str, uint8_t item) {
    aPrintExt (str, item, NULL);
}

void aPrintExt (char **str, uint8_t item, uint32_t *value) {
    uint8_t line;
    ClearScreen ();
    iprintf(" :: Menu ::\r\n");
    for (line = 0; strcmp(str[line], "") != 0; line ++) {
        if (line == item) {
            if (value == NULL) {
                iprintf("> %s\r\n", str[line]);
            }
            else {
                iprintf("> %s: %li\r\n", str[line], *value);
            }
        } 
        else {
            printf("%s\r\n", str[line]);
        }
    }    
}

uint8_t chkLast (char **str, uint8_t item) {
    if (strcmp(str[item + 1], "") == 0) return TRUE;
    else return FALSE;
}

void aTask (char **str, uint8_t x, uint8_t y) {
    switch (x) {
        case 0: /* Submenu 0: Nothing to do*/
            break;
        case 1: /* Submenu 1: Print sensors*/
            aPrintExt (str, y, &(sysBuffer[y]));
            break;
        case 2:
            switch(y) {
                case 0:
                    ClearScreen ();
                    iprintf("\r\nOpenKL25Z: baremetal framework for educational purposes\r\n(C) Nelson Lombardo - MIT License\r\nContact: nelson.lombardo@gmail.com\r\n");
                    break;
                case 1: 
                    ClearScreen ();
                    iprintf("Testing LED...\r\n");
                    blink (FAULT_SLOW_BLINK, 100);
                    RGB_LED(0, 0xFFFFFFFF, 0);
                    ClearScreen ();
                    iprintf("Done!\r\n");                   
                    break;
                case 2:
                    ClearScreen ();
                    iprintf("ADC0:\r\n");
                    iprintf("  clkgate ");
                    PrintHex((SIM_SCGC6 & SIM_SCGC6_ADC0_MASK) >> SIM_SCGC6_ADC0_SHIFT);
                    iprintf("\r\n");
                    iprintf("      res ");
                    PrintHex((ADC0_CFG1 & ADC_CFG1_MODE_MASK) >> ADC_CFG1_MODE_SHIFT);
                    iprintf("\r\n");
                    
                    iprintf("LPTMR0:\r\n");
                    iprintf("  enabled ");
                    PrintHex((SIM_SCGC5 & SIM_SCGC5_LPTMR_MASK) >> SIM_SCGC5_LPTMR_SHIFT);
                    iprintf("\r\n");
                    iprintf("  counter ");
                    PrintHex((LPTMR0_CNR & LPTMR_CNR_COUNTER_MASK) >> LPTMR_CNR_COUNTER_SHIFT);
                    iprintf("\r\n");
                    iprintf("  compare "); 
                    PrintHex((LPTMR0_CMR & LPTMR_CMR_COMPARE_MASK) >> LPTMR_CMR_COMPARE_SHIFT);
                    iprintf("\r\n");
                    iprintf("      int "); 
                    PrintHex((LPTMR0_CSR & LPTMR_CSR_TIE_MASK) >> LPTMR_CSR_TIE_SHIFT);
                    iprintf("\r\n");
                    iprintf("      clk ");
                    PrintHex((LPTMR0_CSR & LPTMR_CSR_TMS_MASK) >> LPTMR_CSR_TMS_SHIFT);
                    iprintf("\r\n");
                    break;
            }
            break;
    }
}

void ClearScreen (void) {
    iprintf("\033[2J\033[1;1H");
}

void PrintHex (uint32_t num){
    iprintf("%#04x", num);
}
