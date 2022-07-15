#include "user/lib/Logger.h"
#include "kernel/Globals.h"

bool Logger::kout_enabled = true;
bool Logger::serial_enabled = true;
const Semaphore Logger::sem = Semaphore(1);

Logger::LogLevel Logger::level = Logger::TRACE;

void Logger::log(char* message, CGA::color col) {
    if (Logger::kout_enabled) {
        CGA::color old_col = kout.color_fg;
        kout << fgc(col) << this->name << " :: " << message << fgc(old_col) << endl;
    }
    if (Logger::serial_enabled) {
        serial.write(this->name);
        serial.write(" :: ");
        serial.write(message);
        serial.write("\r\n");
    }
}

void Logger::trace(char* message) {
    if (Logger::level <= Logger::TRACE) {
        this->log(message, CGA::GREEN);
    }
}

void Logger::debug(char* message) {
    if (Logger::level <= Logger::DEBUG) {
        this->log(message, CGA::CYAN);
    }
}

void Logger::error(char* message) {
    if (Logger::level <= Logger::ERROR) {
        this->log(message, CGA::RED);
    }
}

void Logger::info(char* message) {
    if (Logger::level <= Logger::TRACE) {
        this->log(message, CGA::LIGHT_GREY);
    }
}
