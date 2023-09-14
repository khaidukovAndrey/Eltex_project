#include "vlan_tagger.h"

void logging_programm_completion(struct thread_data params)
{
    close(*params.socket);
    printL(INFO, INITIATOR, "Socket closed.");

    queue_destroy(params.sniffer_queue);
    printL(INFO, INITIATOR, "Sniffer queue destroyed.");

    queue_destroy(params.sender_queue);
    printL(INFO, INITIATOR, "Sender queue destroyed.");

    //tag_rules_clear(params.tag_rules_obj);
    printL(INFO, INITIATOR, "The memory allocated for the file config structure is cleared.");
}

void pthread_init(const char *interface_name)
{
    int sock_r;
    int saddr_len;
    int res;
    int size;
    Queue_t sniffer_queue;
    Queue_t sender_queue;
    pthread_t tid[THREADS_COUNT];
    tag_rules_t *rules;
    struct thread_data func_params = { 0 };
    struct sockaddr_ll saddr = { 0 };

    start_log();

    sock_r = socket(
            AF_PACKET,
            SOCK_RAW,
            htons(ETH_P_ALL));
    if (sock_r < 0)
    {
        printf("Symbols was written: %d\n",
               printL(ERROR, INITIATOR, "Socket opening error (error code: %d)!", errno));
        stop_log();
        exit(EXIT_FAILURE);
    }

    if (setsockopt(
            sock_r,
            SOL_SOCKET,
            SO_BINDTODEVICE,
            interface_name,
            strlen(interface_name)) < 0)
    {
        printf("Symbols was written: %d\n",
               printL(ERROR, INITIATOR, "Error setting up network interface for socket (error code: %d)!", errno));
        close(sock_r);
        stop_log();
        exit(EXIT_FAILURE);
    }

    saddr.sll_family = AF_PACKET;
    saddr.sll_protocol = htons(ETH_P_ALL);

    if (init(&sniffer_queue) == -1)
    {
        printf("Symbols was written: %d\n",
               printL(ERROR, INITIATOR, "Queue initialization error (error code: %d)", errno));
        close(sock_r);
        stop_log();
        exit(EXIT_FAILURE);
    }

    if (init(&sender_queue) == -1)
    {
        printf("Symbols was written: %d\n",
               printL(ERROR, INITIATOR, "Queue initialization error (error code: %d)", errno));
        close(sock_r);
        stop_log();
        exit(EXIT_FAILURE);
    }

    res = config_file_check();
    if (res != 0)
    {
        printL(ERROR, PARSER, "Error opening/closing config file (error code: %d)!", res);
        close(sock_r);
        queue_destroy(&sniffer_queue);
        queue_destroy(&sender_queue);
    }

    res = tag_rules_init(&rules, 64);
    if (res != 0)
    {
        printL(ERROR, PARSER, "Error in allocating memory for the configuration file structure (error code: %d)!", res);
        close(sock_r);
        queue_destroy(&sniffer_queue);
        queue_destroy(&sender_queue);
    }

    size = config_file_read(rules);
    if (size < 0)
    {
        printL(ERROR, PARSER, "Error reading file/writing data to structure (error code: %d)!", size);
        close(sock_r);
        queue_destroy(&sniffer_queue);
        queue_destroy(&sender_queue);
        tag_rules_clear(&rules);
    }

    res = tag_rules_check_collisions(rules, size);
    if (res != 0)
    {
        printL(ERROR, PARSER, "Error when checking the configuration file for collisions (error code: %d)!", res);
        close(sock_r);
        queue_destroy(&sniffer_queue);
        queue_destroy(&sender_queue);
        tag_rules_clear(&rules);
    }

    func_params.socket = &sock_r;
    func_params.sniffer_queue = &sniffer_queue;
    func_params.sender_queue = &sender_queue;
    func_params.saddr = &saddr;
    saddr_len = sizeof(saddr);
    func_params.saddr_len = &saddr_len;
    func_params.tag_rules_obj = rules;

    pthread_create(&tid[0], NULL, packet_sniffer, &func_params);
    printL(INFO, INITIATOR, "Thread №%d started", 0);

    pthread_create(&tid[1], NULL, tagger, &func_params);
    printL(INFO, INITIATOR, "Thread №%d started", 0);

    pthread_create(&tid[2], NULL, packet_sender, &func_params);
    printL(INFO, INITIATOR, "Thread №%d started", 1);

    for (int j = 0; j < THREADS_COUNT; j++)
    {
        pthread_join(tid[j], NULL);
        printL(INFO, INITIATOR, "Thread №%d completed", j);
    }

    logging_programm_completion(func_params);

    stop_log();
}