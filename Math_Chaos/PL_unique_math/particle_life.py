import pygame
import random

window_size = 300
pygame.init()
screen = pygame.display.set_mode((window_size, window_size))


def draw(surface, x, y, color, size):
    for _ in range(size):
        pygame.draw.line(surface, color, (x, y - 1), (x, y + 2), size)


def atom(x, y, color):
    return {"x": x, "y": y, "vx": 0, "vy": 0, "color": color}


def create(number, color):
    group = []
    for _ in range(number):
        group.append(atom(random.randint(1, window_size - 1),
                          random.randint(1, window_size - 1), color))
    return group


def update(particles, g):
    for i in range(len(particles)):
        fx, fy = 0, 0
        for j in range(len(particles)):
            a = particles[i]
            b = particles[j]
            dx, dy = a["x"] - b["x"], a["y"] - b["y"]
            r = (dx ** 2 + dy ** 2) ** 0.5
            if r > 0 and r < 80:
                F = g / r
                fx += F * dx
                fy += F * dy
        a["vx"] = (a["vx"] + fx) * 0.5
        a["vy"] = (a["vy"] + fy) * 0.5
        a["x"] += a["vx"]
        a["y"] += a["vy"]
        if a["x"] <= 0 or a["x"] >= window_size:
            a["vx"] *= -1
        if a["y"] <= 0 or a["y"] >= window_size:
            a["vy"] *= -1


yellow = create(100, (255, 255, 0))
red = create(100, (255, 0, 0))

run = True
while run:
    screen.fill((0, 0, 0))
    update(red, 0.1)
    update(red, -0.15)
    update(yellow, -0.1)
    for p in yellow + red:
        draw(screen, p["x"], p["y"], p["color"], 3)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False

    pygame.display.flip()

pygame.quit()

