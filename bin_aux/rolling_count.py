import time

def rolling_numbers(start, end, duration, fps=60):
    frames = int(duration * fps)  # Total de frames
    increment = (end - start) / frames  # XP ganho por frame
    current_value = start

    for _ in range(frames):
        current_value += increment
        print(f"XP: {int(current_value)}", end="\r")  # Atualiza a tela no mesmo lugar
        time.sleep(1 / fps)  # Espera o tempo necessário para simular a animação

    print(f"XP: {end}")  # Garante que o valor final seja exatamente o desejado

# Exemplo: Subir de 0 para 20000 XP em 2 segundos
rolling_numbers(0, 20000, 4)
