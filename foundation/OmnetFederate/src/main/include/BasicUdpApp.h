//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef BASICUDPAPP_H
#define BASICUDPAPP_H

#include <omnetpp.h>
#include <UDPSocket.h>

#include <InterfaceTable.h>
#include <IPv4InterfaceData.h>
#include "InterfaceTableAccess.h"
#include "NotificationBoard.h"

#include "HLAInterface.h"


/**
 * TODO - Generated class
 */
class BasicUdpApp : public cSimpleModule, public INotifiable {

private:
    UDPSocket _socket;
	std::string _hostName;
	cModule *_hostModule;
	int _port;
	int _defaultDestPort;

    IInterfaceTable *_interfaceTable;
    NotificationBoard *_notificationBoard;

protected:
	virtual int numInitStages( void ) const;
	virtual void initialize( int stage );
	virtual void receiveChangeNotification( int category, const cPolymorphic *details );
	virtual void handleMessage(cMessage *msg);

	IInterfaceTable *getInterfaceTable( void ) { return _interfaceTable; }
	NotificationBoard *getNotificationBoard( void ) { return _notificationBoard; }

public:
	cModule *getHostModule( void ) const {
		return _hostModule;
	}

	const std::string &getHostName( void ) const {
		return _hostName;
	}

	int getPort( void ) {
		return _port;
	}
	int getDefaultDestPort( void ) {
	    return _defaultDestPort;
	}

    virtual void sendToUDP( cPacket *msg, const IPv4Address& destAddr, int destPort );
};

#endif
