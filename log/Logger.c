//
// Created by lothar on 26/03/2020.
//

#include "Logger.h"

int open_log_file()
{
    pLogFile = fopen(LOG_FILE, "a"); //open the file "LOG_FILE" in mode "a" : writes at the end of the file
    if(pLogFile == NULL){
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


void close_log_file(){
    fclose(pLogFile);
}

void log_msg(const char * format, ...){ //contains multiple variables and the number is unknown
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    fprintf(pLogFile,"%s ", asctime(timeinfo));

    va_list params;
    va_start(params, format); //initialise a structure to manipulate the parameters put after the first param
    vfprintf(pLogFile,format, params); //prints into pLogFile the string "format" and params identify a variable argument list
    va_end(params);
    fprintf(pLogFile, "\n"); //prints what's inside our logFile
    fflush(pLogFile); //allows to write any unwritten data in the output buffer
}