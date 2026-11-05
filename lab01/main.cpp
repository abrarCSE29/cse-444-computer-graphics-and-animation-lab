/*
 * Program: Drawing a Point in OpenGL
 * Course : CSE444 - Computer Graphics & Animation Lab
 *
 * What it does:
 *   Opens a 500 x 500 window and draws one red point in the centre.
 *
 * How to compile and run:
 *   g++ main.cpp -o main.exe -lfreeglut -lopengl32 -lglu32
 *   main.exe
 */

#include <GL/glut.h>   // GLUT: creates the window and includes OpenGL


// ---------------------------------------------------------------
// display(): GLUT calls this function whenever the window needs
// to be drawn. All drawing code goes here.
// ---------------------------------------------------------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT);   // Step 1: clear the screen with the background colour

    glColor3f(1.0, 0.0, 0.0);       // Step 2: choose drawing colour (Red, Green, Blue), each 0.0 to 1.0
    glPointSize(10.0);              // Step 3: set point size in pixels (default is 1, too small to see)

    glBegin(GL_POINTS);             // Step 4: start drawing points
        glVertex2i(250, 250);
        glVertex2i(150, 150);       //         the point's (x, y) position
    glEnd();                        //         stop drawing

    glFlush();                      // Step 5: send everything to the screen
}


// ---------------------------------------------------------------
// main(): sets up the window and starts the program.
// ---------------------------------------------------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);                        // start GLUT
    glutInitWindowSize(500, 500);                 // window size: 500 x 500 pixels
    glutCreateWindow("Point in OpenGL");          // create window with this title

    glClearColor(0.5, 0.2, 1.0, 1.0);             // background colour: white
    gluOrtho2D(0, 500, 0, 500);                   // coordinate system: x from 0 to 500, y from 0 to 500
                                                  // (0,0) is the bottom-left corner

    glutDisplayFunc(display);                     // tell GLUT which function draws the scene
    glutMainLoop();                               // keep the window open and running
    return 0;
}