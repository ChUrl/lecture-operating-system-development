#include "user/lib/Logger.h"
#include "kernel/Globals.h"

SpinLock Logger::sem;
bool Logger::kout_enabled = true;
bool Logger::serial_enabled = true;

Logger::LogLevel Logger::level = Logger::ERROR;

constexpr const char* ansi_red = "\033[1;31m";
constexpr const char* ansi_green = "\033[1;32m";
constexpr const char* ansi_yellow = "\033[1;33m";
constexpr const char* ansi_blue = "\033[1;34m";
constexpr const char* ansi_magenta = "\033[1;35m";
constexpr const char* ansi_cyan = "\033[1;36m";
constexpr const char* ansi_white = "\033[1;37m";
constexpr const char* ansi_default = "\033[0;39m ";

void Logger::log(const char* message, CGA::color col) const {
    if (Logger::kout_enabled) {
        CGA::color old_col = kout.color_fg;
        kout << fgc(col)
             << Logger::level_to_string(this->current_message_level) << "::"
             << message << fgc(old_col);
        kout.flush();  // Don't add newline, Logger already does that
    }
    if (Logger::serial_enabled) {
        switch (col) {
        case CGA::WHITE:
            serial.write(ansi_white);
            break;
        case CGA::LIGHT_MAGENTA:
            serial.write(ansi_magenta);
            break;
        case CGA::LIGHT_RED:
            serial.write(ansi_red);
            break;
        case CGA::LIGHT_BLUE:
            serial.write(ansi_blue);
            break;
        default:
            serial.write(ansi_default);
        }
        serial.write(Logger::level_to_string(this->current_message_level));
        serial.write(":: ");
        serial.write(message);
        serial.write('\r');
        // serial.write("\r\n");
    }
}

void Logger::flush() {
    this->buffer[this->pos] = '\0';

    switch (this->current_message_level) {
    case Logger::TRACE:
        this->trace(&buffer);
        break;
    case Logger::DEBUG:
        this->debug(&buffer);
        break;
    case Logger::ERROR:
        this->error(&buffer);
        break;
    case Logger::INFO:
        this->info(&buffer);
        break;
    }

    this->current_message_level = Logger::INFO;
    this->pos = 0;
    Logger::unlock();
}

void Logger::trace(const char* message) const {
    if (Logger::level <= Logger::TRACE) {
        this->log(message, CGA::WHITE);
    }
}
void Logger::trace(const bse::string& message) const {
    trace((const char*)message);
}

void Logger::debug(const char* message) const {
    if (Logger::level <= Logger::DEBUG) {
        this->log(message, CGA::LIGHT_MAGENTA);
    }
}
void Logger::debug(const bse::string& message) const {
    debug((const char*)message);
}

void Logger::error(const char* message) const {
    if (Logger::level <= Logger::ERROR) {
        this->log(message, CGA::LIGHT_RED);
    }
}
void Logger::error(const bse::string& message) const {
    error((const char*)message);
}

void Logger::info(const char* message) const {
    if (Logger::level <= Logger::INFO) {
        this->log(message, CGA::LIGHT_BLUE);
    }
}
void Logger::info(const bse::string& message) const {
    info((const char*)message);
}

// Manipulatoren
Logger& TRACE(Logger& log) {
    log.current_message_level = Logger::TRACE;
    return log;
}

Logger& DEBUG(Logger& log) {
    log.current_message_level = Logger::DEBUG;
    return log;
}

Logger& ERROR(Logger& log) {
    log.current_message_level = Logger::ERROR;
    return log;
}

Logger& INFO(Logger& log) {
    log.current_message_level = Logger::INFO;
    return log;
}
