#ifndef __Logger_Include_H_
#define __Logger_Include_H_

#include "devices/CGA.h"
#include "lib/OutStream.h"
#include "lib/SpinLock.h"

class Logger : public OutStream {
public:
    static Logger& instance() {
        static Logger log;
        return log;
    }

private:
    Logger() = default;
    Logger(const Logger& copy) = delete;
    void operator=(const Logger& copy) = delete;

    static bool kout_enabled;
    static bool serial_enabled;

    void log(char* message, CGA::color col) const;

    friend class NamedLogger;  // Allow NamedLogger to lock/unlock
    static SpinLock sem;
    static void lock() { Logger::sem.acquire(); }
    static void unlock() { Logger::sem.release(); }

public:
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

class NamedLogger {
private:
    char* name;

public:
    NamedLogger(char* name) : name(name) {}

    Logger& trace() {
        Logger::lock();
        return Logger::instance() << TRACE << name << "::";
    }

    Logger& debug() {
        Logger::lock();
        return Logger::instance() << DEBUG << name << "::";
    }

    Logger& error() {
        Logger::lock();
        return Logger::instance() << ERROR << name << "::";
    }

    Logger& info() {
        Logger::lock();
        return Logger::instance() << INFO << name << "::";
    }
};

#endif
