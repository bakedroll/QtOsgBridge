#pragma once

#include <utilsLib/Utils.h>

#define UTILS_QLOG_DEBUG(msg) UTILS_LOG_DEBUG(std::string(msg.toLocal8Bit().constData()))
#define UTILS_QLOG_INFO(msg) UTILS_LOG_INFO(std::string(msg.toLocal8Bit().constData()))
#define UTILS_QLOG_WARN(msg) UTILS_LOG_WARN(std::string(msg.toLocal8Bit().constData()))
#define UTILS_QLOG_FATAL(msg) UTILS_LOG_FATAL(std::string(msg.toLocal8Bit().constData()))
