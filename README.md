Name: Joseph Chang

VUNetID: changj6

VU Email: joseph.chang@vanderbilt.edu

This is my CS 3258 code repository

Assignment #1:
  For this assignment, we have to construct a command line interpreter (CLI) that interpreted commands entered from a keyboard to read in from a file. The commands to implement were Move, Draw, Color, and Read. The first three all accepted three integer parameters wheras the last command accepted a file name. If an unknown command or no command at all was given, the program exitted. 
  If there was a read command, we limited the level of nested read commands to 6. If there were more than 6 nested read levels, the program promped the user that the limit had been reached. To implement the 6 nested read levels, 6 separate ifstrems were used for each level. If the file that the user inputted did not exist, the program prompted the user that no file was found.
  If there was a non-read command, the three parameters would be checked to see if they were valid parameters. If not all parameters were supplied, the missing parameters would be defaulted to the zero value. If the parameters given were characters when the parameters were supposed to be numerical, an error was shown. Comments were also ignored.
  
  To run the program, the program must be compiled and built. Once complete, there are 4 test files in the scripts folder. These tests are to be copied to the executable file. Once the program runs, if will promp the user for a command. "Read (test-file)" will run the program using the test file chosen.

Assignment $5:
  For this assignment, the rendering of the script for this project, which is called project5test.cli in the script folder, took 717 milliseconds to render.
  The tiff image corresponding to the script is reflect.tiff, which is in the images folder.