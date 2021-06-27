#pragma once

#ifdef WIN32

#include <Windows.h>

namespace QtOsgBridge
{
  LONG CALLBACK unhandled_handler(EXCEPTION_POINTERS* e);
}

#endif