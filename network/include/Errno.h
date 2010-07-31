/*
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved
 */

#ifndef IGAME_NETWORK_ERRNO_H
#define IGAME_NETWORK_ERRNO_H

#define IGAME_ERROR_NONE                         0
#define IGAME_ERROR_ACCEPT                       1   /* Tcp accept fail */
#define IGAME_ERROR_CONNECT                      2   /* Tcp connection fail */
#define IGAME_ERROR_TCPSEND                      3   /* Tcp send fail */
#define IGAME_ERROR_TCPRECV                      4   /* Tcp recv fail */
#define IGAME_ERROR_S11N                         5   /* Serialize fail */
#define IGAME_ERROR_MSGID                        6   /* Don't match message id */
#define IGAME_ERROR_MSGHDR                       7   /* Don't match message hdr */
#define IGAME_ERROR_MSGLEN                       8   /* Invalid message len */

#endif // #ifndef IGAME_NETWORK_ERRNO_H
