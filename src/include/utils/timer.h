#ifndef TIMER_H
#define TIMER_H

typedef void (*callback_t)(void);

typedef struct {
	timer_t timer_id;
	int delay;
	callback_t callback;
} Timer;

Timer* after(int delay, callback_t callback);
void delete_timer(Timer* timer);

#endif