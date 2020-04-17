#ifndef _UTMPLIB_H
#define _UTMPLIB_H

extern int utmp_open(char *filename);

extern struct utmp *utmp_next();

extern void utmp_close();

#endif