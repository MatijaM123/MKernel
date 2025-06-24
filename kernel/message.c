#include "message.h"
#include "scheduler.h"
#include "process.h"


int send_message_rpc(int sender_pid, int receiver_pid, const char* data) {
    KBP* receiver = get_process_by_pid(receiver_pid);
    KBP* sender = get_process_by_pid(sender_pid);
    if (!receiver || !sender) return -1;

    if (receiver->msg_queue.count >= MAX_MSG_QUEUE) return -2;

    Message msg = {
        .sender_pid = sender_pid,
        .receiver_pid = receiver_pid,
        .is_reply = 0,
        .reply_expected = 1,
    };
    strncpy(msg.data, data, MAX_MSG_DATA);

    // Stavi poruku u red primaoca
    enqueue_message(&receiver->msg_queue, &msg);

    // Blokiraj pošiljaoca dok ne dobije odgovor
    sender->waiting_for_reply = 1;
    scheduler_block_on(sender, WAIT_MESSAGE, receiver_pid); // čeka odgovor od servera

    scheduler_tick(); // pređe se na sledeći proces

    // Kada se probudi, odgovor je u reply_buffer
    return 0;
}



int receive_message(int receiver_pid, Message* out_msg) {
    KBP* receiver = get_process_by_pid(receiver_pid);
    if (!receiver || receiver->msg_queue.count == 0)
        return -1;

    dequeue_message(&receiver->msg_queue, out_msg);
    return 0;
}


int reply_message(int server_pid, int client_pid, const char* data) {
    KBP* client = get_process_by_pid(client_pid);
    if (!client || !client->waiting_for_reply) return -1;

    client->reply_buffer.sender_pid = server_pid;
    client->reply_buffer.receiver_pid = client_pid;
    client->reply_buffer.is_reply = 1;
    client->reply_buffer.reply_expected = 0;
    strncpy(client->reply_buffer.data, data, MAX_MSG_DATA);

    client->waiting_for_reply = 0;

    // Odblokiraj proces koji čeka poruku
    scheduler_unblock_all_on(WAIT_MESSAGE, server_pid);

    return 0;
}