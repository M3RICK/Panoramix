/*
** EPITECH PROJECT, 2025
** PANORAMIX
** File description:
** Pas nono
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

typedef struct panoramix_s {
    int nb_gitouze;
    int huit_six;
    int nb_fights;
    int nb_refills;
    int bouillon_left;
    int ingredients_left;
    int all_zonzons_done;
    pthread_mutex_t marmite_mutex;
    pthread_mutex_t print_mutex;
    pthread_barrier_t barrier;
    sem_t snore_sem;
    sem_t pot_refilled_sem;
    int druid_is_awake;
    int *zonzon_fights_left;
} panoramix_t;

typedef struct thread_arg_s {
    int id;
    panoramix_t *data;
} thread_arg_t;

void *zonzon_thread(void *arg);
void *pano_thread(void *arg);

void msg(panoramix_t *data, const char *format, ...);
int parse_arguments(int argc, char **argv, panoramix_t *data);
int initialize_data(panoramix_t *data);
void cleanup_resources(panoramix_t *data);

int get_serving(panoramix_t *data, int zonzon_id);
void wake_druid(panoramix_t *data, int zonzon_id);
void fight_romans(panoramix_t *data, int zonzon_id, int *fights_left);
void handle_zonzon_exit(panoramix_t *data, int zonzon_id, thread_arg_t *arg);
int check_all_zonzons_done(panoramix_t *data);

void refill_potion(panoramix_t *data);
int should_druid_exit(panoramix_t *data);

int allocate_thread_memory(panoramix_t *data, pthread_t **zonzons,
    thread_arg_t ***args);
int create_druid_thread(panoramix_t *data, pthread_t *druid);
int create_zonzon_threads(panoramix_t *data, pthread_t *zonzons,
    thread_arg_t **args);
void join_all_threads(panoramix_t *data, pthread_t *zonzons, pthread_t druid);

#endif
