
/*  This program is a modified version of the program "image.c" 
 *  appearing in _The OpenGL Programming Guide_ (red book).
 *  It demonstrates drawing pixels and shows the effect
 *  of glDrawPixels() and glCopyPixels().
 *  Interaction: moving the mouse while pressing the mouse button
 *  will copy the image in the lower-left corner of the window
 *  to the mouse position.
 *  There is no attempt to prevent you from drawing over the original
 *  image.
 *
 *  IMPORTANT: The window with the image must be the active window, i.e.,
 *  it must be clicked or the mouse must be over it, for input to occur.
 *  The result will be shown in the window you started the program from, 
 *  but don't make the mistake of typing in this window: it won't work.
 *
 */

#include <GL/glut.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <iostream>
#include <fstream>
#include <string>

/*	Create checkerboard image	*/
#define    checkImageWidth 1024
#define    checkImageHeight 1024
GLubyte checkImage[checkImageHeight][checkImageWidth][3];

static GLint height;

void
makeCheckImage(void) {
    int i, j, c;

    for (i = 0; i < checkImageHeight; i++) {
        for (j = 0; j < checkImageWidth; j++) {
            c = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) * 255;
            checkImage[i][j][0] = (GLubyte) c;
            checkImage[i][j][1] = (GLubyte) c;
            checkImage[i][j][2] = (GLubyte) c;
        }
    }
}

/*
 * Function name: init
 * Description:   Operations that only need be performed once for rendering
 *                are done here.
 * Arguments:	  none
 * Globals:       none
 * Returns:	  void
 */
void
init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    makeCheckImage();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

/*
 * Function name: display
 * Description:   The glut display callback, operations to render and 
 *                re-render are here.
 * Arguments:	  none
 * Globals:       none
 * Returns:	  void
 */
void
display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2i(0, 0);
    glDrawPixels(checkImageWidth, checkImageHeight, GL_RGB,
                 GL_UNSIGNED_BYTE, checkImage);
    glFlush();
}

/*
 * Function name: reshape
 * Description:   glut reshape callback, what actions to take when the 
 *                window is reshaped.
 * Arguments:	  w - the new width
 *                h - the new height
 * Globals:       none
 * Returns:	  void
 */
void
reshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    height = (GLint) h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*
 * Function name: motion
 * Description:   glut mouse motion callback, copies the image.
 * Arguments:	  x - x position of mouse
 *                y - y positions of mouse
 * Globals:	  none
 * Returns:	  void
 */
void
motion(int x, int y) {
    static GLint screeny;

    screeny = height - (GLint) y;
    glRasterPos2i(x, screeny);
    glCopyPixels(0, 0, checkImageWidth, checkImageHeight, GL_COLOR);
    glFlush();
}

/*
 * Function name: mouse
 * Description:   glut mouse callback, draws lines on the image.
                  First left mouse click puts red dot at mouse location
                  Subsequent right mouse clicks draw a red line from that
                  location to current mouse location. Second left mouse
                  click resets routine and redraws the screen.
 * Arguments:	  button - which button (left, right, middle) is being pressed
                  press  - is it down?
                  x - x position of mouse
 *                y - y position of mouse (note this is from upper left corner)
 * Globals:	  none
 * Returns:	  void
 */
void
mouse(int button, int press, int x, int y) {
    static int state, start_x, start_y;

    GLint viewport[4];
    int height;

    printf("Mouse: %d %d\n", x, y);
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (press == GLUT_DOWN) {
                if (!state) {
                    glGetIntegerv(GL_VIEWPORT, viewport);
                    height = abs(viewport[3] - viewport[1]);
                    glColor3f(1.0, 0.0, 0.0);
                    glBegin(GL_POINTS);
                    glVertex2i(x, height - y);
                    glEnd();
                    glFlush();
                    state = 1;
                    start_x = x;
                    start_y = y;
                } else {
                    state = 0;
                    display();
                }
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if (press == GLUT_DOWN) {
                glGetIntegerv(GL_VIEWPORT, viewport);
                height = abs(viewport[3] - viewport[1]);
                glColor3f(1.0, 0.0, 0.0);
                glBegin(GL_LINES);
                glVertex2i(start_x, height - start_y);
                glVertex2i(x, height - y);
                glEnd();
                glFlush();
            }
            break;
    }
}

