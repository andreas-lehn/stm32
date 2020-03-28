/*
 * Test of crt.c
 */

int a;
int b = 15;
int c;
int d = 0;
const int e = 34;
int f = 42;

char buffer[128] = {
    'h', 'a', 'l', 'l', 'o', '!', 0
};

const char * str = "This is data";

int main(int argc, char **argv) {
    int tmp = a;
    tmp = b;
    tmp = c;
    tmp = d;
    tmp = e;
    tmp = f;
    a = tmp;
    char l = buffer[2];
    char o = buffer[4];
    buffer[6] = o;
    char * a = str;
    buffer[8] = l;
    buffer[9] = 0;
    buffer[7] = a[5];
    return 0;
}
