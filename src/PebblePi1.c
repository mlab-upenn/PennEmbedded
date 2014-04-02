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
#define MOTORA RPI_GPIO_P1_10
#define MOTORB RPI_GPIO_P1_11
#define SIGNAL RPI_GPIO_P1_12

struct motor_params{
  int pin;
  float duty;
  };

void *readsensor(void *arg){
   int time,listenfd,connfd;
   struct sockaddr_in serv_addr;
   char sendBuff[1025];
   listenfd = socket(AF_INET, SOCK_STREAM, 0);
   memset(&serv_addr, '0', sizeof(serv_addr)); 
   memset(sendBuff, '0', sizeof(sendBuff));
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   serv_addr.sin_port = htons(4444);
   bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
   listen(listenfd, 10);
  while (1)
  { 
    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
    time = 0;
    //Output Pin
    bcm2835_gpio_fsel(SIGNAL, BCM2835_GPIO_FSEL_OUTP);
    //Trigger Pulse
    bcm2835_gpio_write(SIGNAL,HIGH);
    bcm2835_delayMicroseconds(5);
    bcm2835_gpio_write(SIGNAL,LOW);
    printf("PULSE SENT\r\n");
    //Input Pin
    bcm2835_gpio_fsel(SIGNAL, BCM2835_GPIO_FSEL_INPT);
    
    //Waiting for echo to begin
    while (!bcm2835_gpio_lev(SIGNAL));
    time = bcm2835_st_read(); //timestamp when echo begins
   
    //waiting for echo to end
    while (bcm2835_gpio_lev(SIGNAL));
    time = bcm2835_st_read() - time;//timestamp when echo end
    printf("FINAL: %d\r\n", time); //displaying timestamp difference - proportional to distance
    snprintf(sendBuff,sizeof(sendBuff),"%i",time);
    printf("%s\n",sendBuff);
    write(connfd,sendBuff,strlen(sendBuff));
    //sendDatatoPhone(time); 
    close(connfd);
    sleep(3); //wait before next cycle
  }
}

void *drivemotors(void *arg){

 struct  motor_params* motor = (struct motor_params*)arg;
 
 //printf("VALUE: %d\n",value);
 
 while(1){
   bcm2835_gpio_write(motor->pin,HIGH);
   bcm2835_delayMicroseconds(motor->duty*200);
   bcm2835_gpio_write(motor->pin,LOW);
   bcm2835_delayMicroseconds((100-motor->duty)*200);
 
  } 
}


int main(int argc,char **argv){ 
  struct motor_params motorleft,motorright;
  pthread_t sth ,lth ,rth;
  
  motorleft.pin = MOTORA;
  motorright.pin = MOTORB;
  int count =0;
  int direction = 3;
  int listenfd = 0, connfd = 0, i = 0;
  struct sockaddr_in serv_addr;
  char sendBuff[1025];
  char recvBuff[1024];
  
  // read ping sensor thread
  //pthread_create(&sth,NULL,readsensor,(void*)direction);

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(recvBuff, '0', sizeof(sendBuff));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(5000);

  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  listen(listenfd, 10);
   
  if(!bcm2835_init())
    return 1;

  bcm2835_gpio_fsel(MOTORA, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(MOTORB, BCM2835_GPIO_FSEL_OUTP);
  
  

  while(1){
    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
    if(recv(connfd, recvBuff, sizeof(recvBuff),0)< 0){
      printf("No data received from the client\n");
    }
    
    else{ 

      printf("%c\n\r", recvBuff[0]);
      //printf("Direction: %d",direction);
      direction = recvBuff[0] - '0';
      
      switch(direction){
      	case FORWARD:
      	  motorright.duty = 3.75;
      	  motorleft.duty = 11.25;
      	  break;
      	case RIGHT:
      	  motorright.duty = 7.25;
      	  motorleft.duty = 11.25;
      	  break;
      	case LEFT:
      	  motorright.duty = 3.75;
      	  motorleft.duty = 7.25;
      	  break;
      	default:
      	  motorright.duty = 7.25;
      	  motorleft.duty = 7.25;
      	  break; 
      }
      
      //starting motor thread   
      pthread_create(&rth,NULL,drivemotors,(void*)&motorright);
      pthread_create(&lth,NULL,drivemotors,(void*)&motorleft);
      //sleep(3);
      if(direction == FORWARD)
    	sleep(3);
      else 
	sleep(1);	 
      pthread_cancel(rth);
      pthread_cancel(lth);
      printf("Ending\n");
      pthread_join(rth,NULL);
      pthread_join(lth,NULL);
    } 
    
    close(connfd);
    
  }
  
  bcm2835_close();

  return 0;
}