/*
 * Function name: main_loop
 * Description:   Process the line of input
 * Arguments:	  line - the line of input
 * Globals:       none
 * Returns:	  void
 */
void
main_loop(char line[], bool original, int readVersion) {
    /* PUT YOUR CLI CODE HERE! */
    char *command;
    char *tempParam;
    float numericalParam[32];
    char *stringParam[32];

    /* List of Known Commands */
    char *move = "Move";
    char *draw = "Draw";
    char *color = "Color";
    char *read = "Read";

    /* Flags for Parameters */
    int numParam = 0;
    bool validNumber = true;
    bool decimalUndetected = true;
    bool numerical = false;

    /* Extracting command */
    command = strtok(line, " ,\t");
    if (stricmp(command, move) == 0) {
        numParam = 3;
        numerical = true;
        command = move;
    } else if (stricmp(command, draw) == 0) {
        numParam = 3;
        numerical = true;
        command = draw;
    } else if (stricmp(command, color) == 0) {
        numParam = 3;
        numerical = true;
        command = color;
    } else if (stricmp(command, read) == 0) {
        numParam = 1;
        numerical = false;
        command = read;
    } else if (line[0] == '\0') {
        printf("No command was given\n");
        printf("Exiting...\n");
        exit(0);
    } else {
        printf("The given command is unknown\n");
        printf("Exiting...\n");
        exit(0);
    }

    /* Processing a Read Command */
    if (command == read) {
        ++readVersion;
        char* returnLine;
        char* fileName = strtok(nullptr, " ,");

        /* First Level of Read Commands */
        if (readVersion == 1) {
            std::ifstream file1(fileName, std::ifstream::in);
            if (file1.fail()) {
                printf("File does not exist\n");
            } else {
                file1.getline(returnLine, 1024);
                while (file1) {
                    main_loop(returnLine, false, 1);
                    file1.getline(returnLine, 1024);
                }
                printf("CLI> ");
            }
            file1.close();

        } else if (readVersion == 2) {
            /* Second Level of Read Commands */
            std::ifstream file2(fileName, std::ifstream::in);
            std::string myLine;
            if (file2.fail()) {
                printf("File does not exist\n");
            } else {
                getline(file2, myLine);
                while (file2) {
                    char* secondPointer = new char[myLine.size() + 1];
                    std::copy(myLine.begin(), myLine.end(), secondPointer);
                    secondPointer[myLine.size()] = '\0';
                    main_loop(secondPointer, false, 2);
                    getline(file2, myLine);
                    delete[] secondPointer;
                }
            }
            file2.close();

        } else if (readVersion == 3) {
            /* Third Level of Read Commands */
            std::ifstream file3(fileName, std::ifstream::in);
            std::string myLine;
            if (file3.fail()) {
                printf("File does not exist\n");
            } else {
                getline(file3, myLine);
                while (file3) {
                    char* thirdPointer = new char[myLine.size() + 1];
                    std::copy(myLine.begin(), myLine.end(), thirdPointer);
                    thirdPointer[myLine.size()] = '\0';
                    main_loop(thirdPointer, false, 3);
                    getline(file3, myLine);
                    delete[] thirdPointer;
                }
            }
            file3.close();

        } else if (readVersion == 4) {
            /* Fourth Level of Read Commands */
            std::ifstream file4(fileName, std::ifstream::in);
            std::string myLine;
            if (file4.fail()) {
                printf("File does not exist\n");
            } else {
                getline(file4, myLine);
                while (file4) {
                    char* fourthPointer = new char[myLine.size() + 1];
                    std::copy(myLine.begin(), myLine.end(), fourthPointer);
                    fourthPointer[myLine.size()] = '\0';
                    main_loop(fourthPointer, false, 4);
                    getline(file4, myLine);
                    delete[] fourthPointer;
                }
            }
            file4.close();

        } else if (readVersion == 5) {
            /* Fifth Level of Read Commands */
            std::ifstream file5(fileName);
            std::string fifthLine;
            if (file5.fail()) {
                printf("File does not exist\n");
            } else {
                getline(file5, fifthLine);
                while (file5) {
                    char* fifthPointer = new char[fifthLine.size() + 1];
                    std::copy(fifthLine.begin(), fifthLine.end(), fifthPointer);
                    fifthPointer[fifthLine.size()] = '\0';
                    main_loop(fifthPointer, false, 5);
                    getline(file5, fifthLine);
                    delete[] fifthPointer;
                }
            }
            file5.close();

        } else if (readVersion == 6) {
            /* Sixth Level of Read Commands */
            std::ifstream file6(fileName, std::ifstream::in);
            std::string myLine;
            if (file6.fail()) {
                printf("File does not exist\n");
            } else {
                file6.getline(returnLine, 1024);
                while (file6) {
                    main_loop(returnLine, false, 6);
                    file6.getline(returnLine, 1024);
                }
            }
            file6.close();

        } else {
            /* Since only 6 levels are allowed, all other read commands are not processed */
            printf("The limit in the number of nested files (6) has been reached\n");
            return;
        }
    } else {
        /* Processing a Non-Read Command */
        printf("In %s: ", command);
        for (int i = 0; i < numParam; ++i) {
            tempParam = strtok(nullptr, " ,");
            if (!numerical) {
                stringParam[i] = tempParam;
            } else {
                if (tempParam == nullptr) {
                    numericalParam[i] = 0;
                } else if (tempParam[0] == '#') {
                    for (int j = i; i < numParam; ++i) {
                        numericalParam[j] = 0;
                    }
                    break;
                } else {
                    for (int j = 0; j < (int) strlen(tempParam); ++j) {
                        if (j == 0 && tempParam[j] == '-') {}
                        else if (decimalUndetected && tempParam[j] == '.') {
                            decimalUndetected = false;
                        } else if (!isdigit(tempParam[j]) || tempParam[j] == '.') {
                            validNumber = false;
                        }
                    }
                    if (validNumber) {
                        numericalParam[i] = strtof(tempParam, nullptr);\
                        decimalUndetected = true;
                    } else {
                        printf("Error: Invalid Parameters\n");
                        return;
                    }
                }
            }
        }

        /* Printing Parameter Format */
        for (int i = 0; i < numParam; ++i) {
            if (numerical) {
                printf("Parameter #%d: %f\t", i + 1, numericalParam[i]);
            } else {
                printf("Parameter #%d: %s\t", i + 1, stringParam[i]);
            }
        }
        printf("\n");
    }


    if (line == nullptr) {
        printf("Exiting...\n");
        exit(0);
    }

    if (original) {
        printf("CLI> ");
    }
    fflush(stdout);

    return;
}

