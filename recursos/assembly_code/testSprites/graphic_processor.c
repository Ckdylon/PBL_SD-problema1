#include <stdio.h>
#include <stdint.h>
#include "graphic_processor.h"
#include "hps_0.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define HW_REGS_BASE 0xfc000000
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 ) //0x3FFFFFF
#define ALT_LWFPGASLVS_OFST 0xff200000

void *virtual_base;
void *h2p_lw_dataA_addr;
void *h2p_lw_dataB_addr;
void *h2p_lw_wrReg_addr;
void *h2p_lw_wrFull_addr;
void *h2p_lw_screen_addr;
void *h2p_lw_result_pulseCounter_addr;
int fd;

int createMappingMemory(){
	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
		printf( "[ERROR]: could not open \"/dev/mem\"...\n" );
		return -1;
	}
	virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_REGS_BASE );
	if( virtual_base == MAP_FAILED ) {
		printf( "[ERROR]: mmap() failed...\n" );
		close( fd );
		return -1;
	}
	h2p_lw_dataA_addr  = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + DATA_A_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
	h2p_lw_dataB_addr  = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + DATA_B_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
	h2p_lw_wrReg_addr  = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + WRREG_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
	h2p_lw_wrFull_addr = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + WRFULL_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
	h2p_lw_screen_addr = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + SCREEN_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
	h2p_lw_result_pulseCounter_addr = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + RESET_PULSECOUNTER_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
	return 1;
}

void closeMappingMemory(){
	// clean up our memory mapping and exit
	if( munmap( virtual_base, HW_REGS_SPAN ) != 0 ) {
		printf( "[ERROR]: munmap() failed...\n" );
		close(fd);
	}
}

/* ================================================================================
Function to verifies if the FIFO is full.
===================================================================================*/
int isFull(){
	return *(uint32_t *) h2p_lw_wrFull_addr;
}

void sendInstruction(unsigned long dataA, unsigned long dataB){
	if(isFull() == 0){
		*(uint32_t *) h2p_lw_wrReg_addr = 0;
		*(uint32_t *) h2p_lw_dataA_addr = dataA;
		*(uint32_t *) h2p_lw_dataB_addr = dataB;
		*(uint32_t *) h2p_lw_wrReg_addr = 1;
		*(uint32_t *) h2p_lw_wrReg_addr = 0;
	}
}

/* ================================================================================
Function to defines the dataA bus of the instruction.
===================================================================================*/
static unsigned long dataA_builder(int opcode, int reg, int memory_address){
	unsigned long data = 0;
	switch(opcode){
		case(0):
			data = data | reg;                  
			data = data << 4;                   
			data = data | opcode;               
			break;
		case(1):
		case(2):
		case(3):
			data = data | memory_address;   
			data = data << 4;      
			data = data | opcode;
			break;
	}
	return data;
}

void setPolygon(int address, int opcode, int color, int form, int mult, int ref_point_x, int ref_point_y){
	unsigned long dataA = dataA_builder(opcode, 0, address);
	unsigned long dataB = 0;
	dataB = form;
	dataB = dataB << 9;
	dataB = dataB | color;
	dataB = dataB << 4;
	dataB = dataB | mult;
	dataB = dataB << 9;
	dataB = dataB | ref_point_y;
	dataB = dataB << 9;
	dataB = dataB | ref_point_x;
	sendInstruction(dataA, dataB);
}

void set_sprite(int registrador, int x, int y, int offset, int activation_bit){
	unsigned long dataA = dataA_builder(0,registrador,0);
	unsigned long dataB = 0; 
	dataB = dataB | activation_bit; 
    dataB = dataB << 10;
    dataB = dataB | x;      		
    dataB = dataB << 10;    		
    dataB = dataB | y;      		
    dataB = dataB << 9;     		
    dataB = dataB | offset;
	sendInstruction(dataA, dataB);
}

void set_background_color(int R, int G, int B){
	unsigned long dataA = dataA_builder(0,0,0);
	unsigned long color = B;
	color = color << 3;
	color = color | G;
	color = color << 3;
	color = color | R;
	sendInstruction(dataA, color);
}

void set_background_block(int column, int line, int R, int G, int B){
	int address = (line * 80) + column;
	unsigned long dataA = dataA_builder(2, 0, address);
	unsigned long color = B;
	color = color << 3;
	color = color | G;
	color = color << 3;
	color = color | R;
	sendInstruction(dataA, color);
}

void waitScreen(int limit){
	int screens = 0;
 	while(screens <= limit){ // Wait x seconds for restart Game 
		if(*(uint32_t *) h2p_lw_screen_addr == 1){ // Checks if a screen has finished drawing.
			// Structure for counter of screen and set parameters.
			screens++;
			*(uint32_t *) h2p_lw_result_pulseCounter_addr = 1;
			*(uint32_t *) h2p_lw_result_pulseCounter_addr = 0;
		}
	}
}

