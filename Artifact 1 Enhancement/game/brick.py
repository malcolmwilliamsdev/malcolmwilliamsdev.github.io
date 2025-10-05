import pygame
from game.constants import GRAY, FONT, BLACK, BRICK_WIDTH, BRICK_HEIGHT

class Brick:
    def __init__(self, x, y, brick_type):
        self.x = x
        self.y = y
        self.width = BRICK_WIDTH
        self.height = BRICK_HEIGHT
        self.type = brick_type
        self.counter = 3 if brick_type == 'counter' else 1
        self.alive = True

    # reduce counter or mark as dead
    def hit(self):
        self.counter -= 1
        if self.counter <= 0:
            self.alive = False
        return self.type

    def draw(self, surface):
        if self.type == 'normal':
            color = GRAY
        elif self.type == 'spawn':
            color = (0, 255, 255)
        else:
            color = (255, 128, 0)
        pygame.draw.rect(surface, color, (self.x, self.y, self.width, self.height))
        # draw counter if applicable
        if self.type == 'counter' and self.alive:
            text = FONT.render(str(self.counter), True, BLACK)
            rect = text.get_rect(center=(self.x + self.width // 2, self.y + self.height // 2))
            surface.blit(text, rect)
