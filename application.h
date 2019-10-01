#ifndef APPLICATION_H
#define APPLICATION_H

#define LED0 0
#define LED1 1
#define LED2 2
#define LED3 3
#define MAX_DELAY_FACTOR 30
#define MIN_DELAY_FACTOR 10
#define AVG_DELAY_FACTOR 20
#define MAX_DISTANCE 40
#define MIN_DISTANCE 10

sint8 detect_object(void);
uint32 count_object(sint8);
void led_on(uint8);
void led_off(uint8);
void distance_alarm(f32);


#endif