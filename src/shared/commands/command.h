#ifndef COMMAND_H
#define COMMAND_H

#include "common.h"

typedef enum cmd_type
{
    CMD_QUIT,
    CMD_INFO,
    CMD_CONNECT,
    CMD_PING,
    CMD_ACK,
    CMD_ABORT,
    CMD_SENDMSG,
    CMD_SENDPRIVMSG
    CMD_JOIN,
    CMD_LEAVE,
    CMD_USERINFO
} cmd_type;

typedef bool(*handler)(char *, void *);

class command
{
    cmd_type    type;
    char        *name;
    bool        needparam;
    handler     h;

  public:
    char *forge_packet(int, const char *);
    inline void execution_info(const char *name, client_slot *cli)
    {
        #if ENABLE_DEBUG
           cout << "* <" << name << "> executed from " << cli->host << ":" << cli->port << endl << endl;
        #endif
    }

};

command *get_command_from_name(char *, command *);
command *get_command_from_type(cmd_type, command *);

#endif