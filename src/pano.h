/*
** EPITECH PROJECT, 2025
** PANORAMIX
** File description:
** Header file
*/

#ifndef PANONO_H_
    #define PANONO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdarg.h>

typedef struct {
    int nb_gitouze;
    int huit_six;
    int nb_fights;
    int nb_refills;
    int current_pot_servings;
    int remaining_refills;
    int all_gitouze_done;
    pthread_mutex_t pot_mutex;
    pthread_mutex_t print_mutex;
    sem_t druid_sem;
    sem_t pot_refilled_sem;
    int *gitouze_fights_left;
} panoramix_t;


typedef struct thread_arg_s {
    int id;
    panoramix_t *data;
} thread_arg_t;


void *gitouze_thread(void *arg);
void *druid_thread(void *arg);

void print_message(panoramix_t *data, const char *format, ...);
int parse_arguments(int argc, char **argv, panoramix_t *data);
int initialize_data(panoramix_t *data);
void cleanup_resources(panoramix_t *data);

#endif
