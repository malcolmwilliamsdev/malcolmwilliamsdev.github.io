#include <GLFW\glfw3.h>
#include "linmath.h"
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <windows.h>
#include <time.h>
#include <algorithm>

using namespace std;

const float DEG2RAD = 3.14159 / 180;

// Ball variables
bool isBallActive = false;
float const ballResetX = 0.0f;
float const ballResetY = -0.8f;

float const ballSpeedMin = 0.005f;
float const ballSpeedBoost = 0.001125f;
float const ballSpeedMax = 0.015f;
float ballSpeed = 0.0f;

// identifiers
int circleId = 0;
int brickId = 0;
vector<int> circlesToRemove;
vector<int> bricksToRemove;

void processInput(GLFWwindow* window);

int randomBetween(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

enum BRICKTYPE { REFLECTIVE, DESTRUCTABLE, EXPLODING };
enum ONOFF { ON, OFF };

class Brick
{
public:
	int id;
	float red, green, blue;
	float x, y, width;
	BRICKTYPE brick_type;
	ONOFF onoff;

	Brick(BRICKTYPE bt, float xx, float yy, float ww, float rr, float gg, float bb)
	{
		id = brickId++;
		brick_type = bt; x = xx; y = yy, width = ww; red = rr, green = gg, blue = bb;
		onoff = ON;
	};

	void drawBrick()
	{
		if (onoff == ON)
		{
			double halfside = width / 2;

			glColor3d(red, green, blue);
			glBegin(GL_POLYGON);

			glVertex2d(x + halfside, y + halfside);
			glVertex2d(x + halfside, y - halfside);
			glVertex2d(x - halfside, y - halfside);
			glVertex2d(x - halfside, y + halfside);

			glEnd();
		}
	}
};

class Circle;
vector<Circle> ballVector;

class Paddle
{
public:
	float red, green, blue;
	float x, y, width, height;

	Paddle(float xx, float yy, float ww, float hh, float rr, float gg, float bb)
	{
		x = xx; y = yy; width = ww; height = hh; red = rr; green = gg; blue = bb;
	};

	void draw()
	{
		double halfwidth = width / 2;
		double halfheight = height / 2;

		glColor3d(red, green, blue);
		glBegin(GL_POLYGON);

		glVertex2d(x + halfwidth, y + halfheight);
		glVertex2d(x + halfwidth, y - halfheight);
		glVertex2d(x - halfwidth, y - halfheight);
		glVertex2d(x - halfwidth, y + halfheight);

		glEnd();
	}
};

// Paddle variables
float paddleSpeed = 0.015;
float paddlePosX = 0.0f;
float const paddlePosY = -0.9f;
float const paddleWidth = 0.3f;
float const paddleHeight = 0.08f;

Paddle paddle = Paddle(0, paddlePosY - paddleHeight / 2, paddleWidth, paddleHeight, 1, 1, 1);

vector<Brick> brickVector;

class Circle
{
public:
	int id;
	float red, green, blue;
	float radius;
	float x;
	float y;
	float speed = 0.0015;
	int direction;


	Circle(double xx, double yy, double rr, int dir, float rad, float r, float g, float b)
	{
		id = circleId++;
		x = xx;
		y = yy;
		radius = rr;
		red = r;
		green = g;
		blue = b;
		radius = rad;
		direction = dir;
	}

	void CheckBrickCollisions()
	{
		bool hitTop = false;
		bool hitBottom = false;
		bool hitLeft = false;
		bool hitRight = false;

		// loop through all bricks
		for (Brick& brick : brickVector)
		{
			// Check brick's x range
			if (x + radius >= brick.x - brick.width / 2 && x - radius <= brick.x + brick.width / 2)
			{
				// Check brick's y range
				if (y + radius >= brick.y - brick.width / 2 && y - radius <= brick.y + brick.width / 2)
				{
					if (brick.brick_type == DESTRUCTABLE)
					{
						// Queue for removal
						bricksToRemove.push_back(brick.id);
					}
					else if (brick.brick_type == EXPLODING)
					{
						// Queue for removal
						bricksToRemove.push_back(brick.id);

						// Create new balls
						Circle ball1 = Circle(x, y, 0, 45, 0.05, red, 1, blue);
						ball1.speed = ballSpeedMin;
						ball1.direction = randomBetween(0, 359);
						ballVector.push_back(ball1);
						
						/*
						Circle ball2 = Circle(x, y, 0, 135, 0.05, red, 1, blue);
						ball2.speed = ballSpeedMin;
						ball2.direction = randomBetween(0, 359);
						ballVector.push_back(ball2);
						
						Circle ball3 = Circle(x, y, 0, 225, 0.05, red, 1, blue);
						ball3.speed = ballSpeedMin;
						ball3.direction = randomBetween(0, 359);
						ballVector.push_back(ball3);

						Circle ball4 = Circle(x, y, 0, 315, 0.05, red, 1, blue);
						ball4.speed = ballSpeedMin;
						ball4.direction = randomBetween(0, 359);
						ballVector.push_back(ball4);
						*/
					}
					else if (brick.brick_type == REFLECTIVE)
					{
						// change brick color
						//brick.red = (rand() % 100) / 100.0;
						//brick.green = (rand() % 100) / 100.0;
						//brick.blue = (rand() % 100) / 100.0;

						// change brick size
						brick.width = brick.width *.75;

						if (brick.width <= 0.05)
						{
							// Queue for removal
							bricksToRemove.push_back(brick.id);
						}
					}

					// Check hit side
					float overlapLeft = (brick.x - brick.width / 2) - (x + radius);
					float overlapRight = (x - radius) - (brick.x + brick.width / 2);
					float overlapTop = (brick.y - brick.width / 2) - (y + radius);
					float overlapBottom = (y - radius) - (brick.y + brick.width / 2);

					bool hitHorizontal = abs(overlapLeft) < abs(overlapRight) ? overlapLeft : overlapRight;
					bool hitVertical = abs(overlapTop) < abs(overlapBottom) ? overlapTop : overlapBottom;

					
					if (abs(hitHorizontal) < abs(hitVertical))
					{
						if (hitHorizontal == overlapLeft)
						{
							hitLeft = true;
						}
						else
						{
							hitRight = true;
						}
					}
					else
					{
						if (hitVertical == overlapTop)
						{
							hitTop = true;
						}
						else
						{
							hitBottom = true;
						}
					}


				}
			}
		}

		if (hitTop || hitBottom || hitLeft || hitRight)
		{
			// Change color if not main ball
			if (id != 0)
			{
				red = (rand() % 100) / 100.0;
				green = (rand() % 100) / 100.0;
				blue = (rand() % 100) / 100.0;
			}

			// Bounce
			if (hitTop || hitBottom)
			{
				direction = -direction;
			}
			else if (hitLeft || hitRight)
			{
				direction = 180 - direction;
			}

			// Speed up
			speed = min(speed + ballSpeedBoost, ballSpeedMax);

			// Randomize the direction a little bit
			direction += randomBetween(-10, 10);
		}
	}


	void MoveOneStep()
	{
		// direction is the direction the circle is moving in degrees 0-360
		x += cos(direction * DEG2RAD) * speed;
		y += sin(direction * DEG2RAD) * speed;

		// clamp to the screen
        x = max(-1 + radius, min(1 - radius, x));
        y = max(-1 + radius, min(1 - radius, y));
	}

	void CheckWallCollisions()
	{
		bool didBounce = false;
		// Right wall
		if (x + radius >= 1)
		{
			direction = 180 - direction;
			didBounce = true;
		}
		// Left wall
		else if (x - radius <= -1)
		{
			direction = 180 - direction;
			didBounce = true;
		}
		// Top wall
		if (y + radius >= 1)
		{
			direction = -direction;
			didBounce = true;
		}
		// Bottom wall
		else if (y - radius <= -1)
		{
			// main ball hit ground. Reset it
			if (id == 0)
			{
				x = ballResetX;
				y = ballResetY;
				speed = 0;
				isBallActive = false;
			}
			else
			{
				circlesToRemove.push_back(id);
				return;
			}
		}

		if (didBounce)
		{
			// Change color if bounced
			if (id != 0)
			{
				red = (rand() % 100) / 100.0;
				green = (rand() % 100) / 100.0;
				blue = (rand() % 100) / 100.0;
			}

			// Speed up
			speed = min(speed + ballSpeedBoost, ballSpeedMax);

			// Randomize the direction a little bit
			direction += randomBetween(-10, 10);
		}
	}

	void CheckBallCollision()
	{
		// loop through all balls
		for (Circle& c : ballVector)
		{
			// skip self
			if (c.id == id)
			{
				continue;
			}

			// skip main ball if not active
			if (c.id == 0 && isBallActive)
			{
				continue;
			}

			// Check ball's x range
			if (x + radius >= c.x - c.radius && x - radius <= c.x + c.radius)
			{
				// Check ball's y range
				if (y + radius >= c.y - c.radius && y - radius <= c.y + c.radius)
				{
					// Check hit side
					float overlapLeft = (c.x - c.radius) - (x + radius);
					float overlapRight = (x - radius) - (c.x + c.radius);
					float overlapTop = (c.y - c.radius) - (y + radius);
					float overlapBottom = (y - radius) - (c.y + c.radius);

					bool hitHorizontal = abs(overlapLeft) < abs(overlapRight) ? overlapLeft : overlapRight;
					bool hitVertical = abs(overlapTop) < abs(overlapBottom) ? overlapTop : overlapBottom;

					if (abs(hitHorizontal) < abs(hitVertical))
					{
						if (hitHorizontal == overlapLeft)
						{
							direction = 180 - direction;
							c.direction = 180 - c.direction;
						}
						else
						{
							direction = 180 - direction;
							c.direction = 180 - c.direction;
						}
					}
					else
					{
						if (hitVertical == overlapTop)
						{
							direction = -direction;
							c.direction = -c.direction;
						}
						else
						{
							direction = -direction;
							c.direction = -c.direction;
						}
					}

					// Change color if not main ball
					if (id != 0)
					{
						red = (rand() % 100) / 100.0;
						green = (rand() % 100) / 100.0;
						blue = (rand() % 100) / 100.0;
					}
				}
			}
		}
	}
		

	void CheckPaddleCollision()
	{
		// Check paddle's x range
		if (x >= paddle.x - paddle.width / 2 && x <= paddle.x + paddle.width / 2)
		{
			// Check paddle's y range
			if (y - radius <= paddle.y + paddle.height / 2 && y - radius >= paddle.y - paddle.height / 2)
			{
				// Get out of the box
				y = paddle.y + paddle.height / 2 + radius;

				// Bpunce
				direction = -direction;
				
				// Speed up
				speed = min(speed + ballSpeedBoost, ballSpeedMax);
				
				// Randomize the direction a little bit
				direction += randomBetween(-10, 10);

				// Change color if not main ball
				if (id != 0)
				{
					red = (rand() % 100) / 100.0;
					green = (rand() % 100) / 100.0;
					blue = (rand() % 100) / 100.0;
				}
			}
		}
	}


	void DrawCircle()
	{
		glColor3f(red, green, blue);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++) {
			float degInRad = i * DEG2RAD;
			glVertex2f((cos(degInRad) * radius) + x, (sin(degInRad) * radius) + y);
		}
		glEnd();
	}
};



