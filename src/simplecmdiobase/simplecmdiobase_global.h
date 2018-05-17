#ifndef QTCMDIO_GLOBAL_GLOBAL_H
#define QTCMDIO_GLOBAL_GLOBAL_H

#include <QtCore/qglobal.h>

#ifndef QT_STATIC
#if defined(QT_BUILD_SIMPLE_CMDIO_LIB)
#  define SIMPLE_CMDIO_EXPORT Q_DECL_EXPORT
#else
#  define SIMPLE_CMDIO_EXPORT Q_DECL_IMPORT
#endif
#endif

#endif // QTCMDIO_GLOBAL_GLOBAL_H
