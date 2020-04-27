//
// Created by lothar on 26/03/2020.
//

#ifndef C_HAT_LOGGER_H
#define C_HAT_LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#define LOG_FILE "../log/c-hat.log"

FILE *pLogFile;
time_t rawtime;
struct tm * timeinfo;

int open_log_file();
void close_log_file();
void log_msg(const char * format, ...);

#endif //C_HAT_LOGGER_H
