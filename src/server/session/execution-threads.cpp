#include "execution-threads.h"

void *exec_thread(void *arg)
{
    exec_thread_params* t_param = (exec_thread_params*) arg;

    if(!t_param)
        pthread_exit(NULL);

    sigset_t mask;
    sigfillset(&mask);
    pthread_sigmask(SIG_BLOCK, &mask, NULL);

    UserSession      *usession = NULL;
    Packet           *pack;

    INFO("debug", "* exec thread %d started \n", pthread_self());

    while(1)
    {
        usession = s_manager->getNextSessionToExecute();
        if (usession)
        {
            pack     = usession->GetPacketFromRecv();
            if (pack)
                cout << "INCOMING MESSAGE: " << pack->m_data << endl;
            //else
            //    cout << "NULL MESSAGE" << endl;
            c_manager->execute("", usession); //pack->m_data);
            usession->releaselock_exec();
        }
        usleep(5);
    }

    if (t_param)
        delete t_param;

    pthread_exit(NULL);
}

execution_threads::execution_threads()
{

}

execution_threads::~execution_threads()
{
    //for (int i = 0; i < tids.size(); i++)
    //    pthread_join(tids[i], &status);
}

void execution_threads::start_exec_threads(uint32 n)
{
    for (uint32 i = 0 ; i<n ; i++)
        start_exec_thread();
}

void execution_threads::start_exec_thread()
{
    pthread_t      tid;
    exec_thread_params* t_params = new exec_thread_params;

    tid = start_thread(&exec_thread, t_params);
    tids.push_back(tid);
}
