#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "./address_map_arm.h"

/* Prototypes for functions used to access physical memory addresses */
int open_physical (int);
void * map_physical (int, unsigned int, unsigned int);
void close_physical (int);
int unmap_physical (void *, unsigned int);

void delay(volatile unsigned int count) {
	while (count-- > 0) {
	}
}

/*This program increments the contents of the red LED parallel port */
int main(void)
{
   volatile int *LEDR_ptr, *LEDR_ptr1;   // virtual address pointer to red LEDs
   int fd = -1, i, j;               // used to open /dev/mem for access to physical addresses
   void *LW_virtual;          // used to map physical addresses for the light-weight bridge

   // Create virtual memory access to the FPGA light-weight bridge
   if ((fd = open_physical (fd)) == -1)
	   return (-1);
   if ((LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
	   return (-1);
   
   LEDR_ptr = (int *) (LW_virtual + HEX3_HEX0_BASE);
   LEDR_ptr1 = (int *)  (LW_virtual + HEX5_HEX4_BASE);
   
   int mensagem[]={115,124,56,64,91,94,0,120,115,63,91,0,0,0,0,0,0};
   while(1){
	   *LEDR_ptr=0;
	   *LEDR_ptr1=0;
	   for(i=0; i<17;i++){
		   if(i<4){
			   *LEDR_ptr+=mensagem[i];
			   delay(40000000);
			   if(i!=3)
				   *LEDR_ptr <<= 8;
		   }else{
			   *LEDR_ptr1 += mensagem[i-4];
			   *LEDR_ptr <<= 8;
			   *LEDR_ptr+=mensagem[i];
			   delay(40000000);
			   *LEDR_ptr1 <<= 8;
		   }
	   }
   }
   unmap_physical (LW_virtual, LW_BRIDGE_SPAN);   // release the physical-memory mapping
   close_physical (fd);   // close /dev/mem
   return 0;
}

// Open /dev/mem, if not already done, to give access to physical addresses
int open_physical (int fd)
{
   if (fd == -1)
      if ((fd = open( "/dev/mem", (O_RDWR | O_SYNC))) == -1)
      {
         printf ("ERROR: could not open \"/dev/mem\"...\n");
         return (-1);
      }
   return fd;
}

// Close /dev/mem to give access to physical addresses
void close_physical (int fd)
{
   close (fd);
}

/*
 * Establish a virtual address mapping for the physical addresses starting at base, and
 * extending by span bytes.
 */
void* map_physical(int fd, unsigned int base, unsigned int span)
{
   void *virtual_base;

   // Get a mapping from physical addresses to virtual addresses
   virtual_base = mmap (NULL, span, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, base);
   if (virtual_base == MAP_FAILED)
   {
      printf ("ERROR: mmap() failed...\n");
      close (fd);
      return (NULL);
   }
   return virtual_base;
}

/*
 * Close the previously-opened virtual address mapping
 */
int unmap_physical(void * virtual_base, unsigned int span)
{
   if (munmap (virtual_base, span) != 0)
   {
      printf ("ERROR: munmap() failed...\n");
      return (-1);
   }
   return 0;
}

