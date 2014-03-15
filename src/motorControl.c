#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <bcm2835.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>

#define FORWARD 1
#define RIGHT 2
#define LEFT 3
#define MOTORA RPI_GPIO_P1_11
#define MOTORB RPI_GPIO_P1_12

void *generatePWM(void *arg){

 int value = (int)arg;
 printf("VALUE: %d\n",value);
 
 while(1){
    switch(value){
       case FORWARD:
	  printf("Moving forward\n");
	  bcm2835_gpio_write(MOTORA,HIGH);
          bcm2835_gpio_write(MOTORB,LOW);
	  bcm2835_delayMicroseconds(2250);
          bcm2835_gpio_write(MOTORA,LOW);
	  bcm2835_gpio_write(MOTORB,HIGH);
	  bcm2835_delayMicroseconds(17750);
	  break;
       case RIGHT:
	  printf("Turning right\n");
          bcm2835_gpio_write(MOTORA,HIGH); 
	  bcm2835_gpio_write(MOTORB,LOW);
	  bcm2835_delayMicroseconds(2250);
          bcm2835_gpio_write(MOTORA,LOW);
	  bcm2835_gpio_write(MOTORB,LOW);
	  bcm2835_delayMicroseconds(17750);
	  break;
       case LEFT:
	  printf("Turning left\n");
          bcm2835_gpio_write(MOTORA,LOW);
          bcm2835_gpio_write(MOTORB,LOW);
	  bcm2835_delayMicroseconds(2250);
          bcm2835_gpio_write(MOTORA,LOW);
	  bcm2835_gpio_write(MOTORB,HIGH);
	  bcm2835_delayMicroseconds(17750);
          break;
       default:
          bcm2835_gpio_write(MOTORA,LOW);
          bcm2835_gpio_write(MOTORB,LOW);
	  bcm2835_delayMicroseconds(2250);
          bcm2835_gpio_write(MOTORA,LOW);
	  bcm2835_gpio_write(MOTORB,LOW);
	  bcm2835_delayMicroseconds(17750);
          break;
     }
 }
}

int main(int argc,char **argv){
  pthread_t pth;
  int count =0;
  int direction = 1;
  int listenfd = 0, connfd = 0, i = 0;
  struct sockaddr_in serv_addr;
  char sendBuff[1025];
  char recvBuff[1024];
  char* message= "I am the server";

  /*listenfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(recvBuff, '0', sizeof(sendBuff));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(5000);

  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  listen(listenfd, 10);
   */
  if(!bcm2835_init())
    return 1;

  bcm2835_gpio_fsel(MOTORA, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(MOTORB, BCM2835_GPIO_FSEL_OUTP);
  
  while(1){
    /*connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
    if(recv(connfd, recvBuff, sizeof(recvBuff),0)< 0){
      printf("No data received from the client\n");
    }
    
    else{ 

      printf("%c\n\r", recvBuff[0]);
      //printf("Direction: %d",direction);
      direction = recvBuff[0] - '0';
      */   
      pthread_create(&pth,NULL,generatePWM,(void*)direction);
      sleep(3);	 
      pthread_cancel(pth);
      printf("Ending\n");
      pthread_join(pth,NULL);
    //} 
    
    close(connfd);
    sleep(1);
    direction++;
    if(direction == 4)
      break;
  }

  bcm2835_close();

  return 0;
}
