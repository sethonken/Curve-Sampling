#include "GL/freeglut.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;
#define RESET 1
#define LINECHANGE 2
#define MOVETRACER 3
#define MOVELINEPOINTS 4
#define subMenu20 5
#define subMenu10 6
#define subMenu5 7
#define subMenuN5 8
#define subMenuN10 9
#define subMenuN20 10

//global variables
int width = 600;
int height = 600;
int pointsClicked = 0;
int lines = 4; //starts out with 4 lines, can be changed to 3
int lineChange = 0; // even or odd for either 3 or 4 lines
int density = 140;
double step = 2.0 / density;
bool noTracing = false;
bool moveTracer = false;
bool currentlyTracing = false;
bool leftClicked = false;
bool printTrace = false;
bool moveLinePoints = false;
bool notMovingLines = true;
bool L1P1 = false;
bool L1P2 = false;
bool L2P1 = false;
bool L2P2 = false;
bool L3P1 = false;
bool L3P2 = false;
bool L4P1 = false;
bool L4P2 = false;
struct Point {
	double x;
	double y;
};
struct Line {
	Point point1;
	Point point2;
	double slope;
};
struct Square {
	Point point1;
	bool p1; //true = +, false = -
	Point point2;
	bool p2;
	Point point3;
	bool p3;
	Point point4;
	bool p4;
};
Line line1;
Line line2;
Line line3;
Line line4;
Point tracer;

//functions
static void init();
static void display();
double slope(double point1x, double point1y, double point2x, double point2y);
void processMenuEvents(int option);
void processSubMenuEvents(int option);

