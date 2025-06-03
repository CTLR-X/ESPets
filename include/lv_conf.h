#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/* Architecture specific settings */
#define LV_ARCH_ESP32 1
#define LV_USE_ARM_HELIUM 0
#define LV_USE_ARM_NEON 0
#define LV_USE_ARM_DSP 0

/* Color depth: 1 (1 byte per pixel), 8 (RGB332), 16 (RGB565), 32 (ARGB8888) */
#define LV_COLOR_DEPTH 16
#define LV_COLOR_16_SWAP 1
#define LV_USE_PERF_MONITOR 1


/* Use more complex drawing routines to enable anti-aliasing, gradients, etc. */
#define LV_DRAW_COMPLEX 1

/* Default display refresh period. LVG will redraw changed areas with this period time */
#define LV_DISP_DEF_REFR_PERIOD 30

/* Input device read period in milliseconds */
#define LV_INDEV_DEF_READ_PERIOD 30

/* Enable anti-aliasing (lines, and radiuses will be smoothed) */
#define LV_ANTIALIAS 1

/* Enable the built-in memory manager */
#define LV_MEM_CUSTOM 0
#define LV_MEM_SIZE (128U * 1024U)    /* Size of the memory used by `lv_mem_alloc` */
#define LV_MEM_ATTR                        /* Complier prefix for big arrays */
#define LV_MEM_ADR_INV 0xFFFFFFFF          /* Invalid address */
#define LV_MEM_AUTO_DEFRAG 1               /* Automatically defrag on free */

/* Enable the built-in (v)snprintf functions */
#define LV_SPRINTF_CUSTOM 0
#define LV_SPRINTF_INCLUDE <stdio.h>
#define lv_snprintf snprintf
#define lv_vsnprintf vsnprintf

/* Enable the built-in log module */
#define LV_USE_LOG 1
#define LV_LOG_LEVEL LV_LOG_LEVEL_INFO
#define LV_LOG_PRINTF 1

/* Enable the built-in demo widgets */
#define LV_USE_DEMO_WIDGETS 1

/* Enable the built-in music demo */

#define LV_USE_DEMO_BENCHMARK 1

/* Enable the built-in widgets */
#define LV_USE_WIDGETS 1

/* Enable the built-in label widget */
#define LV_USE_LABEL 1

/* Enable the built-in button widget */
#define LV_USE_BTN 1

/* Enable the built-in image widget */
#define LV_USE_IMG 1

/* Enable the built-in slider widget */
#define LV_USE_SLIDER 1

/* Enable the built-in chart widget */
#define LV_USE_CHART 1

/* Enable the built-in table widget */
#define LV_USE_TABLE 1

/* Enable the built-in textarea widget */
#define LV_USE_TEXTAREA 1

/* Enable the built-in keyboard widget */
#define LV_USE_KEYBOARD 1

/* Enable the built-in message box widget */
#define LV_USE_MSGBOX 1

/* Enable the built-in spinbox widget */
#define LV_USE_SPINBOX 1

/* Enable the built-in spinner widget */
#define LV_USE_SPINNER 1

/* Enable the built-in calendar widget */
#define LV_USE_CALENDAR 1

/* Enable the built-in canvas widget */
#define LV_USE_CANVAS 1

/* Enable the built-in list widget */
#define LV_USE_LIST 1

/* Enable the built-in menu widget */
#define LV_USE_MENU 1

/* Enable the built-in meter widget */
#define LV_USE_METER 1

/* Enable the built-in msgbox widget */
#define LV_USE_MSGBOX 1

/* Enable the built-in tabview widget */
#define LV_USE_TABVIEW 1

/* Enable the built-in tileview widget */
#define LV_USE_TILEVIEW 1

/* Enable the built-in win widget */
#define LV_USE_WIN 1

/* Enable the built-in color picker widget */
#define LV_USE_COLORWHEEL 1

/* Enable the built-in led widget */
#define LV_USE_LED 1

#endif /*LV_CONF_H*/ 