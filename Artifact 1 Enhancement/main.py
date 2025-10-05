import pygame
import sys
import random

from game.constants import *
from game.paddle import Paddle
from game.ball import Ball
from game.brick import Brick
from game.button import Button

# class for managing game state
class BreakoutGame:
    def __init__(self, screen):
        self.screen = screen
        self.state = STATE_MENU
        self.lives = MAX_LIVES
        self.score = 0
        self.paddle = Paddle()
        self.balls = []
        self.bricks = []
        self.menu_buttons = []
        self.gameover_buttons = []
        self.init_menu()
        self.init_gameover()

    # difficulty settings
    def set_difficulty(self, speedup, start_speed):
        global BALL_SPEEDUP, BALL_START_SPEED, BALL_MAX_SPEED
        BALL_SPEEDUP = speedup
        BALL_START_SPEED = start_speed
        BALL_MAX_SPEED = BALL_START_SPEED * 3
        self.start_game()

    # initialize menu buttons
    def init_menu(self):
        menu_x = WIDTH // 2 - BUTTON_WIDTH // 2
        menu_y = HEIGHT // 2 - 1.5 * BUTTON_HEIGHT - BUTTON_GAP
        self.menu_buttons = [
            Button('Easy', menu_x, int(menu_y), BUTTON_WIDTH, BUTTON_HEIGHT, lambda: self.set_difficulty(BALL_SPEEDUP_EASY, BALL_START_SPEED_EASY)),
            Button('Medium', menu_x, int(menu_y + BUTTON_HEIGHT + BUTTON_GAP), BUTTON_WIDTH, BUTTON_HEIGHT, lambda: self.set_difficulty(BALL_SPEEDUP_MEDIUM, BALL_START_SPEED_MEDIUM)),
            Button('Hard', menu_x, int(menu_y + 2 * (BUTTON_HEIGHT + BUTTON_GAP)), BUTTON_WIDTH, BUTTON_HEIGHT, lambda: self.set_difficulty(BALL_SPEEDUP_HARD, BALL_START_SPEED_HARD)),
            Button('Exit', menu_x, int(menu_y + 3 * (BUTTON_HEIGHT + BUTTON_GAP)), BUTTON_WIDTH, BUTTON_HEIGHT, self.exit_game)
        ]

    # initialize game over buttons
    def init_gameover(self):
        go_x = WIDTH // 2 - GAMEOVER_BUTTON_WIDTH // 2
        go_y = HEIGHT // 2 + 40
        self.gameover_buttons = [
            Button('Play Again', go_x, go_y, GAMEOVER_BUTTON_WIDTH, GAMEOVER_BUTTON_HEIGHT, self.start_game),
            Button('Main Menu', go_x, go_y + GAMEOVER_BUTTON_HEIGHT + BUTTON_GAP, GAMEOVER_BUTTON_WIDTH, GAMEOVER_BUTTON_HEIGHT, self.goto_menu),
            Button('Exit', go_x, go_y + 2 * (GAMEOVER_BUTTON_HEIGHT + BUTTON_GAP), GAMEOVER_BUTTON_WIDTH, GAMEOVER_BUTTON_HEIGHT, self.exit_game)
        ]

    def start_game(self):
        self.state = STATE_PLAYING
        self.lives = MAX_LIVES
        self.score = 0
        self.paddle = Paddle()
        # start with a ball on the paddle
        self.balls = [Ball(self.paddle.x + self.paddle.width//2, self.paddle.y - BALL_RADIUS)]
        self.bricks = []
        # place bricks randomly grouped horizontally
        total_width = BRICK_COLS * BRICK_WIDTH + (BRICK_COLS - 1) * BRICK_SPACING
        x_offset = (WIDTH - total_width) // 2
        y_offset = BRICK_MARGIN_TOP
        for row in range(BRICK_ROWS):
            for col in range(BRICK_COLS):
                x = x_offset + col * (BRICK_WIDTH + BRICK_SPACING)
                y = y_offset + row * (BRICK_HEIGHT + BRICK_SPACING)
                brick_type = random.choice(BRICK_TYPES)
                self.bricks.append(Brick(x, y, brick_type))

    def goto_menu(self):
        self.state = STATE_MENU

    def exit_game(self):
        pygame.quit()
        sys.exit()

    # update game state with time and input
    def update(self, keys):
        if self.state == STATE_PLAYING:
            # move paddle
            direction = 0
            if keys[pygame.K_a] or keys[pygame.K_LEFT]:
                direction -= 1
            if keys[pygame.K_d] or keys[pygame.K_RIGHT]:
                direction += 1
            self.paddle.move(direction)
            
            # update balls
            for ball in self.balls:
                ball.update(self.paddle, self.balls)
            
            # launch ball if not launched
            if not any(ball.launched for ball in self.balls):
                if keys[pygame.K_SPACE]:
                    for ball in self.balls:
                        if not ball.launched:
                            ball.launch()

            # check ball to brick collisions
            for ball in self.balls:
                for brick in self.bricks:
                    if brick.alive and ball.get_rect().colliderect(pygame.Rect(brick.x, brick.y, brick.width, brick.height)):
                        brick_type = brick.hit()
                        ball.dy *= -1
                        # slightly shift direction and change color
                        ball.randomize_direction()
                        ball.randomize_color()
                        # all types award points when hit
                        if brick_type == 'spawn':
                            # spawn new ball at bricks location already launched
                            new_ball = Ball(brick.x + BRICK_WIDTH // 2, brick.y + BRICK_HEIGHT // 2, ball.speed)
                            new_ball.launch()
                            self.balls.append(new_ball)
                            self.score += BRICK_SCORE
                        elif brick_type == 'counter':
                            if not brick.alive:
                                self.score += BRICK_SCORE
                        else:
                            self.score += BRICK_SCORE
            # remove dead bricks
            self.bricks = [b for b in self.bricks if b.alive]
            # remove out balls
            self.balls = [b for b in self.balls if not b.is_out()]
            # check for game over
            if len(self.balls) == 0:
                self.lives -= 1
                if self.lives > 0:
                    # reset ball on paddle
                    self.balls = [Ball(self.paddle.x + self.paddle.width//2, self.paddle.y - BALL_RADIUS)]
                else:
                    self.state = STATE_GAME_OVER
            if len(self.bricks) == 0:
                self.state = STATE_GAME_OVER

    def draw(self):
        self.screen.fill(BLACK)
        # draw main menu
        if self.state == STATE_MENU:
            title = LARGE_FONT.render(GAME_TITLE, True, WHITE)
            rect = title.get_rect(center=(WIDTH//2, 100))
            self.screen.blit(title, rect)
            for btn in self.menu_buttons:
                btn.draw(self.screen)
        # draw gameplay
        if self.state == STATE_PLAYING:
            # lives
            for i in range(self.lives):
                pygame.draw.circle(self.screen, RED, (LIFE_INDICATOR_X + LIFE_INDICATOR_RADIUS + i*LIFE_INDICATOR_GAP, LIFE_INDICATOR_Y), LIFE_INDICATOR_RADIUS)
            # score
            score_text = FONT.render(f'{SCORE_LABEL} {self.score}', True, WHITE)
            self.screen.blit(score_text, (LIFE_INDICATOR_X, LIFE_INDICATOR_Y + LIFE_INDICATOR_RADIUS + 5))
            # paddle, balls, bricks
            self.paddle.draw(self.screen)
            for ball in self.balls:
                ball.draw(self.screen)
            for brick in self.bricks:
                brick.draw(self.screen)
        # draw game over screen
        elif self.state == STATE_GAME_OVER:
            win = len(self.bricks) == 0
            msg = WIN_MESSAGE if win else LOSE_MESSAGE
            title = LARGE_FONT.render(msg, True, WHITE)
            rect = title.get_rect(center=(WIDTH//2, HEIGHT//2-100))
            self.screen.blit(title, rect)
            score_text = FONT.render(f'{FINAL_SCORE_LABEL} {self.score}', True, WHITE)
            rect2 = score_text.get_rect(center=(WIDTH//2, HEIGHT//2-40))
            self.screen.blit(score_text, rect2)
            for btn in self.gameover_buttons:
                btn.draw(self.screen)

    # handle button events
    def handle_event(self, event):
        if self.state == STATE_MENU:
            for btn in self.menu_buttons:
                btn.handle_event(event)
        elif self.state == STATE_GAME_OVER:
            for btn in self.gameover_buttons:
                btn.handle_event(event)

def main():
    pygame.init()
    screen = pygame.display.set_mode((WIDTH, HEIGHT))
    pygame.display.set_caption(GAME_TITLE)

    game = BreakoutGame(screen)
    running = True

    # fixed timestep for game logic
    logic_dt = 1.0 / TICKRATE
    clock = pygame.time.Clock()
    prev_tick_time = pygame.time.get_ticks() / 1000.0

    # game loop
    while running:
        # event handling
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            game.handle_event(event)
        keys = pygame.key.get_pressed()

        # update game logic at fixed rate
        now = pygame.time.get_ticks() / 1000.0
        if now - prev_tick_time >= logic_dt:
            game.update(keys)
            prev_tick_time = now

        # rendering
        game.draw()
        pygame.display.flip()
        
        # cap frame rate
        clock.tick(FPS)

    # cleanup
    pygame.quit()
    sys.exit()

if __name__ == '__main__':
    main()
