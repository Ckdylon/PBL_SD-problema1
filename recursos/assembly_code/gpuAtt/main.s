.equ sys_open, 5
.equ sys_map, 192
.equ page_len, 4096
.equ prot_read, 1
.equ prot_write, 2
.equ map_shared, 1

.global _start

.macro print value display
	 add r6, r8, #\display
	 mov r0, #\value
	 str r0, [r6]
.endm

_start:
     ldr r0, =fileName
	 mov r1, #2
	 mov r2, #0
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

	 cmp r0, #-1
	 beq error

	 movs r8, r0

	  @      (offsett + mascara) correspondem ao primeiro operando da soma
    LDR r1, =ALT_LWFPGASLVS_OFST  @ Carrega o offset LWFPGASLVS em r1
    LDR r2, =HW_REGS_MASK         @ Carrega o valor da máscara HW_REGS_MASK em r2

    @utilizarei o r7 como registrador temporario para carregar o endereco da memoria e como referencia para armazenar o resultado apos a operação novamente na memoria


    @estou utilizando o r5 para manter o h2p_lw_dataA_addr
    @ Calcular h2p_lw_dataA_addr
    LDR r3, =DATA_A_BASE          @ Carrega DATA_A_BASE em r3
    ADD r5, r1, r3                @ Soma o offset com DATA_A_BASE
    AND r5, r5, r2                @ Aplica a máscara HW_REGS_MASK
    ADD r5, r8, r5                @ Soma o endereço base virtual
    
    ldr r7,=h2p_lw_dataA_addr
    STR r5, [r7]  @ Armazena o endereço calculado


    @estou utilizando o r6 para manter o h2p_lw_dataB_addr
    @ Calcular h2p_lw_dataB_addr
    LDR r3, =DATA_B_BASE          @ Carrega DATA_B_BASE em r3

    ADD r6, r1, r3                @ Soma o offset com DATA_B_BASE
    AND r6, r6, r2                @ Aplica a máscara HW_REGS_MASK
    ADD r6, r8, r6                @ Soma o endereço base virtual

    ldr r7,=h2p_lw_dataB_addr
    STR r6, [r7]  @ Armazena o endereço calculado

    @apartir desse ponto utilizarei r4 como reg temporario para as operações
    @ Calcular h2p_lw_wrReg_addr
    LDR r3, =WRREG_BASE           @ Carrega WRREG_BASE em r3
    
    ADD r4, r1, r3                @ Soma o offset com WRREG_BASE
    AND r4, r4, r2                @ Aplica a máscara HW_REGS_MASK
    ADD r4, r8, r4                @ Soma o endereço base virtual
    
    ldr r7,=h2p_lw_wrReg_addr
    STR r4, [r7]  @ Armazena o endereço calculado

    @ Calcular h2p_lw_wrFull_addr
    LDR r3, =WRFULL_BASE          @ Carrega WRFULL_BASE em r3
    
    ADD r4, r1, r3                @ Soma o offset com WRFULL_BASE
    AND r4, r4, r2                @ Aplica a máscara HW_REGS_MASK
    ADD r4, r8, r4                @ Soma o endereço base virtual
    
    ldr r7,=h2p_lw_wrFull_addr
    STR r4, [r7]  @ Armazena o endereço calculado

    @ Calcular h2p_lw_screen_addr
    LDR r3, =SCREEN_BASE          @ Carrega SCREEN_BASE em r3
    
    ADD r4, r1, r3                @ Soma o offset com SCREEN_BASE
    AND r4, r4, r2                @ Aplica a máscara HW_REGS_MASK
    ADD r4, r8, r4                @ Soma o endereço base virtual

    ldr r7,=h2p_lw_screen_addr
    STR r4, [r7]  @ Armazena o endereço calculado

    @ Calcular h2p_lw_result_pulseCounter_addr
    LDR r3, =RESET_PULSECOUNTER_BASE @ Carrega RESET_PULSECOUNTER_BASE em r3
    
    ADD r4, r1, r3                @ Soma o offset com RESET_PULSECOUNTER_BASE
    AND r4, r4, r2                @ Aplica a máscara HW_REGS_MASK
    ADD r4, r8, r4                @ Soma o endereço base virtual

    ldr r7,=h2p_lw_result_pulseCounter_addr
    STR r4, [r7]  @ Armazena o endereço calculado	 
	 
	 print 255 0x10
	 print 255 0x20
	 print 255 0x30
	 print 255 0x40
	 print 255 0x50
	 print 255 0x60

error:
	mov r0, #1
	ldr r1, =failed
	mov r2, #7
	mov r7, #4
	svc 0

	mov r7, #1
	mov r0, #1
	svc 0


.data
fileName: .asciz "/dev/mem"
failed: .ascii "failed\n"
fpga_bridge: .word 0xff200

h2p_lw_dataA_addr:					.word 0
h2p_lw_dataB_addr:					.word 0
h2p_lw_wrReg_addr:					.word 0
h2p_lw_wrFull_addr:					.word 0
h2p_lw_screen_addr:					.word 0
h2p_lw_result_pulseCounter_addr:	.word 0

HW_REGS_BASE:               .word  0xfc000000
HW_REGS_SPAN:               .word  0x04000000
HW_REGS_MASK:               .word  0x3FFFFFF @ ( HW_REGS_SPAN - 1 )
ALT_LWFPGASLVS_OFST:        .word  0xff200000


DATA_A_BASE:                     .word 0x80
DATA_B_BASE:                     .word 0x70
RESET_PULSECOUNTER_BASE:         .word 0x90
SCREEN_BASE:                     .word 0xa0
WRFULL_BASE:                     .word 0xb0
WRREG_BASE:                      .word 0xc0
