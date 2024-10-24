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



@   De r0 a r4 para operações
@   r5 e r6 para dataA e dataB
 


.text

.macro isFull
    push {lr}

    ldr r0,=h2p_lw_wrFull_addr

    pop {lr}
    bx lr
.endm


.macro sendInstruction data_A, data_B
        isFull
        cmp r0,#1
        beq done
        
        ldr h2p_lw_wrReg_addr, #0
        ldr h2p_lw_dataA_addr, data_A
        ldr h2p_lw_dataB_addr, data_B
        ldr h2p_lw_wrReg_addr,#1
        ldr h2p_lw_wrReg_addr,#0

    done:

.endm

.macro set_background_color r, g, b
    push {lr}
     
    dataA_builder 0, 0, 0
    @carrega o valor de dataA_builder retornado em r0 em r5
    ldr r5,r0

    color_builder \r, \g, \b
    sendInstruction r5,r6
    
    pop {lr}
    bx lr
.endm

.macro color_builder r, g, b 
    push {lr}
     
    mov r6, #\b
    lsl r6,#3

    orr r6,r6,#\g
    lsl r6,#3
    
    orr r6,r6,#\r
    lsl r6,#3

    pop {lr}
    bx lr
.endm

.macro set_background_block coluna, linha, r, g, b
    push {lr}
     
    dataA_builder 0, 0, 0
    ldr r5,r0
    color_builder r,g,b
    sendInstruction r5,r6

    pop {lr}
    bx lr
.endm


.macro dataA_builder opcode, reg, memory_address   
    push {lr}
    @ Parâmetros:
    @ r0 -> opcode
    @ r1 -> reg
    @ r2 -> memory_address

    mov r0,#\opcode
    mov r1,#\reg
    mov r2,#\memory_address

    @ Inicializando data com 0
    mov r3, #0  @ r3 será o registrador que vai armazenar a variável "data"

    @ Início do switch case (comparação com opcode)
    cmp r0, #0          @ compara opcode com 0
    beq case_0          @ se for igual a 0, vai para case_0

    cmp r0, #1          @ compara opcode com 1
    beq case_1_2_3      @ se for igual a 1, vai para case_1_2_3

    cmp r0, #2          @ compara opcode com 2
    beq case_1_2_3      @ se for igual a 2, vai para case_1_2_3

    cmp r0, #3          @ compara opcode com 3
    beq case_1_2_3      @ se for igual a 3, vai para case_1_2_3

    @ Caso opcode seja 0
case_0:
    orr r3, r3, r1      @ data = data | reg (r3 = r3 | r1)
    lsl r3, r3, #4      @ data = data << 4
    orr r3, r3, r0      @ data = data | opcode (r3 = r3 | r0)
    b end_switch        @ pular para o final

    @ Caso opcode seja 1, 2 ou 3
case_1_2_3:
    orr r3, r3, r2      @ data = data | memory_address (r3 = r3 | r2)
    lsl r3, r3, #4      @ data = data << 4
    orr r3, r3, r0      @ data = data | opcode (r3 = r3 | r0)

end_switch:
    @ Retornar o valor de "data" em r0
    mov r0, r3


    pop {lr}
    bx lr 
.endm

.macro waitScreen limit

    ldr r0,=screens
    ldr r1,\limit
    ldr r2,=h2p_lw_screen_addr
    ldr r3,=h2p_lw_result_pulseCounter_addr

loop:   
		LDR R4, [R2]                @ Carrega o valor da memória em h2p_lw_screen_addr
		CMP R4, #1                  @ Compara se o valor é 1
		BNE wait_screen             @ Se não for 1, vai para wait_screen

		ADD R0, R0, #1              @ Incrementa o contador de screens (screens++)
		
		MOV R4, #1                  @ Carrega o valor 1 no registrador R4
		STR R4, [R3]                @ Grava o valor 1 em h2p_lw_result_pulseCounter_addr
		MOV R4, #0                  @ Carrega o valor 0 no registrador R4
		STR R4, [R3]                @ Grava o valor 0 em h2p_lw_result_pulseCounter_addr
		
wait_screen:   
		CMP R0, R1                  @ Compara screens com limit
		BLT loop                    @ Se screens for menor que limit, repete o loop

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
	 
	 print 1 0x10
	 print 1 0x20
	 print 1 0x30
	 print 1 0x40
	 print 1 0x50

   set_background_color 0b111, 0b000, 0b000

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
failed: .ascii "Erro no mapeamento de memoria!!\n"
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

screens:                    .word 0 @número inicial de telas a serem usadas na função waitScreen

