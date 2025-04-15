#include "Utils/FileDescriptor.h"
#include <fcntl.h>
#include <unistd.h>

off_t fdGetCur(int fd) { return lseek(fd, 0, SEEK_CUR); }
off_t fdGetEnd(int fd) {
    off_t cur = fdGetCur(fd);
    off_t out = lseek(fd, 0, SEEK_END);
    lseek(fd, cur, SEEK_SET);
    return out;
}

void fdReset(int fd) { lseek(fd, 0, SEEK_SET); }
