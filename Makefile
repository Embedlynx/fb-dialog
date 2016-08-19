# Run make in project dir

CFLAGS += -lts -Wall

fb-dialog: main.c fb_utils.c testutils.c pipe_utils.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	-rm fb-dialog
