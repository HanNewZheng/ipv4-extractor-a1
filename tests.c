#include <assert.h>
#include <stdio.h>

int extractIPv4(const char* str, unsigned long* outAddress, int* outPort);

static void check(const char *input, int found, unsigned long expected_address, int expected_port)
{
    unsigned long address = 123;
    int port = 456;
    int result = extractIPv4(input, &address, &port);
    if (result != found || address != expected_address || port != expected_port) {
        fprintf(stderr, "FAIL: %s: got (%d, %lu, %d)\n", input, result, address, port);
        assert(0);
    }
}

int main(void)
{
    check("0.0.0.0", 1, 0UL, -1);
    check("255.255.255.255:65535", 1, 4294967295UL, 65535);
    check("host=192.168.1.2:0!", 1, 3232235778UL, 0);
    check("1.2.3.4:80 and 5.6.7.8", 1, 16909060UL, 80);
    check("256.1.2.3 / 10.20.30.40", 1, 169090600UL, -1);
    check("1.2.3.4:65536", 0, 0UL, -1);
    check("1.2.3.4:", 0, 0UL, -1);
    check("1.2.3.4:01", 0, 0UL, -1);
    check("01.2.3.4", 0, 0UL, -1);
    check("1.2.3.004", 0, 0UL, -1);
    check("1234.2.3.4", 0, 0UL, -1);
    check("1.2.3.4.5", 0, 0UL, -1);
    check("1.2.3.4:80:9", 0, 0UL, -1);
    check("1.2.3.4:123456", 0, 0UL, -1);
    check("1.2.3.4.", 0, 0UL, -1);
    check(".1.2.3.4", 0, 0UL, -1);
    check(":1.2.3.4", 0, 0UL, -1);
    check("1..2.3.4", 0, 0UL, -1);
    check("1.2.3", 0, 0UL, -1);
    check("1.2.3.4.5", 0, 0UL, -1);
    check("1.2.3.4::80", 0, 0UL, -1);
    check("1.2.3.4:80.9", 0, 0UL, -1);
    check("1.2.3.4:00000", 0, 0UL, -1);
    check("1.2.3.4:99999", 0, 0UL, -1);
    check("0.0.0.0:0", 1, 0UL, 0);
    check("1.2.3.4:65535", 1, 16909060UL, 65535);
    check("1.2.3.4:65536 / 5.6.7.8:42", 1, 84281096UL, 42);
    check("192a168.1.1.1", 1, 2818638081UL, -1);
    check("1.2.3.4x", 1, 16909060UL, -1);
    check("", 0, 0UL, -1);
    check("text only", 0, 0UL, -1);
    puts("All tests passed.");
    return 0;
}
