#include "Log.h"
#include <stdio.h>

/*Open the file pointer for writing*/
void initLog()
{
    FILE *logfp = fopen("Log.txt", "w+");
    fprintf(logfp, "****Start of Logs****\n");
    fclose(logfp);

}

/*Log the message to the file using the ported libfat32 library.*/
void logMessage(char* message)
{
    FILE *logfp = fopen("Log.txt", "a");
    fprintf(logfp, message);
    fprintf(logfp, "\n");
    fclose(logfp);
}

