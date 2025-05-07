/*
** EPITECH PROJECT, 2025
** PANORAMIX
** File description:
** Ficelles
*/

#include "pano.h"

int should_druid_exit(panoramix_t *data)
{
    int exit_flag;

    pthread_mutex_lock(&data->marmite_mutex);
    exit_flag = data->all_zonzons_done || data->ingredients_left <= 0;
    pthread_mutex_unlock(&data->marmite_mutex);
    return exit_flag;
}

void refill_potion(panoramix_t *data)
{
    print_message(data,
        "Druid: Ah! Yes, yes, I'm awake! Working on it! "
        "Beware I can only make %d more refills after this one.\n",
        data->ingredients_left - 1);

    pthread_mutex_lock(&data->marmite_mutex);
    data->bouillon_left = data->huit_six;
    data->ingredients_left--;
    if (data->ingredients_left == 0)
        print_message(data, "Druid: I'm out of viscum. I'm going back to... zZz\n");
    data->druid_is_awake = 0;
    pthread_mutex_unlock(&data->marmite_mutex);
    for (int i = 0; i < data->nb_gitouze; i++)
        sem_post(&data->pot_refilled_sem);
}

void *pano_thread(void *arg)
{
    panoramix_t *data = (panoramix_t *)arg;

    print_message(data, "Druid: I'm ready... but sleepy...\n");
    pthread_barrier_wait(&data->barrier);
    while (1) {
        sem_wait(&data->snore_sem);
        if (should_druid_exit(data))
            break;
        refill_potion(data);
    }
    return NULL;
}

void *zonzon_thread(void *arg)
{
    thread_arg_t *thread_arg = (thread_arg_t *)arg;
    int zonzon_id = thread_arg->id;
    panoramix_t *data = thread_arg->data;
    int fights_left = data->nb_fights;

    pthread_barrier_wait(&data->barrier);
    print_message(data, "Villager %d: Going into battle!\n", zonzon_id);
    usleep(5);
    while (fights_left > 0) {
        if (get_serving(data, zonzon_id)) {
            usleep(5);
            fight_romans(data, zonzon_id, &fights_left);
            sched_yield();
        } else {
            wake_druid(data, zonzon_id);
        }
    }
    handle_zonzon_exit(data, zonzon_id, thread_arg);
    return NULL;
}
