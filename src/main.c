/*
** EPITECH PROJECT, 2025
** PANORAMIX
** File description:
** Main
*/

#include "pano.h"

int allocate_thread_memory(panoramix_t *data, pthread_t **zonzons,
    thread_arg_t ***args)
{
    *zonzons = malloc(sizeof(pthread_t) *data->nb_gitouze);
    *args = malloc(sizeof(thread_arg_t *) *data->nb_gitouze);
    if (*zonzons == NULL || *args == NULL) {
        free(*zonzons);
        free(*args);
        return 0;
    }
    return 1;
}

int create_druid_thread(panoramix_t *data, pthread_t *druid)
{
    if (pthread_create(druid, NULL, pano_thread, data) != 0)
        return 0;
    return 1;
}

int create_zonzon_threads(panoramix_t *data, pthread_t *zonzons,
    thread_arg_t **args)
{
    for (int i = data->nb_gitouze - 1; i >= 0; i--) {
        args[i] = malloc(sizeof(thread_arg_t));
        if (args[i] == NULL)
            return 0;
        args[i]->id = i;
        args[i]->data = data;
        if (pthread_create(&zonzons[i], NULL, zonzon_thread, args[i]) != 0) {
            free(args[i]);
            return 0;
        }
    }
    return 1;
}

void join_all_threads(panoramix_t *data, pthread_t *zonzons,
    pthread_t druid)
{
    for (int i = 0; i < data->nb_gitouze; i++)
        pthread_join(zonzons[i], NULL);
    pthread_join(druid, NULL);
}

static void free_thread_resources(panoramix_t *data, pthread_t *zonzons,
    thread_arg_t **args)
{
    for (int i = 0; i < data->nb_gitouze; i++) {
        if (args[i] != NULL)
            free(args[i]);
    }
    free(zonzons);
    free(args);
}

static int create_and_run_threads(panoramix_t *data)
{
    pthread_t druid;
    pthread_t *zonzons = NULL;
    thread_arg_t **args = NULL;

    if (!allocate_thread_memory(data, &zonzons, &args))
        return 84;
    if (!create_druid_thread(data, &druid)) {
        free(zonzons);
        free(args);
        return 84;
    }
    if (!create_zonzon_threads(data, zonzons, args)) {
        free_thread_resources(data, zonzons, args);
        return 84;
    }
    join_all_threads(data, zonzons, druid);
    free_thread_resources(data, zonzons, args);
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
