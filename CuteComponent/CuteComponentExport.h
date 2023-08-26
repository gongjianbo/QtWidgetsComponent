#pragma once
#include <QtCore/qglobal.h>

#ifdef CuteComponent_Library
#define Cute_API Q_DECL_EXPORT
#else
#define Cute_API Q_DECL_IMPORT
#endif
