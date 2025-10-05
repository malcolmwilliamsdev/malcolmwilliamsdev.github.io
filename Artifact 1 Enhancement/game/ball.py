import pygame
from game.constants import *
import math
import random

class Ball:

    def __init__(self, x, y, speed=None):
        self.x = x
        self.y = y
        self.radius = BALL_RADIUS
        self.speed = BALL_START_SPEED if speed is None else speed
        self.angle = 0
        self.color = random.choice(BALL_COLORS)
        self.launched = False
        self.dx = 0
        self.dy = 0

    # randomly adjust ball direction slightly
    def randomize_direction(self, max_degrees=5):
        angle = math.atan2(self.dy, self.dx)
        delta = math.radians(random.uniform(-max_degrees, max_degrees))
        speed = math.hypot(self.dx, self.dy)
        angle += delta
        self.dx = math.cos(angle) * speed
        self.dy = math.sin(angle) * speed

    # launch the ball at a random angle within the launch range
    def launch(self):
        angle_deg = random.uniform(BALL_LAUNCH_ANGLE_MIN, BALL_LAUNCH_ANGLE_MAX)
        angle_rad = math.radians(angle_deg)
        self.dx = self.speed * math.sin(angle_rad)
        self.dy = -self.speed * math.cos(angle_rad)
        self.launched = True

    # update ball position and handle collisions
    def update(self, paddle, balls):
        if not self.launched:
            self.x = paddle.x + paddle.width // 2
            self.y = paddle.y - self.radius
            return
        self.x += self.dx
        self.y += self.dy
        if self.x - self.radius <= 0:
            self.x = self.radius
            self.dx *= -1
            self.randomize_direction()
            self.randomize_color()
        if self.x + self.radius >= WIDTH:
            self.x = WIDTH - self.radius
            self.dx *= -1
            self.randomize_direction()
            self.randomize_color()
        if self.y - self.radius <= 0:
            self.y = self.radius
            self.dy *= -1
            self.randomize_direction()
            self.randomize_color()
        if self.get_rect().colliderect(paddle.get_rect()) and self.dy > 0:
            self.y = paddle.y - self.radius
            self.dy *= -1
            self.speed = min(self.speed * BALL_SPEEDUP, BALL_MAX_SPEED)
            norm = math.sqrt(self.dx ** 2 + self.dy ** 2)
            self.dx = self.dx / norm * self.speed
            self.dy = self.dy / norm * self.speed
            self.randomize_color()
        for other in balls:
            if other is not self and self.launched and other.launched:
                dist = math.hypot(self.x - other.x, self.y - other.y)
                if dist < self.radius * 2:
                    self.dx, other.dx = other.dx, self.dx
                    self.dy, other.dy = other.dy, self.dy
                    self.randomize_color()
                    other.randomize_color()

    def draw(self, surface):
        pygame.draw.circle(surface, self.color, (int(self.x), int(self.y)), self.radius)

    def get_rect(self):
        return pygame.Rect(self.x - self.radius, self.y - self.radius, self.radius * 2, self.radius * 2)

    def randomize_color(self):
        self.color = random.choice(BALL_COLORS)

    # check ball out of bounds
    def is_out(self):
        return self.y - self.radius > HEIGHT
