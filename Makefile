# Run make in project dir
build:
	gcc -Wall main.c fb_utils.c testutils.c pipe_utils.c -lts -o fb-dialog
clean:
	rm fb-dialog	
