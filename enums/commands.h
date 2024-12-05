#ifndef commands_H
#define commands_H

typedef enum{
    not_command = -1,
    mov = 0,
    cmp = 1,
    add = 2,
    sub = 3,
    lea = 4,
    clr = 5,
    not = 6,
    inc = 7,
    dec = 8,
    jmp = 9,
    bne = 10,
    red = 11,
    prn = 12,
    jsr = 13,
    rts = 14,
    stop = 15
}commands;

#endif