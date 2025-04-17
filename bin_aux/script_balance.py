import random

dragon = {
    "level": 1,
    "attack": 1,
    "defense": 1,
    "speed": 1,
    "health": 10
}

for i in range(0, dragon["level"]):
    dragon["health"] += random.choice(range(17, 24))
    dragon["attack"] += random.choice(range(1, 4))
    dragon["defense"] += random.choice(range(1, 3))
    dragon["speed"] += random.choice(range(2, 5))

for i in dragon.values():
    print(i)
