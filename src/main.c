/*
** EPITECH PROJECT, 2025
** PANORAMIX
** File description:
** Main
*/

#include "pano.h"

static int create_druid_thread(panoramix_t *data, pthread_t *druid)
{
    if (pthread_create(druid, NULL, druid_thread, data) != 0)
        return 0;

    usleep(5000);
    return 1;
}

static int create_gitouze_thread(panoramix_t *data, pthread_t *thread, int id)
{
    thread_arg_t *arg = malloc(sizeof(thread_arg_t));

    if (arg == NULL)
        return 0;
    arg->id = id;
    arg->data = data;
    if (pthread_create(thread, NULL, gitouze_thread, arg) != 0) {
        free(arg);
        return 0;
    }
    return 1;
}

static void join_threads(pthread_t *threads, int count, pthread_t druid)
{
    for (int i = 0; i < count; i++)
        pthread_join(threads[i], NULL);
    pthread_join(druid, NULL);
}

static int create_all_threads(panoramix_t *data)
{
    pthread_t druid;
    pthread_t *gitouze;
    int success = 1;

    gitouze = malloc(sizeof(pthread_t) * data->nb_gitouze);
    if (gitouze == NULL)
        return 84;

    if (!create_druid_thread(data, &druid)) {
        free(gitouze);
        return 84;
    }

    for (int i = 0; i < data->nb_gitouze && success; i++) {
        success = create_gitouze_thread(data, &gitouze[i], i);
        usleep(1000);
    }

    if (!success) {
        free(gitouze);
        return 84;
    }

    join_threads(gitouze, data->nb_gitouze, druid);
    free(gitouze);
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
    result = create_all_threads(&data);
    cleanup_resources(&data);
    return result;
}
