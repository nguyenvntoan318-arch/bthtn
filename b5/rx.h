#ifndef RX_H
#define RX_H
#include <stdint.h>
#define RX_SIZE 32u
#define RX_SLOTS 4u
enum { RX_LONG = 1, RX_FULL = 2, RX_ERROR = 4 };
typedef struct {
    char work[RX_SIZE];
    char queue[RX_SLOTS][RX_SIZE];
    unsigned length;
    uint8_t discard;
    volatile uint8_t head, tail, errors;
} Receiver;
void rx_byte(Receiver *r, char c);
void rx_error(Receiver *r);
/* Goi rx_pop voi ngat tam tat de bao ve hang doi. */
int rx_pop(Receiver *r, char out[RX_SIZE]);
#endif