#define MAXLINE 1024

/*  ASCII Values for return, backspace, delete, escape and ctrl-d */
#define CR   13
#define BS    8
#define DEL 127
#define ESC  27
#define EOT   4

/*
 * Function name: read_next_command
 * Description:   The glut callback function which is called when a key
 *                is pressed. ESC redraws the screen, ctrl-d exits.
 * Arguments:	  key - The ASCII value of the key pressed.
 *                x   - x position of the mouse (window-relative)
 *                y   - y position of the mouse (window-relative)
 * Globals:       none
 * Returns:	  void
 */
void
read_next_command(unsigned char key, int x, int y) {
    static char line[MAXLINE];
    static int count;

    if (count >= MAXLINE - 1) {
        printf("Error: Maximum line length exceeded. Discarded input.\n");
        count = 0;
        return;
    }

    putchar(key);
    fflush(stdout);  /* Immediate output (gratification) */

    if (key != CR && key != BS && key != DEL && key != ESC && key != EOT) {
        line[count] = key;
        count++;
    } else if (key == BS || key == DEL) {
        if (count == 0)
            return;

        fflush(stdout);
        count--;
    } else if (key == CR) {
        printf("\n");
        line[count] = '\0';
        count = 0;
        main_loop(line, true, 0);
    } else if (key == ESC) {
        glutPostRedisplay();
    } else {
        printf("Exiting...\n");
        exit(0);
    }
}

/*
 * Function name: main
 * Description:   Set up GL window and render
 * Arguments:	  command line arguments.
 * Globals:       none
 * Returns:	  void
 */
int
main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(250, 250);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(read_next_command);
    glutMouseFunc(mouse);
    //   glutMotionFunc(motion);

    printf("CLI> ");
    fflush(stdout);

    glutMainLoop();
    return 0;
}
