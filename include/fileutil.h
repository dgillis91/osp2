#ifndef _FILEUTIL_H
#define _FILEUTIL_H

#define READ_FAILURE -1


int readline(int, char*, int);


static int is_interrupted(int);


static int is_empty_read(int, int);


static int is_read_done(int);

#endif