void createGLUTMenus() {
	//submenu for density
	int subMenu = glutCreateMenu(processSubMenuEvents);
	glutAddMenuEntry("+20", subMenu20);
	glutAddMenuEntry("+10", subMenu10);
	glutAddMenuEntry("+5", subMenu5);
	glutAddMenuEntry("-5", subMenuN5);
	glutAddMenuEntry("-10", subMenuN10);
	glutAddMenuEntry("-20", subMenuN20);

	int menu = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("reset", RESET);
	char buffer[30];
	int n = sprintf_s(buffer, "change density [%d]", density);
	glutAddSubMenu(buffer, subMenu);
	int l = 3;
	if (lines == 3)
		l = 4;
	char buffer2[30];
	int n2 = sprintf_s(buffer2, "change to %d lines", l);
	glutAddMenuEntry(buffer2, LINECHANGE);
	glutAddMenuEntry("allow moving extra point", MOVETRACER);
	glutAddMenuEntry("allow moving line points", MOVELINEPOINTS);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void processMenuEvents(int option) {
	switch (option) {
	case RESET:
		pointsClicked = 0;
		printTrace = false;
		printf("reset\n");
		createGLUTMenus();
		display();
		break;
	case LINECHANGE:
		if (lines == 4)
			lines = 3;
		else
			lines = 4;
		processMenuEvents(RESET);
		break;
	case MOVETRACER:
		moveTracer = true;
		break;
	case MOVELINEPOINTS:
		moveLinePoints = true;
		break;
	}
}

void processSubMenuEvents(int option) {
	switch (option) {
	case subMenu20:
		density += 20;
		break;
	case subMenu10:
		density += 10;
		break;
	case subMenu5:
		density += 5;
		break;
	case subMenuN5:
		density -= 5;
		break;
	case subMenuN10:
		density -= 10;
		break;
	case subMenuN20:
		density -= 20;
		break;
	}
	if (density < 5) {
		density = 5;
	}
	step = 2.0 / density;
	createGLUTMenus();
	display();
}

static void mouse_active_move(int x, int y) {
	double widthd = width;
	double heightd = height;
	double point1 = (x / widthd) * 2 - 1;
	double point2 = ((heightd - y) / heightd) * 2 - 1;
	double range = 0.15;

	if (currentlyTracing) {
		if (lines == 3 && pointsClicked > 5 || lines == 4 && pointsClicked > 7) {
			currentlyTracing = true;
			printTrace = true;
			tracer.x = (x / widthd) * 2 - 1;
			tracer.y = ((heightd - y) / heightd) * 2 - 1;
		}
		display();
		noTracing = false;
		return;
	}


	if (line1.point1.x - point1 < range && line1.point1.x - point1 > -range) {
		if (line1.point1.y - point2 < range && line1.point1.y - point2 > -range) { //move 1st point
			noTracing = true;
		}
	}
	else if (line1.point2.x - point1 < range && line1.point2.x - point1 > -range) {
		if (line1.point2.y - point2 < range && line1.point2.y - point2 > -range) { //move 2nd point
			noTracing = true;
		}
	}
	else if (line2.point1.x - point1 < range && line2.point1.x - point1 > -range) {
		if (line2.point1.y - point2 < range && line2.point1.y - point2 > -range) { //move 1st point
			noTracing = true;
		}
	}
	else if (line2.point2.x - point1 < range && line2.point2.x - point1 > -range) {
		if (line2.point2.y - point2 < range && line2.point2.y - point2 > -range) { //move 2nd point
			noTracing = true;
		}
	}
	else if (line3.point1.x - point1 < range && line3.point1.x - point1 > -range) {
		if (line3.point1.y - point2 < range && line3.point1.y - point2 > -range) { //move 1st point
			noTracing = true;
		}
	}
	else if (line3.point2.x - point1 < range && line3.point2.x - point1 > -range) {
		if (line3.point2.y - point2 < range && line3.point2.y - point2 > -range) { //move 2nd point
			noTracing = true;
		}
	}
	else if (line4.point1.x - point1 < range && line4.point1.x - point1 > -range) {
		if (line4.point1.y - point2 < range && line4.point1.y - point2 > -range) { //move 1st point
			noTracing = true;
		}
	}
	else if (line4.point2.x - point1 < range && line4.point2.x - point1 > -range) {
		if (line4.point2.y - point2 < range && line4.point2.y - point2 > -range) { //move 2nd point
			noTracing = true;
		}
	}
	else if (L1P1 || L1P2 || L2P1 || L2P2 || L3P1 || L3P2 || L4P1 || L4P2) {
		noTracing = true;
	}

	if (notMovingLines && moveTracer && leftClicked && !noTracing) {
		if (lines == 3 && pointsClicked > 5 || lines == 4 && pointsClicked > 7) {
			currentlyTracing = true;
			printTrace = true;
			tracer.x = (x / widthd) * 2 - 1;
			tracer.y = ((heightd - y) / heightd) * 2 - 1;
		}
		display();
		noTracing = false;
		return;
	}

	if (moveLinePoints) {
		if (!notMovingLines) {
			if (L1P1) {
				line1.point1.x = (x / widthd) * 2 - 1;
				line1.point1.y = ((heightd - y) / heightd) * 2 - 1;
				line1.slope = slope(line1.point1.x, line1.point1.y, line1.point2.x, line1.point2.y);
			}
			else if (L1P2) {
				line1.point2.x = (x / widthd) * 2 - 1;
				line1.point2.y = ((heightd - y) / heightd) * 2 - 1;
				line1.slope = slope(line1.point1.x, line1.point1.y, line1.point2.x, line1.point2.y);
			}
			else if (L2P1) {
				line2.point1.x = (x / widthd) * 2 - 1;
				line2.point1.y = ((heightd - y) / heightd) * 2 - 1;
				line2.slope = slope(line2.point1.x, line2.point1.y, line2.point2.x, line2.point2.y);
			}
			else if (L2P2) {
				line2.point2.x = (x / widthd) * 2 - 1;
				line2.point2.y = ((heightd - y) / heightd) * 2 - 1;
				line2.slope = slope(line2.point1.x, line2.point1.y, line2.point2.x, line2.point2.y);
			}
			else if (L3P1) {
				line3.point1.x = (x / widthd) * 2 - 1;
				line3.point1.y = ((heightd - y) / heightd) * 2 - 1;
				line3.slope = slope(line3.point1.x, line3.point1.y, line3.point2.x, line3.point2.y);
			}
			else if (L3P2) {
				line3.point2.x = (x / widthd) * 2 - 1;
				line3.point2.y = ((heightd - y) / heightd) * 2 - 1;
				line3.slope = slope(line3.point1.x, line3.point1.y, line3.point2.x, line3.point2.y);
			}
			else if (L4P1) {
				line4.point1.x = (x / widthd) * 2 - 1;
				line4.point1.y = ((heightd - y) / heightd) * 2 - 1;
				line4.slope = slope(line4.point1.x, line4.point1.y, line4.point2.x, line4.point2.y);
			}
			else if (L4P2) {
				line4.point2.x = (x / widthd) * 2 - 1;
				line4.point2.y = ((heightd - y) / heightd) * 2 - 1;
				line4.slope = slope(line4.point1.x, line4.point1.y, line4.point2.x, line4.point2.y);
			}
		}

		notMovingLines = false;
		if (line1.point1.x - point1 < range && line1.point1.x - point1 > -range) {
			if (line1.point1.y - point2 < range && line1.point1.y - point2 > -range) { //move 1st point
				L1P1 = true;
				line1.point1.x = (x / widthd) * 2 - 1;
				line1.point1.y = ((heightd - y) / heightd) * 2 - 1;
				line1.slope = slope(line1.point1.x, line1.point1.y, line1.point2.x, line1.point2.y);
			}
		}
		else if (line1.point2.x - point1 < range && line1.point2.x - point1 > -range) {
			if (line1.point2.y - point2 < range && line1.point2.y - point2 > -range) { //move 2nd point
				L1P2 = true;
				line1.point2.x = (x / widthd) * 2 - 1;
				line1.point2.y = ((heightd - y) / heightd) * 2 - 1;
				line1.slope = slope(line1.point1.x, line1.point1.y, line1.point2.x, line1.point2.y);
			}
		}
		else if (line2.point1.x - point1 < range && line2.point1.x - point1 > -range) {
			if (line2.point1.y - point2 < range && line2.point1.y - point2 > -range) { //move 1st point
				L2P1 = true;
				line2.point1.x = (x / widthd) * 2 - 1;
				line2.point1.y = ((heightd - y) / heightd) * 2 - 1;
				line2.slope = slope(line2.point1.x, line2.point1.y, line2.point2.x, line2.point2.y);
			}
		}
		else if (line2.point2.x - point1 < range && line2.point2.x - point1 > -range) {
			if (line2.point2.y - point2 < range && line2.point2.y - point2 > -range) { //move 2nd point
				L2P2 = true;
				line2.point2.x = (x / widthd) * 2 - 1;
				line2.point2.y = ((heightd - y) / heightd) * 2 - 1;
				line2.slope = slope(line2.point1.x, line2.point1.y, line2.point2.x, line2.point2.y);
			}
		}
		else if (line3.point1.x - point1 < range && line3.point1.x - point1 > -range) {
			if (line3.point1.y - point2 < range && line3.point1.y - point2 > -range) { //move 1st point
				L3P1 = true;
				line3.point1.x = (x / widthd) * 2 - 1;
				line3.point1.y = ((heightd - y) / heightd) * 2 - 1;
				line3.slope = slope(line3.point1.x, line3.point1.y, line3.point2.x, line3.point2.y);
			}
		}
		else if (line3.point2.x - point1 < range && line3.point2.x - point1 > -range) {
			if (line3.point2.y - point2 < range && line3.point2.y - point2 > -range) { //move 2nd point
				L3P2 = true;
				line3.point2.x = (x / widthd) * 2 - 1;
				line3.point2.y = ((heightd - y) / heightd) * 2 - 1;
				line3.slope = slope(line3.point1.x, line3.point1.y, line3.point2.x, line3.point2.y);
			}
		}
		else if (line4.point1.x - point1 < range && line4.point1.x - point1 > -range) {
			if (line4.point1.y - point2 < range && line4.point1.y - point2 > -range) { //move 1st point
				L4P1 = true;
				line4.point1.x = (x / widthd) * 2 - 1;
				line4.point1.y = ((heightd - y) / heightd) * 2 - 1;
				line4.slope = slope(line4.point1.x, line4.point1.y, line4.point2.x, line4.point2.y);
			}
		}
		else if (line4.point2.x - point1 < range && line4.point2.x - point1 > -range) {
			if (line4.point2.y - point2 < range && line4.point2.y - point2 > -range) { //move 2nd point
				L4P2 = true;
				line4.point2.x = (x / widthd) * 2 - 1;
				line4.point2.y = ((heightd - y) / heightd) * 2 - 1;
				line4.slope = slope(line4.point1.x, line4.point1.y, line4.point2.x, line4.point2.y);
			}
		}
		else {
			notMovingLines = true;
		}
		display();
	}
	noTracing = false;
}

static void mouse_press(int button, int state, int x, int y) {
	double widthd = width;
	double heightd = height;

	if (button == 0 && state == 0) {
		leftClicked = true;
		display();
	}
	if (button == 0 && state == 1) { //left button released
		L1P1 = false;
		L1P2 = false;
		L2P1 = false;
		L2P2 = false;
		L3P1 = false;
		L3P2 = false;
		L4P1 = false;
		L4P2 = false;

		if (!notMovingLines) {
			notMovingLines = true;
			return;
		}

		if (currentlyTracing) {
			currentlyTracing = false;
			return;
		}

		leftClicked = false;

		switch (pointsClicked) {
		case 0:
			line1.point1.x = (x / widthd) * 2 - 1;
			line1.point1.y = ((heightd - y) / heightd) * 2 - 1;
			break;
		case 1:
			line1.point2.x = (x / widthd) * 2 - 1;
			line1.point2.y = ((heightd - y) / heightd) * 2 - 1;
			line1.slope = slope(line1.point1.x, line1.point1.y, line1.point2.x, line1.point2.y);
			break;
		case 2:
			line2.point1.x = (x / widthd) * 2 - 1;
			line2.point1.y = ((heightd - y) / heightd) * 2 - 1;
			break;
		case 3:
			line2.point2.x = (x / widthd) * 2 - 1;
			line2.point2.y = ((heightd - y) / heightd) * 2 - 1;
			line2.slope = slope(line2.point1.x, line2.point1.y, line2.point2.x, line2.point2.y);
			break;
		case 4:
			line3.point1.x = (x / widthd) * 2 - 1;
			line3.point1.y = ((heightd - y) / heightd) * 2 - 1;
			break;
		case 5:
			line3.point2.x = (x / widthd) * 2 - 1;
			line3.point2.y = ((heightd - y) / heightd) * 2 - 1;
			line3.slope = slope(line3.point1.x, line3.point1.y, line3.point2.x, line3.point2.y);
			break;
		case 6:
			if (lines == 4) {
				line4.point1.x = (x / widthd) * 2 - 1;
				line4.point1.y = ((heightd - y) / heightd) * 2 - 1;
			}
			else if (lines == 3) {
				tracer.x = (x / widthd) * 2 - 1;
				tracer.y = ((heightd - y) / heightd) * 2 - 1;
			}
			break;
		case 7:
			if (lines == 4) {
				line4.point2.x = (x / widthd) * 2 - 1;
				line4.point2.y = ((heightd - y) / heightd) * 2 - 1;
				line4.slope = slope(line4.point1.x, line4.point1.y, line4.point2.x, line4.point2.y);
			}
			else if (lines == 3) {
				tracer.x = (x / widthd) * 2 - 1;
				tracer.y = ((heightd - y) / heightd) * 2 - 1;
			}
			break;
		case 8:
			tracer.x = (x / widthd) * 2 - 1;
			tracer.y = ((heightd - y) / heightd) * 2 - 1;
			break;
		default:
			tracer.x = (x / widthd) * 2 - 1;
			tracer.y = ((heightd - y) / heightd) * 2 - 1;
			break;
		}

		pointsClicked++;
		display();
	}
}

double slope(double point1x, double point1y, double point2x, double point2y) {
	return ((point2y - point1y) / (point2x - point1x));
}

double yAt_xMin(double pointx, double pointy, double slope) {
	return (-1 - pointx) * slope + pointy;
}

double yAt_xMax(double pointx, double pointy, double slope) {
	return (1 - pointx) * slope + pointy;
}

double xAt_yMin(double pointx, double pointy, double slope) {
	return (-1 - pointy) / slope + pointx;
}

double xAt_yMax(double pointx, double pointy, double slope) {
	return (1 - pointy) / slope + pointx;
}

void drawLine(double pointx, double pointy, double slope) {
	//end the point drawings
	glEnd();
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);

	//draw the lines
	if (yAt_xMin(pointx, pointy, slope) > -1 && yAt_xMin(pointx, pointy, slope) < 1) {
		if (xAt_yMax(pointx, pointy, slope) > -1 && xAt_yMax(pointx, pointy, slope) < 1) { //1
			glVertex2f(-1.0, yAt_xMin(pointx, pointy, slope));
			glVertex2f(xAt_yMax(pointx, pointy, slope), 1.0);
		}
		else { //2
			glVertex2f(-1.0, yAt_xMin(pointx, pointy, slope));
			glVertex2f(1, yAt_xMax(pointx, pointy, slope));
		}
	}
	else {
		if (xAt_yMax(pointx, pointy, slope) > -1 && xAt_yMax(pointx, pointy, slope) < 1) { //3
			glVertex2f(xAt_yMin(pointx, pointy, slope), -1);
			glVertex2f(xAt_yMax(pointx, pointy, slope), 1);
		}
		else { //4
			glVertex2f(xAt_yMin(pointx, pointy, slope), -1);
			glVertex2f(1, yAt_xMax(pointx, pointy, slope));
		}
	}

	//start the point drawings
	glEnd();
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);
}

