.global uart_put_char
.global uart_get_char

# leaf functions, so no stack frame needed
uart_put_char:
    la t0, __uart_base # load uart base
put_poll:
    lbu t1, 5(t0) # read LSR
    andi t1, t1, 0x20 # mask bit 5 of LSR (THR empty bit)
    beqz t1, put_poll # if 0, buffer full, so keep checking
    sb a0, 0(t0) # place byte into THR
    ret

uart_get_char:
    la t0, __uart_base # load uart base
get_poll:
    lbu t1, 5(t0) # read LSR
    andi t1, t1, 0x1 # mask bit 0 of LSR (data available bit)
    beqz t1, get_poll # if 0, no data yet, so keep checking
    lbu a0, 0(t0) # take byte from RBR and place into a0 for callee to receive on return
    ret
