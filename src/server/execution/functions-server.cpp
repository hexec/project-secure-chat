#include "functions-server.h"

bool send_message(void* params)
{
    handler_params* hpar = (handler_params*)params;
    std::list<uint32> id_list;
    int sender_id = s_manager->GetUsersessionId(hpar->usession);

    Packet pack;
    pack.SetOpcode(0);
    pack.m_data = hpar->params;

    s_manager->GetIdList(&id_list);
    std::list<uint32>::iterator itr;
    for(itr = id_list.begin(); itr != id_list.end() ; itr++)
        if (*(itr) != sender_id)
            s_manager->SendPacketTo(*(itr), &pack);

    return true;
}

bool update_user_list(void* params)
{

    return true;
}
