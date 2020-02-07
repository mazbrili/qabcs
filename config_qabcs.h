#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

#define QABCS_VERSION "1.0.3 dev"
#define QABCS_DATEBUILD "xx.xx.xxxx"

#if !defined(GLOBAL_PATH_USERDATA)
#if defined(__WIN32__)
#define GLOBAL_PATH_USERDATA "."
#else
#define GLOBAL_PATH_USERDATA "/usr/share/qabcs"
#endif
#endif

extern QString global_path_to_espeak;
extern QString global_path_to_play;

#endif // CONFIG_H
