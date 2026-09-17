#include "app.h"

static int equal(const char *a, const char *b) {
    while (*a && *a == *b) { ++a; ++b; }
    return *a == *b;
}
void app_init(App *a) { a->on = 0; a->duty = 50; }
uint8_t app_output(const App *a) { return a->on ? a->duty : 0; }

CommandResult app_command(App *a, const char *s) {
    if (equal(s, "ON")) { a->on = 1; return CMD_CHANGED; }
    if (equal(s, "OFF")) { a->on = 0; return CMD_CHANGED; }
    if (equal(s, "Status")) { return CMD_STATUS; }
    const char *prefix = "PWM:";
    while (*prefix) {
        if (*s++ != *prefix++) { return CMD_BAD; }
    }
    unsigned value = 0, digits = 0;
    while (*s >= '0' && *s <= '9') {
        if (++digits > 3) { return CMD_BAD; }
        value = value * 10 + (unsigned)(*s++ - '0');
    }
    if (!digits || value > 100 || *s != '%' || s[1] != '\0') {
        return CMD_BAD;
    }
    a->duty = (uint8_t)value; /* Khong thay doi a->on! */
    return CMD_CHANGED;
}

static char *append(char *p, const char *s) {
    while (*s) { *p++ = *s++; }
    return p;
}
static char *number(char *p, unsigned n) {
    if (n >= 100) { *p++ = '1'; }
    if (n >= 10) { *p++ = (char)('0' + (n / 10) % 10); }
    *p++ = (char)('0' + n % 10);
    return p;
}
void app_status(const App *a, char out[48]) {
    char *p = append(out, a->on ? "STATE=ON; PWM=" : "STATE=OFF; PWM=");
    p = number(p, a->duty);
    p = append(p, "%; OUTPUT=");
    p = number(p, app_output(a));
    p = append(p, "%\r\n");
    *p = '\0';
}
