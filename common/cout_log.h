#pragma once
#include "spdlog/spdlog.h"
#include <sstream>

// https://github.com/artemyv/cmake_spdlog_example/blob/main/src/logger.h
#define SPDLOG_LOGGER_STREAM(log, lvl) \
    log && log->should_log(lvl) &&     \
        LogStream(log, lvl) ==         \
            LogLine()

class LogLine
{
    std::ostringstream m_ss;

public:
    LogLine() {}
    template <typename T>
    LogLine &operator<<(const T &t)
    {
        m_ss << t;
        return *this;
    }
    std::string str() const { return m_ss.str(); }
};

class LogStream
{
    std::shared_ptr<spdlog::logger> m_log;
    spdlog::level::level_enum m_lvl;
    spdlog::source_loc m_loc;

public:
    LogStream(std::shared_ptr<spdlog::logger> log, spdlog::level::level_enum lvl) : m_log{log}, m_lvl{lvl} {}
    bool operator==(const LogLine &line)
    {
        m_log->log(m_lvl, "{}", line.str());
        return true;
    }
};

// specific log implementation macros
static struct cout_logdebug
{
    template <typename T>
    cout_logdebug &operator<<(const T &t)
    {
        return *this;
    }
    //    template <class _Traits>
    //    std::basic_ostream<char, _Traits> &operator<<(std::basic_ostream<char, _Traits> &__out, const char *__s)
    //    {
    //        return os;
    //    }
} CLOGDBUG;
#define ERROR spdlog::level::err
#define LOG(x) SPDLOG_LOGGER_STREAM(spdlog::default_logger(), x)
#define CLOGINFO SPDLOG_LOGGER_STREAM(spdlog::default_logger(), spdlog::level::info)
#define CLOGERR SPDLOG_LOGGER_STREAM(spdlog::default_logger(), spdlog::level::err)
