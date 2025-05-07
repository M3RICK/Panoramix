/*
** EPITECH PROJECT, 2025
** PANORAMIX
** File description:
** Main
*/

#include "pano.h"

static int create_and_run_threads(panoramix_t *data)
{
    pthread_t druid;
    pthread_t *villagers;
    thread_arg_t **args;
    int success = 1;

    // Allocate memory for threads and arguments
    villagers = malloc(sizeof(pthread_t) * data->nb_gitouze);
    args = malloc(sizeof(thread_arg_t*) * data->nb_gitouze);
    if (villagers == NULL || args == NULL) {
        free(villagers);
        free(args);
        return 84;
    }

    // Create druid thread first
    if (pthread_create(&druid, NULL, druid_thread, data) != 0) {
        free(villagers);
        free(args);
        return 84;
    }

    // Create all villager threads
    for (int i = 0; i < data->nb_gitouze; i++) {
        args[i] = malloc(sizeof(thread_arg_t));
        if (args[i] == NULL) {
            success = 0;
            break;
        }
        args[i]->id = i;
        args[i]->data = data;

        if (pthread_create(&villagers[i], NULL, gitouze_thread, args[i]) != 0) {
            free(args[i]);
            success = 0;
            break;
        }
    }

    if (!success) {
        // Clean up on failure
        free(villagers);
        for (int i = 0; i < data->nb_gitouze; i++)
            if (args[i] != NULL)
                free(args[i]);
        free(args);
        return 84;
    }

    // Wait for all threads to finish
    for (int i = 0; i < data->nb_gitouze; i++)
        pthread_join(villagers[i], NULL);
    pthread_join(druid, NULL);

    // Clean up resources
    free(villagers);
    free(args);
    return 0;
}

int main(int argc, char **argv)
{
    panoramix_t data;
    int result;

    if (!parse_arguments(argc, argv, &data))
        return 84;
    if (!initialize_data(&data))
        return 84;

    result = create_and_run_threads(&data);

    cleanup_resources(&data);
    return result;
}
