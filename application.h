#ifndef APPLICATION_H
#define APPLICATION_H

#define LED0 0
#define LED1 1
#define LED2 2
#define LED3 3
#define MAX_DELAY_FACTOR 20

sint8 detect_object(void);
uint32 count_object(sint8);
void led_on(uint8);
void led_off(uint8);
void distance_alarm(f32);


#endif