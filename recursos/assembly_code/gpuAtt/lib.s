.equ sys_open, 5
.equ sys_map, 192
.equ page_len, 4096
.equ prot_read, 1
.equ prot_write, 2
.equ map_shared, 1

.global mem_map
.global set_background_block
.global clear_background

.macro print value display
	 add r6, r10, #\display
	 mov r0, #\value
	 str r0, [r6]
.endm

.type mem_map, %function
mem_map:
    ldr r0, =fileName
    mov r1, #2
    mov r2, #0
    mov r7, #sys_open    
    svc 0 

    movs r11, r0
     
    mov r0, #0
    mov r1, #page_len
    mov r2, #(prot_read + prot_write)
    mov r3, #map_shared
    mov r4, r11
    ldr r5, =fpga_bridge
    ldr r5, [r5]
    mov r7, #sys_map      
    svc 0

    movs r10, r0
    
    bl clear_background

.type set_background_block %function
set_background_block:
    @ ldr r0, =DATA_A_BASE
    
    add r3, r10, #0x80 @ Carrega DATA_A_BASE em r3
    
    mov r1, #200
    mov r0, #80
    mul r1, r0, r1
    lsl r1, #4
    orr r1, #0b0010  @opcode
    str r1, [r3]

    @ ldr r0, =DATA_B_BASE
    add r4, r10, #0x70 @ Carrega DATA_B_BASE em r4
    
    mov r1, #0b111
    lsl r1, #3
    orr r1, #0b000
    lsl r1, #3
    orr r1, #0b111
    str r1, [r4]

    @ ldr r0, =start_signal
    add r5, r10, #0xc0 @ Carrega start_signal em r5

    mov r1, #0 
    str r1, [r5]
 
    mov r1, #1 
    str r1, [r5]

    bl done

@ WBR - 000 000 000 00000 0000
.type clear_background, %function
clear_background:
    @ ldr r0, =DATA_A_BASE
    
    add r3, r10, #0x80 @ Carrega DATA_A_BASE em r3
   
    mov r1, #0b00000 @ coluna
    lsl r1, #4
    orr r1, #0b0000  @opcode
    str r1, [r3]

    @ ldr r0, =DATA_B_BASE
    add r4, r10, #0x70 @ Carrega DATA_B_BASE em r4
    
    mov r1, #0b111
    lsl r1, #3
    orr r1, #0b000
    lsl r1, #3
    orr r1, #0b111
    str r1, [r4]

    @ ldr r0, =start_signal
    add r5, r10, #0xc0 @ Carrega start_signal em r5

    mov r1, #0 
    str r1, [r5]
 
    mov r1, #1 
    str r1, [r5]

    bl done

done:
	mov r7, #1
	mov r0, #1
	svc 0

.data
fileName: .asciz "/dev/mem"
  failed: .ascii "Erro no mapeamento de memoria!!\n"
fpga_bridge: .word 0xff200

start_signal: .word 0xc0
DATA_A_BASE:                     .word 0x80
DATA_B_BASE:                     .word 0x70
RESET_PULSECOUNTER_BASE:         .word 0x90
SCREEN_BASE:                     .word 0xa0
WRFULL_BASE:                     .word 0xb0
WRREG_BASE:                      .word 0xc0
