#ifndef __Logger_Include_H_
#define __Logger_Include_H_

#include "devices/CGA.h"
#include "lib/Semaphore.h"

class Logger {
private:
    Logger(const Logger& copy) = delete;

    char* name;
    static bool kout_enabled;
    static bool serial_enabled;

    // TODO: Don't mix logs
    static const Semaphore sem;

    void log(char* message, CGA::color col);

public:
    Logger(char* name) : name(name) {}

    enum LogLevel {
        TRACE = 0,
        DEBUG = 1,
        ERROR = 2,
        INFO = 3
    };
    static LogLevel level;

    void trace(char* message);
    void debug(char* message);
    void error(char* message);
    void info(char* message);

    // TODO: Make level change accessible over menu
    static void set_level(LogLevel level) {
        Logger::level = level;
    }

    static void enable_kout() {
        Logger::kout_enabled = true;
    }
    static void disable_kout() {
        Logger::kout_enabled = false;
    }
    static void enable_serial() {
        Logger::serial_enabled = true;
    }
    static void disable_serial() {
        Logger::serial_enabled = false;
    }
};

#endif
