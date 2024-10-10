.section .text
.global show

.type show, %function
show:
    ldr r0, =1
    ldr r1, =message
    ldr r2, =message_length
    mov r7, #4
    svc 0

    mov r0, #0
    mov r7, #1
    svc 0

.section .data
message:
    .asciz "Testandoooo!!!!\n"
message_length = . - message
