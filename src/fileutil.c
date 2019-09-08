#include "../include/fileutil.h"
#include <unistd.h>
#include <errno.h>


int readline(int fd, char* buffer, int  nbytes) {
    /* Read a line from a file. Will stop if the line is
    ** longer than `buffer`.
    ** Note: Taken from UNIX SYSTEMS PROGRAMMING.
    ** @params:
    **  - fd: The file descriptor to read.
    **  - buffer: To write to.
    **  - nbytes: Length of the buffer.
    */
    int numread = 0;
    int return_value;

    while (numread < nbytes - 1) {
        return_value = read(fd, buffer + numread, 1);

        if (is_interrupted(return_value)) {
            continue;
        }

        if (is_empty_read(return_value, numread)) {
            return 0;
        }

        // If we get here, at least one byte has been read and we reached EOF before CRLF.
        if (is_read_done(return_value)) {
            break;
        }

        if (return_value == READ_FAILURE) {
            return READ_FAILURE;
        }

        if (buffer[numread - 1] == '\n') {
            buffer[numread] = '\0';
            return numread;
        }
    }
    errno = EINVAL;
    return READ_FAILURE;
}


static int is_interrupted(int read_return_value) {
    return errno == EINTR && read_return_value == -1;
}


static int is_empty_read(int read_return_value, int bytes_read) {
    return read_return_value == 0 && bytes_read == 0;
}


static int is_read_done(int read_return_value) {
    return read_return_value == 0;
}
