#include <stdio.h>

#include "Utils/Logs.h"

#define COLOR_RESET "\033[0m"
#define COLOR_INFO "\033[0;37m"
#define COLOR_WARN "\033[0;33m"
#define COLOR_ERROR "\033[0;31m"

#define LINE_END "\033[s\n" COLOR_RESET
#define DEL_LINE_END "\033[u"

void info(char *msg) { printf(COLOR_INFO "[INFO] %s" LINE_END, msg); }
void warn(char *msg) { printf(COLOR_WARN "[WARN] %s" LINE_END, msg); }
void error(char *msg) { printf(COLOR_ERROR "[ERROR] %s" LINE_END, msg); }
void logAppend(char *msg) { printf(DEL_LINE_END "%s" LINE_END, msg); }
