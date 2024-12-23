#ifdef _WIN32
#include <conio.h>
bool isKeyPressed() {
    return _kbhit();
}
char getPressedKey() {
    return _getch();
}
#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
void enableNonBlockingMode() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

void restoreBlockingMode() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
}

bool isKeyPressed() {
    char buf = 0;
    ssize_t n = read(STDIN_FILENO, &buf, 1);
    if (n > 0) return true;
    return false;
}

char getPressedKey() {
    char buf = 0;
    read(STDIN_FILENO, &buf, 1);
    return buf;
}
#endif