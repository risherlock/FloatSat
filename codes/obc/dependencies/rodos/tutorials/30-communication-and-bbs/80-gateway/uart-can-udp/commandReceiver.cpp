#include "rodos.h"
#include "topics.h"

static Application  receiverName("commandReceiver", 1116);

struct CommandReceiver : public Subscriber {
    CommandReceiver() : Subscriber(command, "CommandReceiver") { }
    uint32_t put([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const size_t len, const void* data, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        const UserCmd* cmd = (const UserCmd*)data;
        PRINTF("command  %ld bytes: %s\n",
            static_cast<long>(cmd->cmdLen),
            cmd->cmdLine);
        return 1;
    }
} commandReceiver;

