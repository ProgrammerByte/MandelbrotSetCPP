//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include "Graphics2D.h"

#include <iostream>

using namespace std;
const int xRes = 1920 / 8;
const int yRes = 1080 / 8;
float aspectRatio;
double zoom = 1, xMov = -1.74995768370609350360221450607069970727110579726252077930242837820286008082972804887218672784431700831100544507655659531379747541999999995, yMov = 0.00000000000000000278793706563379402178294753790944364927085054500163081379043930650189386849765202169477470552201325772332454726999999995;
double currentXVal, currentYVal, invXRes, invYRes;
//float colours[][3] = { {0, 0, 0}, {1, 1, 1} };
int pixels[xRes][yRes];

void reset() {
	xMov = -1.7499576837060935036022145060706997072711057972625207793024283782028600808297;
	yMov = 0.0000000000000000027879370656337940217829475379094436492708505450016308137904;
	zoom = 1;
}

bool input(Graphics2D &engine) {
	switch (engine.getCurrentKey()) {
		case GLFW_KEY_ESCAPE:
			engine.closeWindow();
			return false;

		case GLFW_KEY_UP:
			yMov += 0.1 * zoom;
			return true;

		case GLFW_KEY_DOWN:
			yMov -= 0.1 * zoom;
			return true;

		case GLFW_KEY_RIGHT:
			xMov += 0.1 * zoom;
			return true;

		case GLFW_KEY_LEFT:
			xMov -= 0.1 * zoom;
			return true;

		case GLFW_KEY_Z:
			zoom *= 0.9;
			return true;

		case GLFW_KEY_X:
			zoom *= 1.1;
			return true;

		case GLFW_KEY_R:
			reset();
			return true;
	}
	return false;
}

bool isSuitable(double a, double b) {
	double tempVal;
	double a0 = a;
	double b0 = b;
	for (int i = 0; i < 1024; i++) {
		if (a * a + b * b > 4) {
			return false;
		}
		tempVal = (a * a) - (b * b);
		b = 2 * a * b + b0;
		a = tempVal + a0;
	}
	return true;
}

void calculatePoints(Graphics2D &engine) {
	for (int i = 0; i < xRes; i++) {
		for (int x = 0; x < yRes; x++) {
			currentXVal = ((double)i * 2 * (double)aspectRatio * invXRes - aspectRatio);
			currentYVal = (double)((double)x * 2 * invYRes - 1);
			if (isSuitable(currentXVal * zoom + xMov, currentYVal * zoom + yMov) == true) {
				//engine.point(currentXVal, currentYVal);
				pixels[i][x] = 1;
			}
			else {
				pixels[i][x] = 0;
			}
		}
	}
}

//TODO - POINTS ARE NOT RENDERED TOO WELL IN MY CURRENT ENGINE
//for now the coordinates system goes from (-aspectRatio, -1) to (aspectRatio, 1)
int main() {
	Graphics2D engine = Graphics2D(1920, 1080, "Mandelbrot set", true);
	aspectRatio = engine.getAspectRatio();
	engine.setLineColour(1, 1, 1, 1);
	invXRes = (double)1 / xRes;
	invYRes = (double)1 / yRes;
	double iInc = (double)(2 * (double)aspectRatio) / xRes;
	double xInc = (double)2 / yRes;

	calculatePoints(engine);
	while (engine.shouldClose() == false) {
		for (int i = 0; i < xRes; i++) {
			for (int x = 0; x < yRes; x++) {
				if (pixels[i][x] == 1) {
					//engine.setLineColour(pixels[i][x], pixels[i][x], pixels[i][x], 1);
					engine.point(iInc * i - aspectRatio, xInc * x - 1);
				}
			}
		}
		
		if (input(engine) == true) {
			calculatePoints(engine);
		}
		engine.clear();
	}
}