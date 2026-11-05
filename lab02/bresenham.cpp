/*
 * Program: Bresenham's Line Drawing Algorithm
 * Course : CSE444 - Computer Graphics & Animation Lab
 *
 * Idea:
 *   Step one pixel at a time along the longer direction (the "driving axis").
 *   At every step there are only two choices for the other coordinate:
 *   stay the same, or move by one. A whole-number "decision parameter" p
 *   tells us which pixel is closer to the true line. No floating point,
 *   no rounding: only integer addition and comparison.
 *
 * Compile and run:
 *   g++ bresenham.cpp -o bresenham.exe -lfreeglut -lopengl32 -lglu32
 *   .\bresenham.exe
 */

#include <GL/glut.h>
#include <cstdlib>   // std::abs


// ---------------------------------------------------------------
// drawLineBresenham(): draws a line from (x1, y1) to (x2, y2).
//
// Gentle line (|dx| >= |dy|), x moves every step:
//   p = 2*dy - dx                       (starting decision value)
//   each step: plot (x, y), then move x by one
//     if p <  0 : y stays,     p = p + 2*dy
//     if p >= 0 : y moves,     p = p + 2*dy - 2*dx
//
// Steep line (|dy| > |dx|): same thing with x and y swapped.
//
// sx and sy (+1 or -1) let the same code draw lines that go
// left, right, up or down.
// ---------------------------------------------------------------
void drawLineBresenham(int x1, int y1, int x2, int y2) {
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);

    int sx = (x2 >= x1) ? 1 : -1;   // step direction in x
    int sy = (y2 >= y1) ? 1 : -1;   // step direction in y

    int x = x1;
    int y = y1;

    glBegin(GL_POINTS);

    if (dx >= dy) {
        // ---- Gentle line: x is the driving axis ----
        int p = 2 * dy - dx;

        for (int i = 0; i <= dx; i++) {
            glVertex2i(x, y);
            x += sx;

            if (p < 0) {
                p += 2 * dy;              // next pixel: same row
            } else {
                y += sy;                  // next pixel: move one row
                p += 2 * dy - 2 * dx;
            }
        }
    } else {
        // ---- Steep line: y is the driving axis ----
        int p = 2 * dx - dy;

        for (int i = 0; i <= dy; i++) {
            glVertex2i(x, y);
            y += sy;

            if (p < 0) {
                p += 2 * dx;              // next pixel: same column
            } else {
                x += sx;                  // next pixel: move one column
                p += 2 * dx - 2 * dy;
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
    drawLineBresenham(50, 50, 450, 200);

    glColor3f(0.0, 0.6, 0.0);           // green  : steep slope   (|m| > 1)
    drawLineBresenham(100, 50, 200, 450);

    glColor3f(0.0, 0.0, 1.0);           // blue   : negative slope
    drawLineBresenham(50, 450, 450, 300);

    glColor3f(1.0, 0.5, 0.0);           // orange : horizontal line (m = 0)
    drawLineBresenham(50, 250, 450, 250);

    glColor3f(0.6, 0.0, 0.6);           // purple : vertical line (m undefined)
    drawLineBresenham(400, 50, 400, 450);

    glColor3f(0.0, 0.0, 0.0);           // black  : drawn right-to-left
    drawLineBresenham(450, 100, 50, 400);

    glFlush();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bresenham Line Drawing Algorithm");

    glClearColor(1.0, 1.0, 1.0, 1.0);   // white background
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 500, 0, 500);         // (0,0) bottom-left, (500,500) top-right

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
