#ifndef INCLUDE_UTIL_FILEDESCRIPTOR_H
#define INCLUDE_UTIL_FILEDESCRIPTOR_H

#include <fcntl.h>

off_t fdGetEnd(int fd);
off_t fdGetCur(int fd);
void fdReset(int fd);

#endif /* ifndef INCLUDE_UTIL_FILEDESCRIPTOR_H */
