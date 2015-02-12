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
            }
            break;
    }
}

void ClearScreen (void) {
    iprintf("\033[2J\033[1;1H");
}