void increase_coordinate(Sprite *sp, int mirror){
	switch((*sp).direction){
		case LEFT:                   							//0 degrees
			(*sp).coord_x -= (*sp).step_x; 						
			if(mirror == 1){
				if((*sp).coord_x < 1){                          //Mirrors the sprite's position when it reaches the left edge of the screen.
					(*sp).coord_x = 640;
				}
			}else {
				if((*sp).coord_x < 1){
					(*sp).coord_x = 1;
				}
			}
			break;
		case UPPER_RIGHT:                                       //45 degrees
			(*sp).coord_x += (*sp).step_x; 						
			(*sp).coord_y -= (*sp).step_y; 						
			if(mirror == 1){
				if((*sp).coord_y < 0){                          //Mirrors the sprite's position when it reaches the upper limit of the screen.
					(*sp).coord_y = 480;
				}else if((*sp).coord_x > 640){                  //Mirrors the sprite's position when it reaches the right edge of the screen.
					(*sp).coord_x = 0;	
				}
			}else{
				if((*sp).coord_y < 0){
					(*sp).coord_y = 0;
				}else if((*sp).coord_x > 640){
					(*sp).coord_x = 640;
				}
			}
			break;
		case UP:                                                //90 degrees
			(*sp).coord_y -= (*sp).step_y; 						
			if(mirror == 1){
				if((*sp).coord_y < 0){                          //Mirrors the sprite's position when it reaches the upper limit of the screen.
					(*sp).coord_y = 480;
				}
			}else{
				if((*sp).coord_y < 0){
					(*sp).coord_y = 0;
				}			
			}
			break;
		case UPPER_LEFT:          							    //135 degrees
			(*sp).coord_x -= (*sp).step_x; 						
			(*sp).coord_y -= (*sp).step_y; 						
			if(mirror == 1){
				if((*sp).coord_y < 0){                          //Mirrors the sprite's position when it reaches the upper limit of the screen.
					(*sp).coord_y = 480;
				}else if((*sp).coord_x < 1){                    //Mirrors the sprite's position when it reaches the left edge of the screen.
					(*sp).coord_x = 640;
				}
			}else{
				if((*sp).coord_y < 0){
					(*sp).coord_y = 0;
				}else if((*sp).coord_x < 1){
					(*sp).coord_x = 1;
				}
			}
			break;
		case RIGHT:          									//180 degrees
			(*sp).coord_x += (*sp).step_x; 						//Atualiza a coordenada X.
			if(mirror == 1){
				if((*sp).coord_x > 640){                        //Mirrors the sprite's position when it reaches the right edge of the screen.
					(*sp).coord_x = 0;
				}
			}else{
				if((*sp).coord_x > 620){
					(*sp).coord_x = 620;
				}
			}
			break;
		case BOTTOM_LEFT:          								//225 degrees
			(*sp).coord_x -= (*sp).step_x; 						
			(*sp).coord_y += (*sp).step_y; 						
			if(mirror == 1){
				if((*sp).coord_y > 480){                        //Mirrors the sprite's position when it reaches the lower limit of the screen.
					(*sp).coord_y = 0;
				}else if((*sp).coord_x < 1){                    //Mirrors the sprite's position when it reaches the left edge of the screen.
					(*sp).coord_x = 640;
				}
			}else{
				if((*sp).coord_y > 480){
					(*sp).coord_y = 480;
				}else if((*sp).coord_x < 1){
					(*sp).coord_x = 1;
				}
			}
			break;
		case DOWN:                                              //270 graus (pra baixo)
			(*sp).coord_y += (*sp).step_y; 						
			if(mirror == 1){
				if((*sp).coord_y > 480){                        //Mirrors the sprite's position when it reaches the lower limit of the screen.
					(*sp).coord_y = 0;
				}
			}else{
				if((*sp).coord_y > 480){
					(*sp).coord_y = 480;
				}
			}
			break;
		case BOTTOM_RIGHT:                                      //315 degrees
			(*sp).coord_x += (*sp).step_x; 						
			(*sp).coord_y += (*sp).step_y; 						
			if(mirror == 1){
				if((*sp).coord_y > 480){                        //Mirrors the sprite's position when it reaches the lower limit of the screen.
					(*sp).coord_y = 0;
				}else if((*sp).coord_x > 640){                  //Mirrors the sprite's position when it reaches the right edge of the screen.
					(*sp).coord_x = 0;
				}
			}else{
				if((*sp).coord_y > 480){
					(*sp).coord_y = 480;
				}else if((*sp).coord_x > 640){
					(*sp).coord_x = 640;
				}
			}
			break;
	}
}

int collision(Sprite *sp1, Sprite *sp2){
	int h = 15;
	int y_face_1 = (*sp1).coord_y + h;
	int y_face_2 = (*sp2).coord_y + h;
	int x_face_1 = (*sp1).coord_x + h;
	int x_face_2 = (*sp2).coord_x + h;
	/*-------------------------------------*/   
	if( (y_face_1 > (*sp2).coord_y ) && ((*sp1).coord_y < y_face_2) ){
		//Colisoes a esquerda --------------------------------------------
		if( ( x_face_1 > (*sp2).coord_x ) && (x_face_1 < x_face_2) ){
			return 1;
		}else if ( (x_face_1 < x_face_2) && (x_face_1 > (*sp2).coord_x) ){
			return 1;
		}
		/*--------------------------------------------------------------*/
		//Colisoes a direita --------------------------------------------
		if( ( x_face_1 > x_face_2) && (x_face_2 > (*sp1).coord_x) ){
			return 1;
		}else if( ( x_face_1 > x_face_2) && (x_face_2 > (*sp1).coord_x) ){
			return 1;
		}
		/*--------------------------------------------------------------*/
	}
	return 0;
}
