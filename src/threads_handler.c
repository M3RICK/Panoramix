/*
** EPITECH PROJECT, 2025
** PANORAMIX
** File description:
** Ficelles
*/

#include "pano.h"

int check_all_zonzons_done(panoramix_t *data)
{
    for (int i = 0; i < data->nb_gitouze; i++) {
        if (data->zonzon_fights_left[i] > 0)
            return 0;
    }
    return 1;
}

int get_serving(panoramix_t *data, int zonzon_id)
{
    int success = 0;

    pthread_mutex_lock(&data->marmite_mutex);
    print_message(data, "Villager %d: I need a drink... I see %d servings left.\n",
        zonzon_id, data->bouillon_left);
    if (data->bouillon_left > 0) {
        data->bouillon_left--;
        success = 1;
    }
    pthread_mutex_unlock(&data->marmite_mutex);
    return success;
}

void wake_druid(panoramix_t *data, int zonzon_id)
{
    int should_wake = 0;

    pthread_mutex_lock(&data->marmite_mutex);
    if (!data->druid_is_awake && data->ingredients_left > 0) {
        should_wake = 1;
        data->druid_is_awake = 1;
        print_message(data, "Villager %d: Hey Pano wake up! We need more potion.\n",
            zonzon_id);
    }
    pthread_mutex_unlock(&data->marmite_mutex);
    if (should_wake)
        sem_post(&data->snore_sem);
    sem_wait(&data->pot_refilled_sem);
}

void fight_romans(panoramix_t *data, int zonzon_id, int *fights_left)
{
    print_message(data, "Villager %d: Take that roman scum! Only %d left.\n",
                zonzon_id, *fights_left - 1);
    (*fights_left)--;
}

void handle_zonzon_exit(panoramix_t *data, int zonzon_id, thread_arg_t *arg)
{
    print_message(data, "Villager %d: I'm going to sleep now.\n", zonzon_id);
    pthread_mutex_lock(&data->marmite_mutex);
    data->zonzon_fights_left[zonzon_id] = 0;
    if (check_all_zonzons_done(data)) {
        data->all_zonzons_done = 1;
        sem_post(&data->snore_sem);
    }
    pthread_mutex_unlock(&data->marmite_mutex);
    free(arg);
}
