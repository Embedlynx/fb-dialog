fb-dialog
===========================
Simple utility to display progress bar with a few buttons and read events from the touch screen

Uses libts for reading and handling touchscreen events.

Before running you need to do `export TSLIB_CALIBFILE=[path_to_calibrationfile]` in order for the touch events to be handleled correctly.

# Usage: fb-dialog
* --gauge [width,text] -- Display progress bar that has title `[text]` and is `[width]` pixels wide. Reads progress from stdin just like to the `dialog` tool.
* --message [message] -- Display a message box with text `[message]`
* --confirm [button1-text, button2-text] -- Display two confirmation buttons with text and wait for touch events from touchscreen/libts. After a button is pressed the index of the pressed button (0 or 1) is printed to stdout.
