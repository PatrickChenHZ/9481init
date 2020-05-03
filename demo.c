#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <wiringPi.h>

#include "ili93xx.h"


#define _DEBUG_ 0

//When you'd like to wait by a keyboard entries, enable this line.
//#define WAIT inputKey()

//When you'd like to wait in the waiting time, enable this line.
#define WAIT sleep(1)

void inputKey() {
  char ch;
  printf("Hit any key");
  scanf("%c",&ch);
}

time_t elapsedTime(struct timeval startTime, struct timeval endTime) {
    time_t diffsec = difftime(endTime.tv_sec, startTime.tv_sec);
    suseconds_t diffsub = endTime.tv_usec - startTime.tv_usec;
//printf("diffsec=%ld diffsub=%ld\n",diffsec, diffsub);
    if(diffsub < 0) {
        diffsec--;
        diffsub = (endTime.tv_usec+1000000) - startTime.tv_usec;
    }
    uint16_t diffmsec = diffsub / 1000;
    time_t diff = (diffsec * 1000) + diffmsec;
    return diff;
}

time_t ColorBarTest(int width, int height) {
    struct timeval startTime, endTime;
    gettimeofday(&startTime, NULL);

    uint16_t y1 = height/3;
    uint16_t y2 = (height/3)*2;
    lcdDrawFillRect(0, 0, width-1, y1-1, RED);
    lcdDrawFillRect(0, y1-1, width-1, y2-1, GREEN);
    lcdDrawFillRect(0, y2-1, width-1, height-1, BLUE);

    gettimeofday(&endTime, NULL);
    time_t diff = elapsedTime(startTime, endTime);
    printf("%s elapsed time[ms]=%ld\n",__func__, diff);
    return diff;
} 

int main(int argc, char **argv){

  if(wiringPiSetup() == -1) {
    printf("wiringPiSetup Fail\n");
    return 1;
  }

  int i;
  char base[128];
  strcpy(base, argv[0]);
  for(i=strlen(base);i>0;i--) {
    if (base[i-1] == '/') {
      base[i] = 0;
      break;
    }
  }
if(_DEBUG_)printf("base=%s\n",base);

  char ppath[128];
  strcpy(ppath,base);
  strcat(ppath,"pin.conf");
if(_DEBUG_)printf("ppath=%s\n",ppath);

  int screenWidth,screenHeight;
  char model[20];

#ifdef ILI9481
  screenWidth = 320;
  screenHeight = 480;
  strcpy(model,"ILI9481");
  lcdInit(0x9481,screenWidth,screenHeight,ppath);
#endif


  printf("Your TFT controller is %s.\n",model); 
  printf("TFT resolution is %d x %d.\n",screenWidth, screenHeight); 
  lcdReset();
  lcdSetup();

  ColorBarTest(screenWidth, screenHeight);
  WAIT;

  for(i=0;i<2;i++) {
    lcdDisplayOff();
    sleep(1);
    lcdDisplayOn();
    sleep(1);
  }
  WAIT;


  return 0;
}
