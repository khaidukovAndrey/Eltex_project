#ifndef pthread_init_h
#define pthread_init_h

#include "vlan_tagger.h"
#include <signal.h>

void pthread_init(const char *);

void logging_programm_completion(struct thread_data*);

#endif //pthread_init_h
