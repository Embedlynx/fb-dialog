#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>

FILE* call_command(const char *cmd)
{
    int fd, flags;

    FILE *f = popen(cmd, "r");
    fd = fileno(f);

    flags = fcntl(fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flags);

    return f;
} 

int poll_and_read(FILE *f, char *data, int size)
{
    int fd = fileno(f);
    struct pollfd pfd;
    int poll_time = 2000;
    pfd.events = POLLIN;
    pfd.fd = fd;
    int status = 1;

    status = poll(&pfd, 1, poll_time);

    if(status == -1 || status == 0) {
        printf("Couldn't poll returned : %d\n", status);
        return 0;
    } else {
        if(fgets(data, size, f) == NULL) {
            return 0;
        }
    }
    return status;
}
