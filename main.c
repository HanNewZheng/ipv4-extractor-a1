#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static int is_digit(char c) { return c >= '0' && c <= '9'; }
static int is_run_char(char c) { return is_digit(c) || c == '.' || c == ':'; }

static int number(const char **cursor, const char *end, int max_digits, int max_value, int *out)
{
    const char *p = *cursor;
    int value = 0, digits = 0;
    if (p == end || !is_digit(*p)) return 0;
    if (*p == '0' && p + 1 < end && is_digit(p[1])) return 0;
    while (p < end && is_digit(*p)) {
        if (++digits > max_digits) return 0;
        value = value * 10 + (*p++ - '0');
        if (value > max_value) return 0;
    }
    *cursor = p;
    *out = value;
    return 1;
}

static int valid_run(const char *start, const char *end, unsigned long *address, int *port)
{
    const char *p = start;
    int octet[4], parsed_port = -1;
    for (int i = 0; i < 4; ++i) {
        if (!number(&p, end, 3, 255, &octet[i])) return 0;
        if (i < 3) {
            if (p == end || *p != '.') return 0;
            ++p;
        }
    }
    if (p < end && *p == ':') {
        ++p;
        if (!number(&p, end, 5, 65535, &parsed_port)) return 0;
    }
    if (p != end) return 0;
    uint32_t value = 0;
    for (int i = 0; i < 4; ++i) value = (value << 8) | (uint32_t)octet[i];
    *address = (unsigned long)value;
    *port = parsed_port;
    return 1;
}

int extractIPv4(const char* str, unsigned long* outAddress, int* outPort)
{
    *outAddress = 0;
    *outPort = -1;
    if (str == NULL) return 0;
    const char *p = str;
    while (*p) {
        if (!is_run_char(*p)) { ++p; continue; }
        const char *start = p;
        while (is_run_char(*p)) ++p;
        if (valid_run(start, p, outAddress, outPort)) return 1;
    }
    return 0;
}

#ifndef IPV4_NO_MAIN
int main(void)
{
    char *line = NULL;
    size_t capacity = 0, length = 0;
    int ch;
    for (;;) {
        printf("Enter a string (or 'END' to quit): ");
        fflush(stdout);
        length = 0;
        while ((ch = getchar()) != EOF && ch != '\n') {
            if (length + 1 >= capacity) {
                size_t next = capacity ? capacity * 2 : 128;
                if (next <= capacity) { free(line); return 1; }
                char *grown = realloc(line, next);
                if (!grown) { free(line); return 1; }
                line = grown;
                capacity = next;
            }
            line[length++] = (char)ch;
        }
        if (ch == EOF && length == 0) break;
        if (length + 1 >= capacity) {
            char *grown = realloc(line, length + 1);
            if (!grown) { free(line); return 1; }
            line = grown;
            capacity = length + 1;
        }
        line[length] = '\0';
        if (length && line[length - 1] == '\r') line[--length] = '\0';
        if (strcmp(line, "END") == 0) {
            puts("Program terminated.");
            break;
        }
        unsigned long address;
        int port;
        if (extractIPv4(line, &address, &port)) {
            printf("Extracted IPv4 address: %lu.%lu.%lu.%lu (decimal value: %lu, port: ",
                   (address >> 24) & 255UL, (address >> 16) & 255UL,
                   (address >> 8) & 255UL, address & 255UL, address);
            if (port == -1) printf("none)\n");
            else printf("%d)\n", port);
        } else {
            puts("Invalid input: no valid IPv4 address found");
        }
        if (ch == EOF) break;
    }
    free(line);
    return 0;
}
#endif
