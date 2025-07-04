/*
** EPITECH PROJECT, 2025
** PANORAMIX
** File description:
** tout est dans le nom ma biche
*/

#include "pano.h"

void msg(panoramix_t *data, const char *format, ...)
{
    va_list args;

    pthread_mutex_lock(&data->print_mutex);
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    fflush(stdout);
    pthread_mutex_unlock(&data->print_mutex);
}

static void display_usage(char *program_name)
{
    printf("USAGE: %s <nb_villagers> <pot_size> <nb_fights> <nb_refills>\n",
        program_name);
    printf("Values must be >0.\n");
}

static int check_arguments(panoramix_t *data)
{
    if (data->nb_gitouze <= 0 || data->huit_six <= 0 ||
        data->nb_fights <= 0 || data->nb_refills <= 0)
        return 0;
    return 1;
}

int parse_arguments(int argc, char **argv, panoramix_t *data)
{
    if (argc != 5) {
        display_usage(argv[0]);
        return 0;
    }
    data->nb_gitouze = atoi(argv[1]);
    data->huit_six = atoi(argv[2]);
    data->nb_fights = atoi(argv[3]);
    data->nb_refills = atoi(argv[4]);
    if (!check_arguments(data)) {
        display_usage(argv[0]);
        return 0;
    }
    return 1;
}

static int init_mutexes(panoramix_t *data)
{
    if (pthread_mutex_init(&data->marmite_mutex, NULL) != 0)
        return 0;
    if (pthread_mutex_init(&data->print_mutex, NULL) != 0) {
        pthread_mutex_destroy(&data->marmite_mutex);
        return 0;
    }
    return 1;
}

static int init_semaphores(panoramix_t *data)
{
    if (sem_init(&data->snore_sem, 0, 0) != 0) {
        pthread_mutex_destroy(&data->marmite_mutex);
        pthread_mutex_destroy(&data->print_mutex);
        return 0;
    }
    if (sem_init(&data->pot_refilled_sem, 0, 0) != 0) {
        pthread_mutex_destroy(&data->marmite_mutex);
        pthread_mutex_destroy(&data->print_mutex);
        sem_destroy(&data->snore_sem);
        return 0;
    }
    return 1;
}

static int init_barrier(panoramix_t *data)
{
    if (pthread_barrier_init(&data->barrier, NULL,
        data->nb_gitouze + 1) != 0) {
        pthread_mutex_destroy(&data->marmite_mutex);
        pthread_mutex_destroy(&data->print_mutex);
        sem_destroy(&data->snore_sem);
        sem_destroy(&data->pot_refilled_sem);
        return 0;
    }
    return 1;
}

static int init_zonzon_fights(panoramix_t *data)
{
    data->zonzon_fights_left = malloc(sizeof(int) * data->nb_gitouze);
    if (data->zonzon_fights_left == NULL) {
        pthread_mutex_destroy(&data->marmite_mutex);
        pthread_mutex_destroy(&data->print_mutex);
        sem_destroy(&data->snore_sem);
        sem_destroy(&data->pot_refilled_sem);
        pthread_barrier_destroy(&data->barrier);
        return 0;
    }
    for (int i = 0; i < data->nb_gitouze; i++)
        data->zonzon_fights_left[i] = data->nb_fights;
    return 1;
}

int initialize_data(panoramix_t *data)
{
    data->bouillon_left = data->huit_six;
    data->ingredients_left = data->nb_refills;
    data->all_zonzons_done = 0;
    data->druid_is_awake = 0;
    if (!init_mutexes(data))
        return 0;
    if (!init_semaphores(data))
        return 0;
    if (!init_barrier(data))
        return 0;
    if (!init_zonzon_fights(data))
        return 0;
    return 1;
}

void cleanup_resources(panoramix_t *data)
{
    pthread_mutex_destroy(&data->marmite_mutex);
    pthread_mutex_destroy(&data->print_mutex);
    sem_destroy(&data->snore_sem);
    sem_destroy(&data->pot_refilled_sem);
    pthread_barrier_destroy(&data->barrier);
    free(data->zonzon_fights_left);
}
