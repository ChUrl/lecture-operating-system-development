#ifndef __Logger_Include_H_
#define __Logger_Include_H_

#include "devices/CGA.h"
#include "lib/OutStream.h"
// #include "lib/Semaphore.h"

class Logger : public OutStream {
private:
    Logger(const Logger& copy) = delete;

    char* name;
    static bool kout_enabled;
    static bool serial_enabled;

    // TODO: Don't mix logs
    // static const Semaphore sem;

    void log(char* message, CGA::color col) const;

public:
    Logger(char* name) : name(name) {}

    enum LogLevel {
        TRACE,
        DEBUG,
        ERROR,
        INFO
    };
    static LogLevel level;
    LogLevel current_message_level = Logger::INFO;  // Use this to log with manipulators

    void flush() override;

    void trace(char* message) const;
    void debug(char* message) const;
    void error(char* message) const;
    void info(char* message) const;

    // TODO: Make level change accessible over menu
    static void set_level(LogLevel level) {
        Logger::level = level;
    }

    static char* level_to_string(LogLevel level) {
        switch (level) {
        case Logger::TRACE:
            return "TRACE";
        case Logger::DEBUG:
            return "DEBUG";
        case Logger::ERROR:
            return "ERROR";
        case Logger::INFO:
            return "INFO";
        }
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

// Manipulatoren
Logger& TRACE(Logger& log);
Logger& DEBUG(Logger& log);
Logger& ERROR(Logger& log);
Logger& INFO(Logger& log);

#endif
