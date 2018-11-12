#ifndef CONFIG_H
#define CONFIG_H


#define QABCS_VERSION "1.0.0"
#define QABCS_DATEBUILD "12.11.2018"

#if !defined(GLOBAL_PATH_USERDATA)
#if defined(__WIN32__)
#define GLOBAL_PATH_USERDATA "."
#else
#define GLOBAL_PATH_USERDATA "/usr/share/qabcs"
#endif
#endif


#endif // CONFIG_H
