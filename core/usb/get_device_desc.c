#include "device.h"

#include <stdio.h>
#include <stdlib.h>

static void dump(void *data, size_t size) {
    uint8_t *ptr = data;
    while (size--)
        printf("%02X", *ptr++);
    putchar('\n');
}

int usb_get_device_desc_device(usb_event_t *event) {
    usb_event_type_t type = event->type;
    event->type = USB_INIT_EVENT;
    switch (type) {
        case USB_INIT_EVENT: {
            static const uint8_t setup[] = {0x80, 0x06, 0x00, 0x01, 0x00, 0x00, 0x08, 0x00};
            event->type = USB_TRANSFER_EVENT;
            event->info.transfer.buffer = (uint8_t *)setup;
            event->info.transfer.length = sizeof(setup);
            event->info.transfer.endpoint = 0;
            event->info.transfer.setup = true;
            event->info.transfer.direction = false;
            event->context = (void *)0;
            break;
        }
        case USB_TRANSFER_EVENT: {
            dump(event->info.transfer.buffer, event->info.transfer.length);
            switch ((uintptr_t)event->context) {
                case 0:
                case 2:
                    event->type = USB_TRANSFER_EVENT;
                    event->info.transfer.buffer = NULL;
                    event->info.transfer.length = 0;
                    event->info.transfer.endpoint = 0;
                    event->info.transfer.setup = false;
                    event->info.transfer.direction = false;
                    break;
                case 1: {
                    static const uint8_t setup[] = {0x80, 0x06, 0x00, 0x02, 0x00, 0x00, 0x40, 0x00};
                    event->type = USB_TRANSFER_EVENT;
                    event->info.transfer.buffer = (uint8_t *)setup;
                    event->info.transfer.length = sizeof(setup);
                    event->info.transfer.endpoint = 0;
                    event->info.transfer.setup = true;
                    event->info.transfer.direction = false;
                    break;
                }
                case 3:
                    event->type = USB_DESTROY_EVENT;
                    break;
            }
            event->context = (void *)((uintptr_t)event->context + 1);
            break;
        }
    }
    return 0;
}
