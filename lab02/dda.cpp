/*
 * Program: DDA (Digital Differential Analyzer) Line Drawing Algorithm
 * Course : CSE444 - Computer Graphics & Animation Lab
 *
 * Idea:
 *   A line moves a fixed amount in x and a fixed amount in y at every step.
 *   Work out those two small amounts (xInc, yInc), start at the first end
 *   point, and keep adding them. Round each position to the nearest pixel.
 *
 * Compile and run:
 *   g++ dda.cpp -o dda.exe -lfreeglut -lopengl32 -lglu32
 *   .\dda.exe
 */

#include <GL/glut.h>
#include <algorithm> // std::max
#include <cmath>     // std::round
#include <cstdlib>   // std::abs


// ---------------------------------------------------------------
// drawLineDDA(): draws a line from (x1, y1) to (x2, y2).
//
//   1. dx = x2 - x1,  dy = y2 - y1
//   2. steps = the larger of |dx| and |dy|
//      (one pixel per step along the longer direction, so no gaps)
//   3. xInc = dx / steps,  yInc = dy / steps
//   4. Repeat steps + 1 times: plot (round(x), round(y)), then
//      x += xInc and y += yInc
// ---------------------------------------------------------------
void drawLineDDA(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;

    int steps = std::max(std::abs(dx), std::abs(dy));

    // Same start and end point: only one pixel to draw
    if (steps == 0) {
        glBegin(GL_POINTS);
            glVertex2i(x1, y1);
        glEnd();
        return;
    }

    float xInc = dx / (float)steps;   // how far x moves each step
    float yInc = dy / (float)steps;   // how far y moves each step

    float x = x1;
    float y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++) {
        glVertex2i((int)std::round(x), (int)std::round(y));   // nearest pixel
        x += xInc;
        y += yInc;
    }
    glEnd();
}


// ---------------------------------------------------------------
// display(): draws one test line for every kind of slope.
// ---------------------------------------------------------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(2.0);

    glColor3f(1.0, 0.0, 0.0);           // red    : gentle slope  (|m| < 1)
    drawLineDDA(50, 50, 450, 200);

    glColor3f(0.0, 0.6, 0.0);           // green  : steep slope   (|m| > 1)
    drawLineDDA(100, 50, 200, 450);

    glColor3f(0.0, 0.0, 1.0);           // blue   : negative slope
    drawLineDDA(50, 450, 450, 300);

    glColor3f(1.0, 0.5, 0.0);           // orange : horizontal line (m = 0)
    drawLineDDA(50, 250, 450, 250);

    glColor3f(0.6, 0.0, 0.6);           // purple : vertical line (m undefined)
    drawLineDDA(400, 50, 400, 450);

    glColor3f(0.0, 0.0, 0.0);           // black  : drawn right-to-left
    drawLineDDA(450, 100, 50, 400);

    glFlush();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("DDA Line Drawing Algorithm");

    glClearColor(1.0, 1.0, 1.0, 1.0);   // white background
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 500, 0, 500);         // (0,0) bottom-left, (500,500) top-right

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
