#ifndef _GRAPHIC_PROCESSOR_H
#define _GRAPHIC_PROCESSOR_H

#define MASX_TO_SHIFT_X 0b00011111111110000000000000000000
#define MASX_TO_SHIFT_Y 0b00000000000001111111111000000000

/*---- Constants that define the direction of a sprite -----*/
#define LEFT   		 0
#define UPPER_RIGHT  1
#define UP 			 2
#define UPPER_LEFT   3
#define RIGHT  		 4
#define BOTTOM_LEFT  5
#define DOWN         6
#define BOTTOM_RIGHT 7

/*-------Defining data relating to mobile sprites -------------------------------------------------------------------------*/
typedef struct{
	int coord_x;          //current x-coordinate of the sprite on screen.
	int coord_y;          //current y-coordinate of the sprite on screen.
	int direction;        //variable that defines the sprite's movement angle.
	int offset;           //Variable that defines the sprite's memory offset. Used to choose which animation to use.
	int data_register;    //Variable that defines in which register the data relating to the sprite will be stored.
	int step_x; 		  //Number of steps the sprite moves on the X axis.
	int step_y; 		  //Number of steps the sprite moves on the Y axis.
	int ativo;
	int collision;        // 0 - without collision , 1 - collision detected
} Sprite;

/*-------Defining data relating to fixed sprites -------------------------------------------------------------------------*/
typedef struct{
	int coord_x;          //current x-coordinate of the sprite on screen.
	int coord_y;          //current y-coordinate of the sprite on screen.
	int offset;           //Variable that defines the sprite's memory offset. Used to choose which animation to use.
	int data_register;    //Variable that defines in which register the data relating to the sprite will be stored.
	int ativo;
} Sprite_Fixed;

void set_sprite(int registrador, int x, int y, int offset, int activation_bit);

void set_background_color(int R, int G, int B);

void set_background_block(int column, int line, int R, int G, int B);

void setPolygon(int address, int opcode, int color, int form, int mult, int ref_point_x, int ref_point_y);

void sendInstruction(unsigned long dataA, unsigned long dataB);

void closeMappingMemory();

/* ================================================================================
Return:
	- -1: Fail
	-  1: Success
==================================================================================*/
int createMappingMemory();

/* ================================================================================
Description: Function used to increase/decrement the positions of a sprite according to its movement angle.
Parameters:
	- *sp: Passing by reference to the struct that stores the sprite data.
	- mirror: Enables(1)/disables(0) sprite coordinate mirroring if it exceeds the screen limits.
===================================================================================*/
void increase_coordinate(Sprite *sp, int mirror);

/* ================================================================================
Description: Function used to check if there is a collision between any two sprites.
Parameters:
	- *sp1 e *sp2: Passing by reference to the structs that store the sprites data.
Retorno:
	- 0: without collision
	- 1: collision detected
===================================================================================*/
int collision(Sprite *sp1, Sprite *sp2);

/* =================================================================================
Description: Function that checks whether the instruction FIFOs are full.
Return:
	- 0: False
	- 1: True
===================================================================================*/
int isFull();

/* ================================================================================
Description: Function used to count the rendering time for the number of screens specified by the "limit" variable.
===================================================================================*/
void waitScreen(int limit);

#endif	/* _GRAPHIC_PROCESSOR_H */