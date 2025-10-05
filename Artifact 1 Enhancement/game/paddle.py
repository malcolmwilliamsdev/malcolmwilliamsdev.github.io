import pygame
from game.constants import WHITE, PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_MARGIN, PADDLE_SPEED, WIDTH, HEIGHT

class Paddle:
    def __init__(self):
        self.width = PADDLE_WIDTH
        self.height = PADDLE_HEIGHT
        self.x = WIDTH // 2 - self.width // 2
        self.y = HEIGHT - PADDLE_MARGIN
        self.speed = PADDLE_SPEED

    def move(self, direction):
        self.x += direction * self.speed
        self.x = max(0, min(self.x, WIDTH - self.width))

    def draw(self, surface):
        pygame.draw.rect(surface, WHITE, (self.x, self.y, self.width, self.height))

    def get_rect(self):
        return pygame.Rect(self.x, self.y, self.width, self.height)
