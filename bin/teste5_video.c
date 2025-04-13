#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define PLAYER_RADIUS 20
#define PLAYER_SPEED 5

bool check_collision_circle_rect(int cx, int cy, int radius, SDL_Rect rect) {
    // Calcula ponto mais próximo no retângulo ao centro do círculo
    int closestX = SDL_clamp(cx, rect.x, rect.x + rect.w);
    int closestY = SDL_clamp(cy, rect.y, rect.y + rect.h);
    int dx = cx - closestX;
    int dy = cy - closestY;
    return (dx * dx + dy * dy) <= (radius * radius);
}

void draw_circle(SDL_Renderer* renderer, int cx, int cy, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if (dx * dx + dy * dy <= radius * radius)
                SDL_RenderDrawPoint(renderer, cx + dx, cy + dy);
        }
    }
}

int SDL_main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Colisão com SDL2",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int playerX = SCREEN_WIDTH / 2;
    int playerY = SCREEN_HEIGHT / 2;

    SDL_Rect bloco = { 100, 100, 100, 100 };

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }

        const Uint8* keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_UP]) playerY -= PLAYER_SPEED;
        if (keys[SDL_SCANCODE_DOWN]) playerY += PLAYER_SPEED;
        if (keys[SDL_SCANCODE_LEFT]) playerX -= PLAYER_SPEED;
        if (keys[SDL_SCANCODE_RIGHT]) playerX += PLAYER_SPEED;

        // Verificar colisão
        if (check_collision_circle_rect(playerX, playerY, PLAYER_RADIUS, bloco)) {
            printf("Colidiu!\n");
        }

        // Render
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // Fundo
        SDL_RenderClear(renderer);

        // Desenhar retângulo
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &bloco);

        // Desenhar jogador (círculo)
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        draw_circle(renderer, playerX, playerY, PLAYER_RADIUS);

        SDL_RenderPresent(renderer);

        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
