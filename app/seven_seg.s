.equ sys_open, 5
.equ sys_map, 192
.equ page_len, 4096
.equ prot_read, 1
.equ prot_write, 2
.equ map_shared, 1

.global _start

@ Mapeia o endereço de memória virtual
.macro open_mem
     ldr r0, =fileName
     mov r7, #sys_open    
     svc 0 
     movs r4, r0
     
     mov r0, #0
     mov r1, #page_len
     mov r2, #(prot_read + prot_write)
     mov r3, #map_shared
     ldr r5, =fpga_bridge
     ldr r5, [r5]
     mov r7, #sys_map      
     svc 0 
     movs r8, r0
.endm

_start:
    open_mem
    mov r0, #255
    add r6, r8, #0x60
    str r0, [r6]

.data
fileName: .asciz "/dev/mem"
hex_0: .word 0x60       @ Assuming this is the virtual address where display control resides
fpga_bridge: .word 0xff200 @ 0xf200000 / 4096 que é o tamanho da página
