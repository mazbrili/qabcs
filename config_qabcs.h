#ifndef CONFIG_H
#define CONFIG_H


#define QABCS_VERSION "1.0.0"
#define QABCS_DATEBUILD "27.10.2018"

#if defined(__WIN32__)
#define GLOBAL_PATH_USERDATA QString(".")
#else
#define GLOBAL_PATH_USERDATA QString("/usr/share/qabcs")
#endif


#endif // CONFIG_H
