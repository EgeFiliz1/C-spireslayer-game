#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

typedef struct {
    int gold;
    int current_hp;
    int max_hp;
    int floor;
    char current_room[16];
} GameState;

static void init_game_state(GameState *state) {
    state->gold = 0;
    state->current_hp = 80;
    state->max_hp = 80;
    state->floor = 0;
    strcpy(state->current_room, "NONE");
}

static void chomp_newline(char *line) {
    size_t len = strlen(line);

    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
    }
}

static int execute_line(GameState *state, const char *line) {
    (void)state;
    (void)line;

    /*
     * Implement the Spring 2026 "spireslayer" grammar here.
     *
     * A typical approach is:
     * 1. Parse the whole line.
     * 2. Reject malformed input by returning 0.
     * 3. Apply semantic checks and print the required response.
     * 4. Return 1 after printing any non-INVALID response.
     */
    return 0;
}

int main(void) {
    char line[MAX_LINE_LENGTH + 2];
    GameState state;

    init_game_state(&state);

    while (1) {
        printf(">> ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        chomp_newline(line);

        if (strcmp(line, "Exit") == 0) {
            break;
        }

        if (!execute_line(&state, line)) {
            printf("INVALID\n");
        }
    }

    return 0;
}
