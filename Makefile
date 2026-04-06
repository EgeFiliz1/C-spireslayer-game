CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -pedantic

# Tüm .c dosyalarını buraya ekliyoruz ki beraber derlensinler 
SRCS := src/main.c src/player.c src/card.c src/enemy.c src/codex.c

.PHONY: default grade clean

default: spireslayer

# spireslayer artık tüm kaynak dosyalara (SRCS) bağlı
spireslayer: $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS)

grade:
	python3 test/grader.py ./spireslayer test-cases

clean:
	rm -f spireslayer