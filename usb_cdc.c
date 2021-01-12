#include <xc.h>
#include <string.h>
#include "usb_cdc.h"
#include "usbdesc.h"
#include "usb.h"

#define CDC_READY 0
#define CDC_BUSY 1

#define FIFO_SIZE 16

uint8_t cdc_state;

typedef struct fifo {
    char buf[FIFO_SIZE];
    uint16_t head;
    uint16_t tail;
    uint16_t count;
} cfifo_t;

char txbuf[8];

cfifo_t fifo_in, fifo_out;

extern int usb_cdc_callback(uint8_t ep, uint8_t* buf, uint8_t len);

static volatile LineCoding_t line;
extern uint8_t ep_data_buffer[136];
extern BD_endpoint_t endpoints[EP_NUM_MAX];

int put_fifo(cfifo_t* fifo, uint8_t* d, uint8_t len) {
    int t;
    if (len + fifo->count > FIFO_SIZE) return -1;
    if (len + fifo->head > FIFO_SIZE) {
        t = FIFO_SIZE - fifo->head;
        memcpy(&fifo->buf[fifo->head], &d[0], t);
        memcpy(&fifo->buf[0], &d[t], (fifo->head + len) - FIFO_SIZE);
        fifo->head = (fifo->head + len) - FIFO_SIZE;
    } else {
        memcpy(&fifo->buf[fifo->head], &d[0], len);
        fifo->head += len;
    }
    fifo->count += len;
    return len;
}

int get_fifo(cfifo_t* fifo, uint8_t* d, uint8_t len) {
    int t;
    if (fifo->count - len < 0) return -1;
    if (fifo->tail + len > FIFO_SIZE) {
        t = FIFO_SIZE - fifo->tail;
        memcpy(&d[0], &fifo->buf[fifo->tail], t);
        memcpy(&d[t], &fifo->buf[0], (fifo->tail + len) - FIFO_SIZE);
        fifo->tail = (fifo->tail + len) - FIFO_SIZE;
    } else {
        memcpy(&d[0], &fifo->buf[fifo->tail], len);
        fifo->tail += len;
    }
    fifo->count -= len;
    return len;
}

void cdc_init_endpoints() {
    configure_ep_in(1, &ep_data_buffer[EP1_IN_OFFSET], EP1_BUFF_SIZE);
    configure_ep_out(1, &ep_data_buffer[EP1_OUT_OFFSET], EP1_BUFF_SIZE);
    configure_ep_in(2, &ep_data_buffer[EP2_IN_OFFSET], EP2_BUFF_SIZE);
    UEP1 = EP_IN | EP_OUT | EP_HSHK;
    UEP2 = EP_IN | EP_HSHK;
    usb_set_tx_packet(1, 0, 0);
    usb_set_rx_packet(1, EP1_BUFF_SIZE);
    usb_set_tx_packet(2, 0, 0);
    cdc_state = CDC_READY;
}

void process_cdc_request(USB_SETUP_t *usb_setup) {
    switch (usb_setup->bRequest) {
        case GET_LINE_CODING:
            ctl_send(&line, 7);
            break;
        case SET_LINE_CODING:
            ctl_recv(&line, 7);
            break;
        case SET_CONTROL_LINE_STATE:
        case SEND_BREAK:
        case SEND_ENCAPSULATED_COMMAND:
            ctl_ack();
            break;
        default:
            ep0_stall();
    }
}

void cdc_request_xfer_complete(USB_SETUP_t *usb_setup) {
    switch (usb_setup->bRequest) {
        case GET_LINE_CODING:
            break;
        case SET_LINE_CODING:
            if ((line.DTERRate >= 75 && line.DTERRate <= 512000) && (line.DataBits >= 7 && line.DataBits <= 9)) {
//                    init_usart(line.DTERRate, line.DataBits);
            }
            break;
        case SET_CONTROL_LINE_STATE:
        case SEND_BREAK:
        case SEND_ENCAPSULATED_COMMAND:
            break;
    }
}

void init_cdc() {
    line.DTERRate = 9600;
    line.ParityType = 0;
    line.CharFormat = 0;
    line.DataBits = 8;

    fifo_in.head = 0;
    fifo_in.tail = 0;
    fifo_in.count = 0;

    fifo_out.head = 0;
    fifo_out.tail = 0;
    fifo_out.count = 0;

    cdc_state = CDC_BUSY;
}

void send_cdc_buf(uint8_t* buf, uint8_t len) {
       if (cdc_state == CDC_READY)
        put_fifo(&fifo_in, buf, len);
}

int cdc_get_rx_bytes() {
    return fifo_out.count;
}

int recv_cdc_buf(uint8_t* buf, uint8_t len) {
    int rlen;
    if (cdc_state == CDC_READY) rlen = get_fifo(&fifo_in, buf, len);
    return rlen;
}

void handle_cdc_in() {
    uint8_t pkt_len = MIN(fifo_in.count, EP1_BUFF_SIZE);
    if (get_fifo(&fifo_in, &txbuf[0], pkt_len)>0) {
        usb_set_tx_packet(1, &txbuf[0], pkt_len);
    } else usb_set_tx_packet(1, 0, 0);
}

void handle_cdc_out() {
    if (put_fifo(&fifo_out, endpoints[1].out.ADR, endpoints[1].out.CNT) > 0) {
        usb_cdc_callback(1, 0, 0);
        usb_set_rx_packet(1, EP1_BUFF_SIZE);
    }
}