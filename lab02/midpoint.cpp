/*
 * Program: Midpoint Line Drawing Algorithm
 * Course : CSE444 - Computer Graphics & Animation Lab
 *
 * Idea:
 *   Write the line as a function  F(x, y) = dy*x - dx*y + c
 *     F = 0  on the line
 *     F > 0  below the line
 *     F < 0  above the line
 *
 *   For a gentle line, after pixel (x, y) the next pixel is either
 *     E  = (x+1, y)      (East: same row)
 *     NE = (x+1, y+1)    (North-East: one row up)
 *   Test the point halfway between them, M = (x+1, y+0.5):
 *     M above the line (F(M) <= 0)  ->  line passes closer to E,  pick E
 *     M below the line (F(M) >  0)  ->  line passes closer to NE, pick NE
 *
 *   d holds 2*F(M) (doubled so it stays a whole number). After each choice
 *   d is updated by adding a constant: dE or dNE.
 *
 * Compile and run:
 *   g++ midpoint.cpp -o midpoint.exe -lfreeglut -lopengl32 -lglu32
 *   .\midpoint.exe
 */

#include <GL/glut.h>
#include <cstdlib>   // std::abs


// ---------------------------------------------------------------
// drawLineMidpoint(): draws a line from (x1, y1) to (x2, y2).
//
// Gentle line (|dx| >= |dy|):
//   d   = 2*dy - dx          (2 * F at the first midpoint)
//   dE  = 2*dy               (change in d after choosing E)
//   dNE = 2*(dy - dx)        (change in d after choosing NE)
//
// Steep line (|dy| > |dx|): same thing with x and y swapped.
// sx and sy (+1 or -1) handle lines going left or down.
// ---------------------------------------------------------------
void drawLineMidpoint(int x1, int y1, int x2, int y2) {
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);

    int sx = (x2 >= x1) ? 1 : -1;   // step direction in x
    int sy = (y2 >= y1) ? 1 : -1;   // step direction in y

    int x = x1;
    int y = y1;

    glBegin(GL_POINTS);

    if (dx >= dy) {
        // ---- Gentle line: x moves every step ----
        int d   = 2 * dy - dx;
        int dE  = 2 * dy;
        int dNE = 2 * (dy - dx);

        for (int i = 0; i <= dx; i++) {
            glVertex2i(x, y);
            x += sx;

            if (d <= 0) {
                d += dE;          // midpoint on/above line: choose E
            } else {
                y += sy;          // midpoint below line: choose NE
                d += dNE;
            }
        }
    } else {
        // ---- Steep line: y moves every step ----
        int d   = 2 * dx - dy;
        int dN  = 2 * dx;
        int dNE = 2 * (dx - dy);

        for (int i = 0; i <= dy; i++) {
            glVertex2i(x, y);
            y += sy;

            if (d <= 0) {
                d += dN;          // choose N  (x stays)
            } else {
                x += sx;          // choose NE (x moves)
                d += dNE;
            }
        }
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
    drawLineMidpoint(50, 50, 450, 200);

    glColor3f(0.0, 0.6, 0.0);           // green  : steep slope   (|m| > 1)
    drawLineMidpoint(100, 50, 200, 450);

    glColor3f(0.0, 0.0, 1.0);           // blue   : negative slope
    drawLineMidpoint(50, 450, 450, 300);

    glColor3f(1.0, 0.5, 0.0);           // orange : horizontal line (m = 0)
    drawLineMidpoint(50, 250, 450, 250);

    glColor3f(0.6, 0.0, 0.6);           // purple : vertical line (m undefined)
    drawLineMidpoint(400, 50, 400, 450);

    glColor3f(0.0, 0.0, 0.0);           // black  : drawn right-to-left
    drawLineMidpoint(450, 100, 50, 400);

    glFlush();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Midpoint Line Drawing Algorithm");

    glClearColor(1.0, 1.0, 1.0, 1.0);   // white background
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 500, 0, 500);         // (0,0) bottom-left, (500,500) top-right

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
