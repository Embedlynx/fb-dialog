#ifndef _PIPEUTILS_H
#define _PIPEUTILS_H

FILE* call_command(const char *cmd);
int poll_and_read(FILE *f, char *data, int size);

#endif /* _PIPEUTILS_H */
