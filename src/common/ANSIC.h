#pragma once

//https://misc.flogisoft.com/bash/tip_colors_and_formatting

//Stringyfier for c preproccessor
#define ANSIC_STR1(x) #x
#define ANSIC_STR(x) ANSIC_STR1 (x)


//ANSI text color escape string formatter
#define ANSIC(text, foreground, background) \
"\x1B[" ANSIC_STR (text) ";3" ANSIC_STR (foreground) ";4" ANSIC_STR (background) "m"


//Used for text attributes
#define ANSIC_NORMAL       0
#define ANSIC_BOLD         1
#define ANSIC_UNDERSCORE   4
#define ANSIC_BLINK        5
#define ANSIC_REVERSE      7
#define ANSIC_CONCEAL      8


//Used for foreground and background color
#define ANSIC_BLACK   0
#define ANSIC_RED     1
#define ANSIC_GREEN   2
#define ANSIC_YELLOW  3
#define ANSIC_BLUE    4
#define ANSIC_MAGENTA 5
#define ANSIC_CYAN    6
#define ANSIC_WHITE   7
#define ANSIC_DEFAULT 9


//Default text color used in terminals
//#define ANSIC_DEFAULT ANSIC (ANSIC_NORMAL, ANSIC_DEFAULT, ANSIC_DEFAULT)
#define ANSIC_RESET "\x1B[0m"
