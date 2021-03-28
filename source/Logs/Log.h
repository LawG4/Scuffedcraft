/**
 * So because we don't have step by step through debugging, the nesr best alternative is to have some kind of logging system 
 * There isn't logging built into devkitppc, so instead we have to make one using the libfat32 library that has been ported 
 * However I imagine my ability to write a fast logging library won't be amazing, so instead I would like to be able to be
 * able to switch off all of the logging functionalities when we're doing a performance build
 * 
 * To do a performance build simply switch on the performance define here and all logging functions will return automatically
 * When doing a release build we will remove the logging functionality entirely to reduce the slight overhead as much as 
 * possible
 * 
 * I aim to have a file IO section, however the implementation of that will be very different from the logging library
 * so I will be keeping them seperate for now.  
 */ 
#ifndef LOG_INCLUDE
#define LOG_INCLUDE

/**
 * Start every log function with the LOG_FN define, so that if we are in performance mode we can simply return back 
 * immediatly 
 */ 
/*#define LOG_PERF_MODE*/
#ifdef LOG_PERF_MODE
#define LOG_FN return;
#else
#define LOG_FUN
#endif

/**
 * Open the logging file and clear it
 */
void initLog();


/**
 * Write a simple message to the log text file
 * @param message The message to write to the log file
 * @note Not the finalised form, this is mainly because I need to debug a feature
 */ 
void logMessage(char* message);


#endif