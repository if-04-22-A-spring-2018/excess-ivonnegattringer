#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>


void print_page(FILE *file, struct winsize *ws) {
    char *cc = malloc(sizeof(char) * (ws->ws_col + 1));
    for (int i = 0; i < ws->ws_row && !feof(file); i++) {
        fgets(cc, ws->ws_col, file);
        printf("%s", cc);
    }
    free(cc);
}

void goAround(FILE *file, struct winsize *ws, int page) {
    rewind(file);
    if (file == 0)return;
    char s;
    int count = 0;
    while (count <= page) {
        system("clear");
        print_page(file, ws);
        count++;
    }
    s = getchar();
    if (s == '\n' && !feof(file)) {
        goAround(file, ws, page + 1);
    } else if (s == 'b' && page > 0) {
        while (s != '\n')
            s = getchar();
        goAround(file, ws, page - 1);
    } else if (s != '\n') {
        while (s != '\n')
            s = getchar();
        goAround(file, ws, page);
    }

}


int main(int argc, char *argv[]) {
    FILE *file = fopen(argv[1], "r");
    struct winsize ws;

    if (ioctl(0, TIOCGWINSZ, &ws) != 0) {
        fprintf(stderr, "TIOCGWINSZ:%s\n", strerror(errno));
        exit(-1);
    }
    goAround(file, &ws, 0);
    printf("row=%d, col=%d, xpixel=%d, ypixel=%d\n",
           ws.ws_row, ws.ws_col, ws.ws_xpixel, ws.ws_ypixel);
    fclose(file);
    return 0;
}
