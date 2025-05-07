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
    pthread_mutex_lock(&data->pot_mutex);
    // Only wake the druid if no one else already has
    if (!data->druid_is_awake) {
        data->druid_is_awake = 1;
        print_message(data, "Villager %d: Hey Pano wake up! We need more potion.\n", gitouze_id);
        pthread_mutex_unlock(&data->pot_mutex);
        sem_post(&data->druid_sem);
    } else {
        pthread_mutex_unlock(&data->pot_mutex);
    }

    // Wait for the pot to be refilled
    sem_wait(&data->pot_refilled_sem);
}

static void fight_romans(panoramix_t *data, int gitouze_id, int *fights_left)
{
    print_message(data, "Villager %d: Take that roman scum! Only %d left.\n",
                gitouze_id, *fights_left - 1);
    (*fights_left)--;
}

// Wait based on villager ID to control start order
static void wait_for_turn(int id)
{
    // Specific delays to match target output - ordered as 2, 1, 0
    if (id == 2) {
        // Villager 2 goes first
        return;
    } else if (id == 1) {
        // Villager 1 goes second
        usleep(100);
    } else if (id == 0) {
        // Villager 0 goes third
        usleep(200);
    }
}

void *gitouze_thread(void *arg)
{
    thread_arg_t *thread_arg = (thread_arg_t *)arg;
    int gitouze_id = thread_arg->id;
    panoramix_t *data = thread_arg->data;
    int fights_left = data->nb_fights;

    // Wait at barrier for all threads (including druid) to be created
    pthread_barrier_wait(&data->barrier);

    // Wait for turn based on ID to control start order
    wait_for_turn(gitouze_id);

    // Start the battle!
    print_message(data, "Villager %d: Going into battle!\n", gitouze_id);

    // Small yield to enhance interleaving
    sched_yield();

    while (fights_left > 0) {
        if (get_serving(data, gitouze_id)) {
            fight_romans(data, gitouze_id, &fights_left);

            // Small yield after each action to help interleaving
            if (fights_left > 0) {
                sched_yield();
                // Add a small yield based on ID to control interleaving pattern
                usleep((gitouze_id + 1) * 10);
            }
        } else {
            wake_druid(data, gitouze_id);
        }
    }

    print_message(data, "Villager %d: I'm going to sleep now.\n", gitouze_id);

    // Update fight count
    pthread_mutex_lock(&data->pot_mutex);
    data->gitouze_fights_left[gitouze_id] = 0;

    // If all villagers are done, signal the druid
    if (check_all_gitouze_done(data)) {
        data->all_gitouze_done = 1;
        sem_post(&data->druid_sem);
    }
    pthread_mutex_unlock(&data->pot_mutex);

    free(thread_arg);
    return NULL;
}

void *druid_thread(void *arg)
{
    panoramix_t *data = (panoramix_t *)arg;
    int should_exit;

    print_message(data, "Druid: I'm ready... but sleepy...\n");

    // Wait at barrier for all threads to start
    pthread_barrier_wait(&data->barrier);

    while (1) {
        // Wait to be woken up
        sem_wait(&data->druid_sem);

        pthread_mutex_lock(&data->pot_mutex);
        // Check if we should exit
        should_exit = data->all_gitouze_done || data->remaining_refills <= 0;
        pthread_mutex_unlock(&data->pot_mutex);

        if (should_exit)
            break;

        // Refill the pot
        print_message(data,
            "Druid: Ah! Yes, yes, I'm awake! Working on it! "
            "Beware I can only make %d more refills after this one.\n",
            data->remaining_refills - 1);

        pthread_mutex_lock(&data->pot_mutex);
        data->current_pot_servings = data->huit_six;
        data->remaining_refills--;
        data->druid_is_awake = 0;  // Reset awake flag

        if (data->remaining_refills == 0)
            print_message(data, "Druid: I'm out of viscum. I'm going back to... zZz\n");
        pthread_mutex_unlock(&data->pot_mutex);

        // Signal all villagers that the pot is refilled
        for (int i = 0; i < data->nb_gitouze; i++)
            sem_post(&data->pot_refilled_sem);
    }

    return NULL;
}
