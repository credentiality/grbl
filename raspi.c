// Very ugly mashup of raspi and RT_PREEMPT example code

// Writes to /dev/mem, so don't enable this on something other than a raspi

// Compile with gcc -lrt 

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sched.h>
#include <sys/mman.h>

#include <string.h>

 /************************************************************************\
  * You are about to change the GPIO settings of your raspberry pi.      *
  * Mess this up and it will stop working!                               *
  * It might be a good idea to 'sync' before running this program        *
  * so at least you still have your code changes written to the SD-card! *
 \************************************************************************/

// defines for using the realtime kernel
#define MY_PRIORITY (49) /* we use 49 as the PRREMPT_RT use 50
                            as the priority of kernel tasklets
                            and interrupt handler by default */

#define MAX_SAFE_STACK (8*1024) /* The maximum stack size which is
                                   guaranteed safe to access without
                                   faulting */

#define NSEC_PER_SEC    (1000000000) /* The number of nsecs per sec. */


// defines for talking to the gpio pins

// Access from ARM Running Linux

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

int  mem_fd;
void *gpio_map;

// I/O access
volatile unsigned *gpio;

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))

#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

static void setup_io();

// 0 means GPIO-0 on this diagram: http://elinux.org/File:GPIOs.png
const int raspi_direction_pins[] = { 0, 14, 4, 0, 0, 0 };

// 0 means GPIO-0 on this diagram: http://elinux.org/File:GPIOs.png
const int raspi_step_pins[] = { 1, 15, 17, 1, 1, 1 };

inline void raspi_direction(int axis, int value) {

  //printf("set direction[%d]:%d\n", axis, value);
  if (value) {
    GPIO_SET = 1 << raspi_direction_pins[axis];
  } else {
    GPIO_CLR = 1 << raspi_direction_pins[axis];
  }
}

inline void raspi_step(int axis, int value) {
  //printf("set step[%d]:%d\n", axis, value);
  if (value) {
    GPIO_SET = 1 << raspi_step_pins[axis];
  } else {
    GPIO_CLR = 1 << raspi_step_pins[axis];
  }
}

static void stack_prefault(void);

int raspi_init()
{
        struct sched_param param;

        /* Declare ourself as a real time task */
        param.sched_priority = MY_PRIORITY;
        if(sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
                perror("sched_setscheduler failed");
                return 0;
        }

        /* Lock memory */
        if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1) {
                perror("mlockall failed");
                return 0;
        }

        /* Pre-fault our stack */
        stack_prefault();

  // Set up gpi pointer for direct register access
  setup_io();

  int axis;
  for (axis = 0; axis < 3; axis++) {
    INP_GPIO(raspi_step_pins[axis]); // must use INP_GPIO before we can use OUT_GPIO
    OUT_GPIO(raspi_step_pins[axis]);
  }

  return 1;
}

static void stack_prefault(void) {

        unsigned char dummy[MAX_SAFE_STACK];

        memset(dummy, 0, MAX_SAFE_STACK);
        return;
}


//
// Set up a memory regions to access GPIO
//
static void setup_io()
{
   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("can't open /dev/mem \n");
      exit(-1);
   }

   /* mmap GPIO */
   gpio_map = mmap(
      NULL,             //Any address in our space will do
      BLOCK_SIZE,       //Map length
      PROT_READ|PROT_WRITE,// Enable reading & writing to mapped memory
      MAP_SHARED,       //Shared with other processes
      mem_fd,           //File to map
      GPIO_BASE         //Offset to GPIO peripheral
   );

   close(mem_fd); //No need to keep mem_fd open after mmap

   if (gpio_map == MAP_FAILED) {
      perror("mmap failed");
      exit(-1);
   }

   // Always use volatile pointer!
   gpio = (volatile unsigned *)gpio_map;


} // setup_io
