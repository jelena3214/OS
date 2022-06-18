.extern ecall
.global supervisorTrap
.align 4
supervisorTrap:
    addi sp, sp, -256

    .irp index, 0,1, 2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    sd x\index, \index * 8(sp)
    .endr

    call ecall

    .irp index, 0,1, 2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    ld x\index, \index * 8(sp)
    .endr
    addi sp, sp, 256
    addi sp, sp, -8
    sd x5, 0x00(sp)
    csrr x5, sepc
    addi x5, x5, 4
    csrw sepc, x5
    ld x5, 0x00(sp)
    addi sp, sp , 8
    sret