void initBall()
{
	circleId = 0;
	isBallActive = false;
	Circle ball = Circle(ballResetX, ballResetY, 0, 0.05, 0.05, .3, .3, 1);
	ball.speed = 0;
	ballVector.push_back(ball);
}

void initBricks()
{
	float lg = .2f;
	float md = .15f;
	float sm = .1f;

	// row 1
	brickVector.push_back(Brick(REFLECTIVE, -0.8, 0.8, lg, 1, 1, 0)); // yellow
	brickVector.push_back(Brick(DESTRUCTABLE, -0.6, 0.8, md, 0, 0, 1)); // blue
	brickVector.push_back(Brick(EXPLODING, -0.4, 0.8, sm, 1, 0, 0)); // red
	brickVector.push_back(Brick(REFLECTIVE, -0.2, 0.8, lg, 1, 1, 0)); // yellow
	brickVector.push_back(Brick(DESTRUCTABLE, 0, 0.8, md, 0, 0, 1)); // blue
	brickVector.push_back(Brick(EXPLODING, 0.2, 0.8, sm, 1, 0, 0)); // red
	brickVector.push_back(Brick(REFLECTIVE, 0.4, 0.8, lg, 1, 1, 0)); // yellow
	brickVector.push_back(Brick(DESTRUCTABLE, 0.6, 0.8, md, 0, 0, 1)); // blue
	brickVector.push_back(Brick(EXPLODING, 0.8, 0.8, sm, 1, 0, 0)); // red

	// row 2
	brickVector.push_back(Brick(DESTRUCTABLE, -0.8, 0.6, md, 0, 0, 1)); // blue
	brickVector.push_back(Brick(EXPLODING, -0.6, 0.6, sm, 1, 0, 0)); // red
	brickVector.push_back(Brick(REFLECTIVE, -0.4, 0.6, lg, 1, 1, 0)); // yellow
	brickVector.push_back(Brick(DESTRUCTABLE, -0.2, 0.6, md, 0, 0, 1)); // blue
	brickVector.push_back(Brick(EXPLODING, 0, 0.6, sm, 1, 0, 0)); // red
	brickVector.push_back(Brick(REFLECTIVE, 0.2, 0.6, lg, 1, 1, 0)); // yellow
	brickVector.push_back(Brick(DESTRUCTABLE, 0.4, 0.6, md, 0, 0, 1)); // blue
	brickVector.push_back(Brick(EXPLODING, 0.6, 0.6, sm, 1, 0, 0)); // red
	brickVector.push_back(Brick(REFLECTIVE, 0.8, 0.6, lg, 1, 1, 0)); // yellow

	// row 3
	brickVector.push_back(Brick(EXPLODING, -0.8, 0.4, sm, 1, 0, 0)); // red
	brickVector.push_back(Brick(REFLECTIVE, -0.6, 0.4, lg, 1, 1, 0)); // yellow
	brickVector.push_back(Brick(DESTRUCTABLE, -0.4, 0.4, md, 0, 0, 1)); // blue
	brickVector.push_back(Brick(EXPLODING, -0.2, 0.4, sm, 1, 0, 0)); // red
	brickVector.push_back(Brick(REFLECTIVE, 0, 0.4, lg, 1, 1, 0)); // yellow
	brickVector.push_back(Brick(DESTRUCTABLE, 0.2, 0.4, md, 0, 0, 1)); // blue
	brickVector.push_back(Brick(EXPLODING, 0.4, 0.4, sm, 1, 0, 0)); // red
	brickVector.push_back(Brick(REFLECTIVE, 0.6, 0.4, lg, 1, 1, 0)); // yellow
	brickVector.push_back(Brick(DESTRUCTABLE, 0.8, 0.4, md, 0, 0, 1)); // blue

	// row 4
	brickVector.push_back(Brick(REFLECTIVE, -0.8, 0.2, lg, 1, 1, 0)); // yellow
	brickVector.push_back(Brick(DESTRUCTABLE, -0.6, 0.2, md, 0, 0, 1)); // blue
	brickVector.push_back(Brick(EXPLODING, -0.4, 0.2, sm, 1, 0, 0)); // red
	brickVector.push_back(Brick(REFLECTIVE, -0.2, 0.2, lg, 1, 1, 0)); // yellow
	brickVector.push_back(Brick(DESTRUCTABLE, 0, 0.2, md, 0, 0, 1)); // blue
	brickVector.push_back(Brick(EXPLODING, 0.2, 0.2, sm, 1, 0, 0)); // red
	brickVector.push_back(Brick(REFLECTIVE, 0.4, 0.2, lg, 1, 1, 0)); // yellow
	brickVector.push_back(Brick(DESTRUCTABLE, 0.6, 0.2, md, 0, 0, 1)); // blue
	brickVector.push_back(Brick(EXPLODING, 0.8, 0.2, sm, 1, 0, 0)); // red
}




