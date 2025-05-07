/*
** EPITECH PROJECT, 2025
** PANORAMIX
** File description:
** Thread
*/

#include "pano.h"

static int check_all_gitouze_done(panoramix_t *data)
{
    for (int i = 0; i < data->nb_gitouze; i++) {
        if (data->gitouze_fights_left[i] > 0)
            return 0;
    }
    return 1;
}

static void mark_gitouze_done(panoramix_t *data, int id)
{
    pthread_mutex_lock(&data->pot_mutex);
    data->gitouze_fights_left[id] = 0;

    if (check_all_gitouze_done(data)) {
        data->all_gitouze_done = 1;
        sem_post(&data->druid_sem);
    }
    pthread_mutex_unlock(&data->pot_mutex);
}

static int get_serving(panoramix_t *data, int gitouze_id)
{
    int success = 0;

    pthread_mutex_lock(&data->pot_mutex);
    print_message(data, "Villager %d: I need a drink... I see %d servings left.\n",
                 gitouze_id, data->current_pot_servings);

    if (data->current_pot_servings > 0) {
        data->current_pot_servings--;
        success = 1;
    }

    pthread_mutex_unlock(&data->pot_mutex);
    return success;
}

static void wake_druid(panoramix_t *data, int gitouze_id)
{
    print_message(data, "Villager %d: Hey Pano wake up! We need more potion.\n",
                gitouze_id);
    sem_post(&data->druid_sem);

    sem_wait(&data->pot_refilled_sem);
}

static void fight_romans(panoramix_t *data, int gitouze_id, int *fights_left)
{
    print_message(data, "Villager %d: Take that roman scum! Only %d left.\n",
                gitouze_id, *fights_left - 1);
    (*fights_left)--;
}

void *gitouze_thread(void *arg)
{
    thread_arg_t *thread_arg = (thread_arg_t *)arg;
    int gitouze_id = thread_arg->id;
    panoramix_t *data = thread_arg->data;
    int fights_left = data->nb_fights;

    print_message(data, "Villager %d: Going into battle!\n", gitouze_id);

    while (fights_left > 0) {
        if (get_serving(data, gitouze_id))
            fight_romans(data, gitouze_id, &fights_left);
        else
            wake_druid(data, gitouze_id);
    }

    print_message(data, "Villager %d: I'm going to sleep now.\n", gitouze_id);
    mark_gitouze_done(data, gitouze_id);
    free(arg);
    return NULL;
}

static int should_druid_exit(panoramix_t *data)
{
    int should_exit = 0;

    pthread_mutex_lock(&data->pot_mutex);
    if (data->all_gitouze_done)
        should_exit = 1;
    pthread_mutex_unlock(&data->pot_mutex);
    return should_exit;
}

static void refill_pot(panoramix_t *data)
{
    print_message(data,
        "Druid: Ah! Yes, yes, I'm awake! Working on it! "
        "Beware I can only make %d more refills after this one.\n",
        data->remaining_refills - 1);

    pthread_mutex_lock(&data->pot_mutex);
    data->current_pot_servings = data->huit_six;
    data->remaining_refills--;

    if (data->remaining_refills == 0)
        print_message(data, "Druid: I'm out of viscum. I'm going back to... zZz\n");

    pthread_mutex_unlock(&data->pot_mutex);
}

static void signal_villagers(panoramix_t *data)
{
    for (int i = 0; i < data->nb_gitouze; i++)
        sem_post(&data->pot_refilled_sem);
}

static int can_still_refill(panoramix_t *data)
{
    int result;

    pthread_mutex_lock(&data->pot_mutex);
    result = (data->remaining_refills > 0);
    pthread_mutex_unlock(&data->pot_mutex);
    return result;
}

void *druid_thread(void *arg)
{
    panoramix_t *data = (panoramix_t *)arg;

    print_message(data, "Druid: I'm ready... but sleepy...\n");

    while (1) {
        sem_wait(&data->druid_sem);

        if (should_druid_exit(data))
            break;

        if (!can_still_refill(data))
            break;

        refill_pot(data);
        signal_villagers(data);
    }

    return NULL;
}
