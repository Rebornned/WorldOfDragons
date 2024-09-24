import random

# Exemplo de uso:
total_xp = 10000
num_niveis = 100

# Exibir XP necessário para cada nível
for nvl in range(1, num_niveis):
    perlvlXp = random.choice(range(int(total_xp * ((nvl-1)/100)),  int(total_xp * (nvl/100)))) + 10
    print(f"{nvl+1} {perlvlXp}")
