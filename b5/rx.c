#include "rx.h"
void rx_error(Receiver *r) {
    r->errors |= RX_ERROR; r->discard = 1; r->length = 0;
}
void rx_byte(Receiver *r, char c) {
    if (c == '\r' || c == '\n') { return; }
    if (c == '!') {
        if (!r->discard && r->length) {
            uint8_t next = (uint8_t)((r->head + 1u) % RX_SLOTS);
            if (next == r->tail) { r->errors |= RX_FULL; }
            else {
                for (unsigned i = 0; i < r->length; ++i) {
                    r->queue[r->head][i] = r->work[i];
                }
                r->queue[r->head][r->length] = '\0';
                r->head = next;
            }
        }
        r->length = 0; r->discard = 0;
        return;
    }
    if (r->discard) { return; }
    if (r->length == RX_SIZE - 1u || c < 32 || c > 126) {
        r->errors |= RX_LONG; r->discard = 1; r->length = 0;
        return;
    }
    r->work[r->length++] = c;
}
int rx_pop(Receiver *r, char out[RX_SIZE]) {
    if (r->tail == r->head) { return 0; }
    unsigned i = 0;
    do { out[i] = r->queue[r->tail][i]; } while (out[i++] != '\0');
    r->tail = (uint8_t)((r->tail + 1u) % RX_SLOTS);
    return 1;
}