int main(void) {
	srand((time(0)));

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	GLFWwindow* window = glfwCreateWindow(728, 728, "8-2 Assignment - Malcolm Williams", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	//brickVector.push_back(Brick(REFLECTIVE, paddlePosX, -.8, 0.1, 1, 0, 0));
	
	// Init main ball
	initBall();

	// Init bricks
	initBricks();

	while (!glfwWindowShouldClose(window)) {
		//Setup View
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		processInput(window);

		// Draw paddle
		paddle.draw();

		// Circle movement
		for (Circle &c : ballVector)
		{
			c.MoveOneStep();
			c.CheckBrickCollisions();
			c.CheckWallCollisions();
			c.CheckBallCollision();
			c.CheckPaddleCollision();
			c.DrawCircle();
		}

		// Remove circles that have hit the floor
		for (int id : circlesToRemove)
        {
            ballVector.erase(std::remove_if(ballVector.begin(), ballVector.end(), [id](const Circle& c) { return c.id == id; }), ballVector.end());
        }
		circlesToRemove.clear();

		// Remove destruction bricks that have been hit
		for (int id : bricksToRemove)
		{
			brickVector.erase(std::remove_if(brickVector.begin(), brickVector.end(), [id](const Brick& b) { return b.id == id; }), brickVector.end());
		}
		bricksToRemove.clear();

		// Draw bricks
		for (Brick &brick : brickVector)
		{
			brick.drawBrick();
		}

		// reset the bricks
		if (brickVector.empty())
		{
			ballVector.clear();
			initBall();
			initBricks();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwDestroyWindow(window);
	glfwTerminate;
	exit(EXIT_SUCCESS);
}

void movePaddle(int right)
{
	paddlePosX += paddleSpeed * right;
	paddlePosX = max(-1 + paddleWidth, min(1, paddlePosX));
	paddle.x = paddlePosX - paddleWidth / 2;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		// Start the ball
		if (!isBallActive)
		{
			ballVector.at(0).direction = randomBetween(45, 135);
			ballVector.at(0).speed = ballSpeedMin;
			isBallActive = true;
			
		}
	}

	// Move paddle right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		movePaddle(1);
	}

	// Move paddle left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		movePaddle(-1);
	}
}