bool location(double xVal, double yVal) {
	double L1 = (tracer.y - line1.point1.y) - line1.slope * (tracer.x - line1.point1.x);
	double L2 = (tracer.y - line2.point1.y) - line2.slope * (tracer.x - line2.point1.x);
	double L3 = (tracer.y - line3.point1.y) - line3.slope * (tracer.x - line3.point1.x);
	double L4 = (tracer.y - line4.point1.y) - line4.slope * (tracer.x - line4.point1.x);

	double t;
	if (lines == 4)
		t = -(L1 * L2) / (L3 * L4);
	else if (lines == 3)
		t = -(L1 * L2) / (L3 * L3);

	L1 = (yVal - line1.point1.y) - line1.slope * (xVal - line1.point1.x);
	L2 = (yVal - line2.point1.y) - line2.slope * (xVal - line2.point1.x);
	L3 = (yVal - line3.point1.y) - line3.slope * (xVal - line3.point1.x);
	L4 = (yVal - line4.point1.y) - line4.slope * (xVal - line4.point1.x);

	double value;
	if (lines == 4)
		value = (L1 * L2) + t * (L3 * L4);
	else if (lines == 3)
		value = (L1 * L2) + t * (L3 * L3);

	if (value > 0)
		return true;
	else
		return false;
}

