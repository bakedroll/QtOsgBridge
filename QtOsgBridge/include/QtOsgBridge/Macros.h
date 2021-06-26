#pragma once

#include <osgHelper/LogManager.h>

#define OSGH_QLOG_DEBUG(msg) OSGH_LOG_DEBUG(std::string(msg.toLocal8Bit().constData()))
#define OSGH_QLOG_INFO(msg) OSGH_LOG_INFO(std::string(msg.toLocal8Bit().constData()))
#define OSGH_QLOG_WARN(msg) OSGH_LOG_WARN(std::string(msg.toLocal8Bit().constData()))
#define OSGH_QLOG_FATAL(msg) OSGH_LOG_FATAL(std::string(msg.toLocal8Bit().constData()))
