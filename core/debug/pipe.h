#ifndef PIPE_H
#define PIPE_H

#ifdef PIPE_SUPPORT

#define PIPE_OUT_FIFO "/tmp/cemu-fromcalc"
#define PIPE_IN_FIFO "/tmp/cemu-tocalc"

// sets a to 1 if enabled
#define PIPE_CHECK_COMMAND 2
// hl: data
// bc: size
#define PIPE_SEND_DATA_COMMAND 3
// bc: size
// de: destination
#define PIPE_RECV_DATA_COMMAND 4

void pipe_init(void);
void pipe_check_command(void);
void pipe_send_data_command(void);
void pipe_receive_data_command(void);

#endif

#endif //PIPE_H
