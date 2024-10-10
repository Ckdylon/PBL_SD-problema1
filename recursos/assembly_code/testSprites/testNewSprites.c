#include "graphic_processor.h"

int main(){
	Sprite sprt_1;
	Sprite sprt_2;
	Sprite sprt_3;
	Sprite sprt_4;
	Sprite sprt_5;
	Sprite sprt_6;
	Sprite sprt_7;
	Sprite sprt_8;
	Sprite sprt_9;
	Sprite sprt_10;
	Sprite sprt_11;
	Sprite sprt_12;
	Sprite sprt_13;
	Sprite sprt_14;
	Sprite sprt_15;
	Sprite sprt_16;
	Sprite sprt_17;
	Sprite sprt_18;
	Sprite sprt_19;
	Sprite sprt_20;
	Sprite sprt_21;
	Sprite sprt_22;
	Sprite sprt_23;
	Sprite sprt_24;
	Sprite sprt_25;

	sprt_1.ativo = 1; sprt_1.data_register  = 1;  sprt_1.coord_x = 0;  sprt_1.coord_y = 0;  sprt_1.offset = 0; 
	sprt_2.ativo = 1; sprt_2.data_register  = 2;  sprt_2.coord_x = 620;  sprt_2.coord_y = 0;  sprt_2.offset = 1;
	sprt_3.ativo = 1; sprt_3.data_register  = 3;  sprt_3.coord_x = 280;  sprt_3.coord_y = 100;  sprt_3.offset = 2;
	sprt_4.ativo = 1; sprt_4.data_register  = 4;  sprt_4.coord_x = 310; sprt_4.coord_y = 100;  sprt_4.offset = 3;
	sprt_5.ativo = 1; sprt_5.data_register  = 5;  sprt_5.coord_x = 340; sprt_5.coord_y = 100;  sprt_5.offset = 4;
	sprt_6.ativo = 1; sprt_6.data_register  = 6;  sprt_6.coord_x = 370; sprt_6.coord_y = 100;  sprt_6.offset = 5;
	
	sprt_7.ativo = 1; sprt_7.data_register  = 7;  sprt_7.coord_x = 220; sprt_7.coord_y = 130;  sprt_7.offset = 6;
	sprt_8.ativo = 1; sprt_8.data_register  = 8;  sprt_8.coord_x = 250; sprt_8.coord_y = 130;  sprt_8.offset = 7;
	sprt_9.ativo = 1; sprt_9.data_register  = 9;  sprt_9.coord_x = 280; sprt_9.coord_y = 130;  sprt_9.offset = 8;
	sprt_10.ativo = 1; sprt_10.data_register  = 10; sprt_10.coord_x = 310; sprt_10.coord_y = 130;  sprt_10.offset = 9;
	sprt_11.ativo = 1; sprt_11.data_register  = 11; sprt_11.coord_x = 340; sprt_11.coord_y = 130;  sprt_11.offset = 10;
	sprt_12.ativo = 1; sprt_12.data_register  = 12; sprt_12.coord_x = 370; sprt_12.coord_y = 130;  sprt_12.offset = 11;
	
	sprt_13.ativo = 1; sprt_13.data_register  = 13; sprt_13.coord_x = 220; sprt_13.coord_y = 160;  sprt_13.offset = 12;
	sprt_14.ativo = 1; sprt_14.data_register  = 14; sprt_14.coord_x = 250; sprt_14.coord_y = 160;  sprt_14.offset = 13;
	sprt_15.ativo = 1; sprt_15.data_register  = 15; sprt_15.coord_x = 280; sprt_15.coord_y = 160;  sprt_15.offset = 14;
	sprt_16.ativo = 1; sprt_16.data_register  = 16; sprt_16.coord_x = 310; sprt_16.coord_y = 160;  sprt_16.offset = 15;
	sprt_17.ativo = 1; sprt_17.data_register  = 17; sprt_17.coord_x = 340; sprt_17.coord_y = 160;  sprt_17.offset = 16;
	sprt_18.ativo = 1; sprt_18.data_register  = 18; sprt_18.coord_x = 370; sprt_18.coord_y = 160;  sprt_18.offset = 17;
	
	sprt_19.ativo = 1; sprt_19.data_register  = 19; sprt_19.coord_x = 220; sprt_19.coord_y = 190;  sprt_19.offset = 18;
	sprt_20.ativo = 1; sprt_20.data_register  = 20; sprt_20.coord_x = 250; sprt_20.coord_y = 190;  sprt_20.offset = 19;
	sprt_21.ativo = 1; sprt_21.data_register  = 21; sprt_21.coord_x = 280; sprt_21.coord_y = 190;  sprt_21.offset = 20;
	sprt_22.ativo = 1; sprt_22.data_register  = 22; sprt_22.coord_x = 310;  sprt_22.coord_y  = 190; sprt_22.offset = 21;
	sprt_23.ativo = 1; sprt_23.data_register  = 23; sprt_23.coord_x = 340;  sprt_23.coord_y  = 190; sprt_23.offset = 22;
	sprt_24.ativo = 1; sprt_24.data_register  = 24; sprt_24.coord_x = 370;  sprt_24.coord_y  = 190; sprt_24.offset = 23;
	
	sprt_25.ativo = 1; sprt_25.data_register  = 25; sprt_25.coord_x = 220; sprt_25.coord_y  = 220; sprt_25.offset = 24;


	createMappingMemory();
    // R G B
	set_background_color(0b111, 0b000, 0b000);
	//set_background_block(0, 220, 111, 0, 0);
	// void setPolygon(int address, int opcode, int color, int form, int mult, int ref_point_x, int ref_point_y);
	setPolygon(0b111, 0b111, 0b111, 0b111, 0b111, 200, 0);

	while(1){ if(isFull() == 0) { set_sprite(sprt_1.data_register, sprt_1.coord_x, sprt_1.coord_y, sprt_1.offset, sprt_1.ativo); break; } }
	while(1){ if(isFull() == 0) { set_sprite(sprt_2.data_register, sprt_2.coord_x, sprt_2.coord_y, sprt_2.offset, sprt_2.ativo); break; } }
	while(1){ if(isFull() == 0) { set_sprite(sprt_3.data_register, sprt_3.coord_x, sprt_3.coord_y, sprt_3.offset, sprt_3.ativo); break; } }
	while(1){ if(isFull() == 0) { set_sprite(sprt_4.data_register, sprt_4.coord_x, sprt_4.coord_y, sprt_4.offset, sprt_4.ativo); break; } }
	while(1){ if(isFull() == 0) { set_sprite(sprt_5.data_register, sprt_5.coord_x, sprt_5.coord_y, sprt_5.offset, sprt_5.ativo); break; } }
	while(1){ if(isFull() == 0) { set_sprite(sprt_6.data_register, sprt_6.coord_x, sprt_6.coord_y, sprt_6.offset, sprt_6.ativo); break; } }
	while(1){ if(isFull() == 0) { set_sprite(sprt_7.data_register, sprt_7.coord_x, sprt_7.coord_y, sprt_7.offset, sprt_7.ativo); break; } }
	while(1){ if(isFull() == 0) { set_sprite(sprt_8.data_register, sprt_8.coord_x, sprt_8.coord_y, sprt_8.offset, sprt_8.ativo); break; } }
	while(1){ if(isFull() == 0) { set_sprite(sprt_9.data_register, sprt_9.coord_x, sprt_9.coord_y, sprt_9.offset, sprt_9.ativo); break; } }
	while(1){ if(isFull() == 0) { set_sprite(sprt_10.data_register, sprt_10.coord_x, sprt_10.coord_y, sprt_10.offset, sprt_10.ativo); break; } }
	while(1){ if(isFull() == 0) { set_sprite(sprt_11.data_register, sprt_11.coord_x, sprt_11.coord_y, sprt_11.offset, sprt_11.ativo); break; } }
	while(1){ if(isFull() == 0) { set_sprite(sprt_12.data_register, sprt_12.coord_x, sprt_12.coord_y, sprt_12.offset, sprt_12.ativo); break; } }
	while(1){ if(isFull() == 0) { set_sprite(sprt_13.data_register, sprt_13.coord_x, sprt_13.coord_y, sprt_13.offset, sprt_13.ativo); break; } }
	while(1){ if(isFull() == 0) { set_sprite(sprt_14.data_register, sprt_14.coord_x, sprt_14.coord_y, sprt_14.offset, sprt_14.ativo); break; } }
	while(1){ if(isFull() == 0) { set_sprite(sprt_15.data_register, sprt_15.coord_x, sprt_15.coord_y, sprt_15.offset, sprt_15.ativo); break; } }
	while(1){ if(isFull() == 0) { set_sprite(sprt_16.data_register, sprt_16.coord_x, sprt_16.coord_y, sprt_16.offset, sprt_16.ativo); break; } }
	while(1){ if(isFull() == 0) { set_sprite(sprt_17.data_register, sprt_17.coord_x, sprt_17.coord_y, sprt_17.offset, sprt_17.ativo); break; } }
	while(1){ if(isFull() == 0) { set_sprite(sprt_18.data_register, sprt_18.coord_x, sprt_18.coord_y, sprt_18.offset, sprt_18.ativo); break; } }
	while(1){ if(isFull() == 0) { set_sprite(sprt_19.data_register, sprt_19.coord_x, sprt_19.coord_y, sprt_19.offset, sprt_19.ativo); break; } }
	while(1){ if(isFull() == 0) { set_sprite(sprt_20.data_register, sprt_20.coord_x, sprt_20.coord_y, sprt_20.offset, sprt_20.ativo); break; } }
	while(1){ if(isFull() == 0) { set_sprite(sprt_21.data_register, sprt_21.coord_x, sprt_21.coord_y, sprt_21.offset, sprt_21.ativo); break; } }
	while(1){ if(isFull() == 0) { set_sprite(sprt_22.data_register, sprt_22.coord_x, sprt_22.coord_y, sprt_22.offset, sprt_22.ativo); break; } }
	while(1){ if(isFull() == 0) { set_sprite(sprt_23.data_register, sprt_23.coord_x, sprt_23.coord_y, sprt_23.offset, sprt_23.ativo); break; } }
	while(1){ if(isFull() == 0) { set_sprite(sprt_24.data_register, sprt_24.coord_x, sprt_24.coord_y, sprt_24.offset, sprt_24.ativo); break; } }
	while(1){ if(isFull() == 0) { set_sprite(sprt_25.data_register, sprt_25.coord_x, sprt_25.coord_y, sprt_25.offset, sprt_25.ativo); break; } }

	closeMappingMemory();
}