void drawConic() {
	//start the line drawings
	glEnd();
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);

	//Marching Squares
	for (double y = 1.0; y > -1.0; y -= step) {
		for (double x = -1.0; x < 1.0; x += step) { // this is 1 square
													// square and its corner points
			Square s;
			s.point1.x = x;
			s.point1.y = y;
			s.point2.x = x + step;
			s.point2.y = y;
			s.point3.x = x;
			s.point3.y = y - step;
			s.point4.x = x + step;
			s.point4.y = y - step;

			/*
			For boolean square corners:
			-p1 is top left
			-p2 is top right
			-p3 is bottom left
			-p4 is bottom right
			*/

			s.p1 = location(s.point1.x, s.point1.y);
			s.p2 = location(s.point2.x, s.point2.y);
			s.p3 = location(s.point3.x, s.point3.y);
			s.p4 = location(s.point4.x, s.point4.y);

			if ((s.p1 && !s.p2 && s.p3 && !s.p4) || (!s.p1 && s.p2 && !s.p3 && s.p4)) { // vertical
				glVertex2f((s.point1.x + s.point2.x) / 2, s.point1.y);
				glVertex2f((s.point1.x + s.point2.x) / 2, s.point3.y);
			}
			else if ((s.p1 && s.p2 && !s.p3 && !s.p4) || (!s.p1 && !s.p2 && s.p3 && s.p4)) { // horizontal
				glVertex2f(s.point1.x, (s.point1.y + s.point3.y) / 2);
				glVertex2f(s.point2.x, (s.point1.y + s.point3.y) / 2);
			}
			else if ((!s.p1 && !s.p2 && s.p3 && !s.p4) || (s.p1 && s.p2 && !s.p3 && s.p4)) { // diagonal bottom left
				glVertex2f(s.point1.x, (s.point1.y + s.point3.y) / 2);
				glVertex2f((s.point1.x + s.point2.x) / 2, s.point3.y);
			}
			else if ((!s.p1 && !s.p2 && !s.p3 && s.p4) || (s.p1 && s.p2 && s.p3 && !s.p4)) { // diagonal bottom right
				glVertex2f((s.point1.x + s.point2.x) / 2, s.point3.y);
				glVertex2f(s.point2.x, (s.point1.y + s.point3.y) / 2);
			}
			else if ((!s.p1 && s.p2 && !s.p3 && !s.p4) || (s.p1 && !s.p2 && s.p3 && s.p4)) { // diagonal top right
				glVertex2f((s.point1.x + s.point2.x) / 2, s.point1.y);
				glVertex2f(s.point2.x, (s.point1.y + s.point3.y) / 2);
			}
			else if ((s.p1 && !s.p2 && !s.p3 && !s.p4) || (!s.p1 && s.p2 && s.p3 && s.p4)) { // diagonal top left
				glVertex2f(s.point1.x, (s.point1.y + s.point3.y) / 2);
				glVertex2f((s.point1.x + s.point2.x) / 2, s.point1.y);
			}
		}
	}
}

