#include <mbed.h>
#include <math.h>
#include "LIS3DSH.h"
#include "Exercise.h"

/* situps state enum */
typedef enum state_situps{
  lying,
  sitting
}state_situps;

/* pushup state enum */
// when doing a pushup, we can make 2 states by juding whether our arms are bent
typedef enum state_Pushup{
  stright,
  bent
}state_Pushup;

/* squates state enum */
typedef enum state_squats{
  standing,
  squating
}state_squats;

/* jumping jacks state enum */
typedef enum state_jumpingjacks{
  standing1,
  standing2
}state_jumpingjacks;


/* defination */
InterruptIn userButton(BUTTON1);
DigitalOut ledOut1(LED3);
DigitalOut ledOut2(LED4);
DigitalOut ledOut3(LED5);
DigitalOut ledOut4(LED6);
LIS3DSH acc(SPI_MOSI, SPI_MISO, SPI_SCK, PE_3);

/* config */
#define ACTION_NUMBER 5
static int counter;
static int flag = 0; //we use 4 value to indicate 4 exercises.
static int state = 0;


/* definition of exercise */
static Exercise situps(45, 30, lying, sitting);
static Exercise pushup(170, 145, stright, bent);
static Exercise squat(60, 30, standing, squating);
static Exercise jumpingjacks(85, 75, standing1, standing2);


/**************** universal values ************/
int16_t x, y, zAccel = 0; //acceleration in z (raw)
float g_z = 0;  //acceleration in z(g force)
double angle = 0; //angle reletive to z axis
const float PI = 3.1415926;


/************** buffer array *************/
const uint8_t N = 5; // filter length
float ringbuf[N]; // sample buffer
uint8_t ringbuf_index = 0; // index to insert sample


/************ finish execise indication   ****/
void ledAllLighting() {
  ledOut1 = 1;
  ledOut2 = 1;
  ledOut3 = 1;
  ledOut4 = 1;
}
void ledInit() {
  ledOut1 = 0;
  ledOut2 = 0;
  ledOut3 = 0;
  ledOut4 = 0;
}


/********** data filting  ************/
float getG_ZVal(){ 
  acc.ReadData(&x, &y, &zAccel);
  // normalise to 1g
  g_z = (float)zAccel / 17694.0;
  ringbuf[ringbuf_index++] = g_z;
  // at the end of the buffer, wrap around to the beginning
  if (ringbuf_index >= N) {
    ringbuf_index = 0;
  }
  // START OF FILTERING
  float g_z_filt = 0;
  // add all samples
  for (uint8_t i = 0; i < N; i++) {
    g_z_filt += ringbuf[i];
  }
  // divide by number of samples
  g_z_filt /= (float)N;
  // restrict to 1g (acceleration not decoupled from orientation)
  if (g_z_filt > 1) {
    g_z_filt = 1;
  }
  return 180 * acos(g_z_filt) / PI;
}

// indicator of led lighting
int sign = 0;

void detectExercise(Exercise exercise, DigitalOut& led, int waitingTime) {  
  while (acc.Detect() != 1) {
    wait_ms(200);
  }
  while (counter < ACTION_NUMBER * 2 + 2) {
      // compute angle in degrees
      angle = getG_ZVal();
      /* state mechine logic */
      switch (state) {
        case 0 : // 0 represents state1
          if (angle > exercise.getTransitionAngle1()) {
            state = exercise.getState2();
            counter++;
          }
          led = 0;
          break;
        case 1 : // 1 represents state2
          if (angle < exercise.getTransitionAngle2()) {
            state = exercise.getState1();
            counter++;
          }
          led = 1;
          break;      
        default:
          printf("do it again!");
      }
    sign = counter;
    wait_ms(waitingTime);
  }  
}

/* changing flag means that chaning exercise */
void initAndFlagChange() {
  counter = 0;
  sign = counter;
  state = 0;
  flag++;
}

int main() {
  while(1) {
    userButton.rise(&initAndFlagChange);
    switch (flag) {
      case 1:
        if (sign >= ACTION_NUMBER * 2 + 2) {
          ledAllLighting();
        } else {
          ledInit();
          detectExercise(situps, ledOut1, 100);            
        }  
        break;
      case 2:
        if (sign >= ACTION_NUMBER * 2 + 2) {
          ledAllLighting();
        } else {
          ledInit();
          detectExercise(pushup, ledOut2, 30);
        }
        break;
      case 3:
        if (sign >= ACTION_NUMBER * 2 + 2) {
          ledAllLighting();
        } else {
          ledInit();
          detectExercise(squat, ledOut4, 10);
        }
        break;
      case 4:
        if (sign >= ACTION_NUMBER * 2 + 2) {
          ledAllLighting();
        } else {
          ledInit();
          detectExercise(jumpingjacks, ledOut3, 100);
        }     
        break;
      default:
        flag = 0;
        break;
    }
  }
}