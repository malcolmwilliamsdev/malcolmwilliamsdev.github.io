import pygame
from game.constants import WHITE, FONT, BLACK

class Button:
    def __init__(self, text, x, y, w, h, callback):
        self.text = text
        self.rect = pygame.Rect(x, y, w, h)
        self.callback = callback

    def draw(self, surface):
        pygame.draw.rect(surface, WHITE, self.rect, border_radius=8)
        text = FONT.render(self.text, True, BLACK)
        rect = text.get_rect(center=self.rect.center)
        surface.blit(text, rect)

    def handle_event(self, event):
        # detect mouse click
        if event.type == pygame.MOUSEBUTTONDOWN and self.rect.collidepoint(event.pos):
            self.callback()
