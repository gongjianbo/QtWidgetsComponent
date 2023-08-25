#pragma once
#include <QtCore/qglobal.h>

#ifdef QtComponent_Library
#define QtComponent_API Q_DECL_EXPORT
#else
#define QtComponent_API Q_DECL_IMPORT
#endif