static void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0f, 1.0f, 0.0f);
	glPointSize(3);

	//draw points and lines
	glBegin(GL_POINTS);
	switch (pointsClicked) {
	case 0:
		glEnd();
		break;
	default:
		//First Line
		glVertex2f(line1.point1.x, line1.point1.y);
		if (pointsClicked == 1)
			break;

		glVertex2f(line1.point2.x, line1.point2.y);

		drawLine(line1.point1.x, line1.point1.y, line1.slope);

		if (pointsClicked == 2)
			break;

		//Second Line
		glVertex2f(line2.point1.x, line2.point1.y);
		if (pointsClicked == 3)
			break;

		glVertex2f(line2.point2.x, line2.point2.y);

		drawLine(line2.point1.x, line2.point1.y, line2.slope);

		if (pointsClicked == 4)
			break;

		//Third Line
		glVertex2f(line3.point1.x, line3.point1.y);
		if (pointsClicked == 5)
			break;

		glVertex2f(line3.point2.x, line3.point2.y);

		drawLine(line3.point1.x, line3.point1.y, line3.slope);

		if (pointsClicked == 6)
			break;

		//Fourth Line
		if (lines == 3) {
			glVertex2f(tracer.x, tracer.y);
			drawConic();
			break;
		}
		glVertex2f(line4.point1.x, line4.point1.y);
		if (pointsClicked == 7)
			break;

		glVertex2f(line4.point2.x, line4.point2.y);

		drawLine(line4.point1.x, line4.point1.y, line4.slope);

		if (pointsClicked == 8)
			break;

		//tracer
		glVertex2f(tracer.x, tracer.y);
		drawConic();
	}
	if (printTrace) {
		//tracer
		glVertex2f(tracer.x, tracer.y);
		drawConic();
	}
	glEnd();

	//SwapBuffers
	glutSwapBuffers();
}

void reshape(int w, int h) {
	width = w;
	height = h;

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void init(void) {
	/* select clearing (background) color */
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float s = width / 2.0;
	if (height < width) s = height / 2;
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
	glutInit(&__argc, __argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	glutInitWindowSize(width, height);
	glutInitWindowPosition(1000, 100);
	glutCreateWindow("Sampling");

	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	//mouse callbacks
	createGLUTMenus();
	glutMouseFunc(mouse_press);
	glutMotionFunc(mouse_active_move);

	glutMainLoop();
	return 0;
}