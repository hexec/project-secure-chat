#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include <sys/epoll.h>
#include <pthread.h>

#include <iostream>
#include <sstream>
#include <string>

#include "../../shared/networking/socket-base.h"
#include "../../shared/callback.h"

using namespace std;

#define MAX_CONNECTIONS (64)

void* EpollThread(void* arg);

/*
net_task new_connection_net_task(int sock)
{
    net_task n_task;
    n_task.ptr = (void*) sock;
    n_task.p_pack = NULL;
    n_task.type_task = NEW;
    return n_task;
}
*/

class SocketServer
{
    struct epoll_event event, *events;

    struct sockaddr_in serveraddr, clientaddr;
    struct addrinfo serverinfo, *serverinfo_res;

    int sock_listen;
    int epoll_fd;


    inline void SetupAddrInfo(int family, int socktype, int protocol);
    inline void SetBlocking(int, const bool) throw(SocketException);

    void SetupEpoll() throw(SocketException);
    void SetupSocket(int port) throw(SocketException);

    EventCallback<void, void*> cb_notify;

    friend void* EpollThread(void* arg);

  public:
    SocketServer() throw(SocketException);
    ~SocketServer();

    void Init(int) throw(SocketException);

    void InitCallback(void (*fptr)(void*))
    {
        //cb_notify->RegisterCb(fptr);
    }

    /*void new_connection_init()
    {
        b_new_connection = false;
        pthread_cond_init(&c_new_connection);
        pthread_mutex_init(&m_new_connection);
        pthread_mutex_lock(&m_new_connection);
    }

    void new_connection_cond_signal()
    {

        pthread_cond_signal(&c_new_connection);
    }
    void new_connection_cond_wait()
    {
        pthread_cond_wait(&c_new_connection);
    }

    void new_connection_enter_lock()
    {
        pthread_mutex_lock(&m_new_connection_enter);
    }

    void new_connection_exit_unlock()
    {
        pthread_mutex_lock(&m_new_connection_exit);
    }*/
};

#endif
