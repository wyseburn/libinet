/**
 *  Version:     @(#)Network.hxx   0.0.1 23/05/2008
 *  Authors:     Hailong Xia <hlxxxx@gmail.com> 
 *  Brief  :     INET Lib header.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty 
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See 
 *  the GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General
 *  Public License along with this library; if not, write to the 
 *  Free Software Foundation, Inc., 59 Temple Place, Suite 330, 
 *  Boston, MA 02111-1307  USA
 */

#ifndef __INET_NETWORK_H__
#define __INET_NETWORK_H__

#include "Compat.hxx"
#include "Client.hxx"
#include "Listener.hxx"
#include "Time.hxx"

namespace INET 
{
    static Client* CreateClient();
    static void DestroyClient(Client* client);
    static Listener* CreateListener();
    static DestroyListener(Listener* listener)
}


#endif // #ifndef __INET_NETWORK_H__
