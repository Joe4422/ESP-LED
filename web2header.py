from os import listdir

MAIN_DIR =      "main/"
WEB_DIR =       "web/"
WEB_DATA_H =    "web_data.h"
WEB_DATA_C =    "web_data.c"

VAR_EXTERN =    "extern "
VAR_TYPE =      "const char* "

WEB_DATA_H_START = """#ifndef WEB_DATA_H_
#define WEB_DATA_H_

/****************************************************************
 * Defines, consts
 ****************************************************************/
"""

WEB_DATA_H_END = "#endif /* WEB_DATA_H_ */"

WEB_DATA_C_START = """/****************************************************************
 * Includes
 ****************************************************************/
#include "web_data.h"

/****************************************************************
 * Defines, consts
 ****************************************************************/
"""

WEB_DATA_H_DATA = WEB_DATA_H_START
WEB_DATA_C_DATA = WEB_DATA_C_START

files = listdir(WEB_DIR)

for file in files:
    f = open(WEB_DIR + file, "r")
    VARIABLE_NAME = "WEB_" + file.upper().replace(".", "_", 1)
    WEB_DATA_H_DATA = WEB_DATA_H_DATA + VAR_EXTERN + VAR_TYPE + VARIABLE_NAME + ";\n\n"
    WEB_DATA_C_DATA = WEB_DATA_C_DATA + VAR_TYPE + VARIABLE_NAME + " = \""
    for line in f:
        WEB_DATA_C_DATA = WEB_DATA_C_DATA + line.replace("\n", "").replace("\"", "\\\"") + " \\\n"

    f.close()

    WEB_DATA_C_DATA = WEB_DATA_C_DATA[:-3] + "\";\n\n"

WEB_DATA_H_DATA = WEB_DATA_H_DATA + WEB_DATA_H_END

wdh = open(MAIN_DIR + WEB_DATA_H, "w")
wdh.write(WEB_DATA_H_DATA)
wdh.close()

wdc = open(MAIN_DIR + WEB_DATA_C, "w")
wdc.write(WEB_DATA_C_DATA)
wdc.close()
    
