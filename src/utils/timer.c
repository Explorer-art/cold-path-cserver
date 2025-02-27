#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <utils/timer.h>

void timer_handler(union sigval sv) {
	Timer *timer = (Timer*) sv.sival_ptr;

	if (timer->callback) {
		timer->callback();
	}

    struct itimerspec ts;

    ts.it_value.tv_sec = timer->delay;
    ts.it_value.tv_nsec = 0;
    ts.it_interval.tv_sec = timer->delay;
    ts.it_interval.tv_nsec = 0;

    timer_settime(timer->timer_id, 0, &ts, NULL);
}

Timer* after(int delay, callback_t callback) {
    struct sigevent sev;
    struct itimerspec ts;
    Timer *timer = (Timer*) malloc(sizeof(Timer));

    if (timer == NULL) {
        perror("[ERROR] Malloc failed\n");
        return NULL;
    }

    timer->delay = delay;
    timer->callback = callback;

    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = timer_handler;
    sev.sigev_notify_attributes = NULL;
    sev.sigev_value.sival_ptr = timer;

    if (timer_create(CLOCK_REALTIME, &sev, &(timer->timer_id)) == -1) {
        perror("[ERROR] Create timer failed\n");
        free(timer);
        return NULL;
    }

    ts.it_value.tv_sec = delay;
    ts.it_value.tv_nsec = 0;
    ts.it_interval.tv_sec = delay;
    ts.it_interval.tv_nsec = 0;

    if (timer_settime(timer->timer_id, 0, &ts, NULL) == -1) {
        perror("[ERROR] Timer set time failed\n");
        free(timer);
        return NULL;
    }

    return timer;
}

void delete_timer(Timer *timer) {
    if (timer_delete(timer->timer_id) == -1) {
        perror("[ERROR] Failed delete timer\n");
    }

    free(timer);
}