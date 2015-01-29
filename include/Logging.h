#ifndef LOGGING_H
#define LOGGING_H

#include <cstdio>
#include <cstdarg>

/*

    Logging singleton
    -----------------
    This will keep all logs centralized.

    Note: not thread-safe.

    TODO: add option to save output to file

*/

#define LOG Logging::getInstance()

class Logging
{
    private:
        Logging() {}
        Logging(Logging const&);
        Logging &operator = (Logging const &copy);

        enum LogCode { LOG_MSG, LOG_ERROR, LOG_INFO };
        // Print message to console using unknown number of arguments
        static void log(const short type, const char *format, va_list args) {
            char msg[256];
            char msgType;
            vsnprintf(msg, 256, format, args);
            switch(type) {
                case LOG_ERROR:
                    msgType = '!';
                    break;
                case LOG_INFO:
                    msgType = '?';
                    break;
                default:
                    msgType = '+';
                    break;
            }
            // Output message to console
            std::printf("[%c] %s\n", msgType, msg);
/*
            // TODO: add a flag to output to file
            FILE * logFile;
            logFile = fopen ("log.txt","a+");
            if (logFile != NULL) {
                char buff[256];
                snprintf(buff, 256,"[%c] %s\n", msgType, msg);
                fputs (buff, logFile);
                fclose (logFile);
            }
*/
        }

    protected:
    public:
        // Return instance of Logging class
        static Logging *getInstance() {
            static Logging instance;
            return &instance;
        }
        // Print log message to console
        static void log(const char *msg, ...) {
            va_list args;
            va_start(args, msg);
            va_end(args);
            log(LOG_MSG, msg, args);
        }
        // Print error message to console
        static void error(const char *msg, ...) {
            va_list args;
            va_start(args, msg);
            va_end(args);
            log(LOG_ERROR, msg, args);
        }
        // Print info message to console
        static void info(const char *msg, ...) {
            va_list args;
            va_start(args, msg);
            va_end(args);
            log(LOG_INFO, msg, args);
        }
};

#endif // LOGGING_H
