#define MAX_MSG_DATA 128
#define MAX_MSG_QUEUE 16

typedef struct {
    int sender_pid;
    int receiver_pid;
    int is_reply;          // 0 = zahtev, 1 = odgovor
    int reply_expected;    // 1 ako pošiljalac očekuje odgovor
    char data[MAX_MSG_DATA];
} Message;

typedef struct {
    Message queue[MAX_MSG_QUEUE];
    int head, tail, count;
} MessageQueue;


int send_message_rpc(int sender_pid, int receiver_pid, const char* data);
int receive_message(int receiver_pid, Message* out_msg);
int reply_message(int server_pid, int client_pid, const char* data);