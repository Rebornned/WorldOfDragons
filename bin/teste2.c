#include <stdio.h>
#include <windows.h>

int main() {
    // Exemplo de jogo baseado em setas do teclado
    printf("Pressione as setas (UP, DOWN, LEFT, RIGHT). Pressione ESC para sair.\n");

    while (1) {
        if (GetAsyncKeyState(VK_UP)) {
            printf("Tecla UP pressionada!\n");
            Sleep(200);  // Pequena pausa para evitar múltiplas detecções consecutivas
        }
        if (GetAsyncKeyState(VK_DOWN)) {
            printf("Tecla DOWN pressionada!\n");
            Sleep(200);
        }
        if (GetAsyncKeyState(VK_LEFT)) {
            printf("Tecla LEFT pressionada!\n");
            Sleep(200);
        }
        if (GetAsyncKeyState(VK_RIGHT)) {
            printf("Tecla RIGHT pressionada!\n");
            Sleep(200);
        }
        if (GetAsyncKeyState(VK_ESCAPE)) {
            printf("Saindo do jogo...\n");
            break;  // Sai do loop se a tecla ESC for pressionada
        }

        Sleep(50);  // Pequena pausa para não sobrecarregar o processador
    }

    return 0;
}
