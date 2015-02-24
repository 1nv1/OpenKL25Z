#ifndef APP_MENU_H
#define APP_MENU_H

#include <stdint.h>
#include <string.h> 
#include <stdio.h>
#include "types.h"
#include "main.h"
#include "common.h"

/* Events of menu */
extern enum _EventsAppMenu
{
    eSelect,
    eUp,
    eNoEvent
};

extern enum _StateAppMenu { 
    MENU, 
    SUBMENU, 
    TASK,
    STATE_MAX
};

#define STATE_START MENU

extern char *AppStringMenu[];

extern char *AppStringSubmenu1[];

extern char *AppStringSubmenu2[];

extern char *AppStringSubmenu3[];

extern char *(*AppStringSubmenu[3]);

//extern EventsAppMenu EventAppMenu; // Enumeración para los eventos
extern uint8_t StateAppMenu;   // Estado actual
extern uint8_t EventAppMenu;   // Evento actual

extern uint8_t vItem;
extern uint8_t vSubmenu;

// Acciones:
extern void aPrint (char **str, uint8_t item);

void aPrintExt (char **str, uint8_t item, uint32_t *value);

extern uint8_t chkLast (char **str, uint8_t item);

extern void aTask (char **str, uint8_t x, uint8_t y);

extern void InitAppMenu (void); 
extern void ClearScreen (void);
extern void PrintHex (uint32_t num);

#endif
