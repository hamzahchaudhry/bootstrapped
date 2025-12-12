.section .init
.globl _start
_start:
    la   sp, _stack_top      # from linker script
    call kernel_main
1:  j 1b                     # if kernel_main returns, spin

