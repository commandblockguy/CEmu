#ifdef PIPE_SUPPORT

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "../cpu.h"
#include "../emu.h"
#include "../mem.h"
#include "pipe.h"

int fd_in = NULL;
int fd_out = NULL;
bool initted = false;

void pipe_init(void) {
    mkfifo(PIPE_IN_FIFO, 0666);
    mkfifo(PIPE_OUT_FIFO, 0666);

    fd_out = open(PIPE_OUT_FIFO, O_WRONLY | O_NONBLOCK);
    fd_in  = open(PIPE_IN_FIFO,  O_RDONLY | O_NONBLOCK);

    initted = true;
}

void pipe_check_command(void) {
    if(!initted) pipe_init();
    cpu.registers.A = 1;
}

void pipe_send_data_command(void) {
    uint32_t size = cpu.registers.bc.hl;
    uint32_t address = cpu.registers.hl.hl;
    void *buf;

    //gui_console_printf("Sending %u bytes from %06x\n", size, address);

    if(!initted) pipe_init();

    if(!fd_out || !(buf = malloc(size))) {
        cpu.registers.bc.hl = 0;
        return;
    }

    virt_mem_cpy(buf, address, size);

    write(fd_out, buf, size);

    free(buf);
}

void pipe_receive_data_command(void) {
    uint32_t size = cpu.registers.bc.hl;
    uint32_t address = cpu.registers.de.hl;
    int32_t gotten;
    uint8_t *buf;

    // gui_console_printf("Receiving up to %u bytes to %06x\n", size, address);

    if(!initted) pipe_init();

    buf = phys_mem_ptr(address, size);

    if(!fd_in || !buf) {
        cpu.registers.bc.hl = 0;
        return;
    }

    gotten = read(fd_in, buf, size);

    // gui_console_printf("Got %i bytes\n", gotten, address);

    if(gotten >= 0) {
        cpu.registers.bc.hl = gotten;
    } else {
        cpu.registers.bc.hl = 0;
    }
}

#endif
