/* SPDX-License-Identifier: GPL-2.0-or-later */


#ifndef __MFD_WM831X_WATCHDOG_H__
#define __MFD_WM831X_WATCHDOG_H__



#define WM831X_WDOG_ENA                         0x8000  
#define WM831X_WDOG_ENA_MASK                    0x8000  
#define WM831X_WDOG_ENA_SHIFT                       15  
#define WM831X_WDOG_ENA_WIDTH                        1  
#define WM831X_WDOG_DEBUG                       0x4000  
#define WM831X_WDOG_DEBUG_MASK                  0x4000  
#define WM831X_WDOG_DEBUG_SHIFT                     14  
#define WM831X_WDOG_DEBUG_WIDTH                      1  
#define WM831X_WDOG_RST_SRC                     0x2000  
#define WM831X_WDOG_RST_SRC_MASK                0x2000  
#define WM831X_WDOG_RST_SRC_SHIFT                   13  
#define WM831X_WDOG_RST_SRC_WIDTH                    1  
#define WM831X_WDOG_SLPENA                      0x1000  
#define WM831X_WDOG_SLPENA_MASK                 0x1000  
#define WM831X_WDOG_SLPENA_SHIFT                    12  
#define WM831X_WDOG_SLPENA_WIDTH                     1  
#define WM831X_WDOG_RESET                       0x0800  
#define WM831X_WDOG_RESET_MASK                  0x0800  
#define WM831X_WDOG_RESET_SHIFT                     11  
#define WM831X_WDOG_RESET_WIDTH                      1  
#define WM831X_WDOG_SECACT_MASK                 0x0300  
#define WM831X_WDOG_SECACT_SHIFT                     8  
#define WM831X_WDOG_SECACT_WIDTH                     2  
#define WM831X_WDOG_PRIMACT_MASK                0x0030  
#define WM831X_WDOG_PRIMACT_SHIFT                    4  
#define WM831X_WDOG_PRIMACT_WIDTH                    2  
#define WM831X_WDOG_TO_MASK                     0x0007  
#define WM831X_WDOG_TO_SHIFT                         0  
#define WM831X_WDOG_TO_WIDTH                         3  

#endif
