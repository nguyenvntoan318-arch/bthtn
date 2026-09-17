#ifndef APP_H
#define APP_H
#include <stdint.h>
typedef struct { uint8_t on; uint8_t duty; } App;
typedef enum { CMD_BAD, CMD_CHANGED, CMD_STATUS } CommandResult;
void app_init(App *a);
CommandResult app_command(App *a, const char *s);
uint8_t app_output(const App *a);
/* out phai co it nhat 48 byte. */
void app_status(const App *a, char out[48]);
#endif

