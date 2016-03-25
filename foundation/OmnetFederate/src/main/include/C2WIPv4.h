//
// Copyright (C) 2004 Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#ifndef __INET_C2WIPV4_H
#define __INET_C2WIPV4_H

#include "INETDefs.h"

#include "IARPCache.h"
#include "ICMPAccess.h"
#include "ILifecycle.h"
#include "INetfilter.h"
#include "IPv4Datagram.h"
#include "IPv4FragBuf.h"
#include "ProtocolMap.h"
#include "QueueBase.h"


#ifdef WITH_MANET
#include "ControlManetRouting_m.h"
#endif

#include "INotifiable.h"
#include "IPv4Datagram.h"
#include "NotificationBoard.h"

#include "HLAInterface.h"

#include <boost/shared_ptr.hpp>
#include <deque>
#include <map>
#include <set>

class ARPPacket;
class ICMPMessage;
class IInterfaceTable;
class IPv4Datagram;
class IRoutingTable;
class NotificationBoard;

/**
 * Implements the IPv4 protocol.
 */
class INET_API C2WIPv4 : public QueueBase, public INetfilter, public ILifecycle, public cListener, public INotifiable
{
  public:
    typedef QueueBase Super;
    /**
     * Represents an IPv4Datagram, queued by a Hook
     */
    class QueuedDatagramForHook {
      public:
        QueuedDatagramForHook(IPv4Datagram* datagram, const InterfaceEntry* inIE, const InterfaceEntry* outIE, const IPv4Address& nextHopAddr, IHook::Type hookType) :
                datagram(datagram), inIE(inIE), outIE(outIE), nextHopAddr(nextHopAddr), hookType(hookType) {}
        virtual ~QueuedDatagramForHook() {}

        IPv4Datagram* datagram;
        const InterfaceEntry* inIE;
        const InterfaceEntry* outIE;
        IPv4Address nextHopAddr;
        const IHook::Type hookType;
    };
    typedef std::map<IPv4Address, cPacketQueue> PendingPackets;

  protected:
    bool isDsr;
    static simsignal_t completedARPResolutionSignal;
    static simsignal_t failedARPResolutionSignal;

    IRoutingTable *rt;
    IInterfaceTable *ift;
    NotificationBoard *nb;
    IARPCache *arp;
    ICMPAccess icmpAccess;
    cGate *arpInGate;
    cGate *arpOutGate;
    int transportInGateBaseId;
    int queueOutGateBaseId;

    // config
    int defaultTimeToLive;
    int defaultMCTimeToLive;
    simtime_t fragmentTimeoutTime;
    bool forceBroadcast;
    bool useProxyARP;

    // working vars
    bool isUp;
    long curFragmentId; // counter, used to assign unique fragmentIds to datagrams
    IPv4FragBuf fragbuf;  // fragmentation reassembly buffer
    simtime_t lastCheckTime; // when fragbuf was last checked for state fragments
    ProtocolMapping mapping; // where to send packets after decapsulation

    // ARP related
    PendingPackets pendingPackets;  // map indexed with IPv4Address for outbound packets waiting for ARP resolution

    // statistics
    int numMulticast;
    int numLocalDeliver;
    int numDropped;  // forwarding off, no outgoing interface, too large but "don't fragment" is set, TTL exceeded, etc
    int numUnroutable;
    int numForwarded;

    std::string _hostFullName;


    //
    // CODE FOR NODE ATTACK
    //
    bool _nodeAttack;

public:
    std::string getHostFullName( void ) {
        return _hostFullName;
    }

    //
    // CODE FOR SNIFFER ATTACK
    //
    typedef std::set< IPv4Address > IPAddressSet;

private:
    IPAddressSet _listenerIPAddressSet;
    bool _hasListeners;

    void addListener( const IPv4Address &ipAddress ) {
        _listenerIPAddressSet.insert( ipAddress );
        _hasListeners = true;
    }

    void deleteListener( const IPv4Address &ipAddress ) {
        _listenerIPAddressSet.erase( ipAddress );
        if ( _listenerIPAddressSet.empty() ) {
            _hasListeners = false;
        }
    }


    //
    // CODE FOR NETWORK FILTER ATTACK
    //
    bool _hasFilters;

public:
    typedef HLAInterface::NetworkAddress NetworkAddress;
    typedef HLAInterface::NetworkAddressComparator NetworkAddressComparator;
    typedef HLAInterface::NetworkAddressSet NetworkAddressSet;

    typedef std::pair< NetworkAddress, NetworkAddress > NetworkAddressSDPair;

    struct NetworkAddressSDPairComparator {
        bool operator()( const NetworkAddressSDPair &networkAddressSDPair1, const NetworkAddressSDPair &networkAddressSDPair2 ) {
            NetworkAddressComparator networkAddressComparator;

            {
                const NetworkAddress &sourceNetworkAddress1 = networkAddressSDPair1.first;
                const NetworkAddress &sourceNetworkAddress2 = networkAddressSDPair2.first;
                if (  networkAddressComparator( sourceNetworkAddress1, sourceNetworkAddress2 )  ) {
                    return true;
                }
                if (  networkAddressComparator( sourceNetworkAddress2, sourceNetworkAddress1 )  ) {
                    return false;
                }
            }

            if (  networkAddressComparator( networkAddressSDPair1.second, networkAddressSDPair2.second )  ) {
                return true;
            }
            return false;
        }
    };

    typedef std::set< NetworkAddressSDPair, NetworkAddressSDPairComparator > NetworkAddressSDPairSet;

private:
    NetworkAddressSet _networkAddressSet;
    NetworkAddressSDPairSet _filterAttackSet;

    void addFilter( const NetworkAddress &srcNetworkAddress, const NetworkAddress &dstNetworkAddress ) {
        std::cerr << "Host \"" << _hostFullName << "\":  adding filter (" << srcNetworkAddress << "," << dstNetworkAddress << ")" << std::endl;
        _hasFilters = true;
        _filterAttackSet.insert(  NetworkAddressSDPair( srcNetworkAddress, dstNetworkAddress )  );
    }

    void deleteFilter( const NetworkAddress &srcNetworkAddress, const NetworkAddress &dstNetworkAddress ) {
        NetworkAddressSDPair networkAddressSDPair( srcNetworkAddress, dstNetworkAddress );

        if ( _filterAttackSet.find( networkAddressSDPair ) == _filterAttackSet.end() ) {
            std::cerr << "Host \"" << _hostFullName << "\":  not deleting filter (" << srcNetworkAddress << "," << dstNetworkAddress
             << "):  filter not present" << std::endl;
        } else {
            std::cerr << "Host \"" << _hostFullName << "\":  deleting filter (" << srcNetworkAddress << "," << dstNetworkAddress
             << ")" << std::endl;
            _filterAttackSet.erase( networkAddressSDPair );
            if ( _filterAttackSet.empty() ) {
                _hasFilters = false;
            }
        }
    }

    bool isFiltered( const IPv4Address &sourceIPv4Address, const IPv4Address &destinationIPv4Address );

    //
    // CODE FOR NETWORK ATTACK
    //
    typedef std::map< NetworkAddress, std::string, NetworkAddressComparator > NetworkAddressInterfaceNameMap;
    typedef std::set< std::string > AttackedInterfaceSet;

private:
    bool _hasNetworkAttack;
    NetworkAddressInterfaceNameMap _networkAddressInterfaceNameMap;
    AttackedInterfaceSet _attackedInterfaceSet;

    //
    // CODE FOR REPLAY/OUT-OF-ORDER-PACKETS ATTACK
    //
public:
    class DatagramData {
        friend std::ostream &::operator<<( std::ostream &os, const DatagramData &datagramData );
    private:
        IPv4Datagram *_datagram;
        const InterfaceEntry *_fromIE;
        const InterfaceEntry *_destIE;
        const IPv4Address _nextHopAddr;

    public:
        DatagramData( IPv4Datagram *datagram, const InterfaceEntry *fromIE, const InterfaceEntry *destIE, const IPv4Address &nextHopAddr ) :
         _datagram( datagram ), _fromIE( fromIE ), _destIE( destIE ), _nextHopAddr( nextHopAddr ) { }

        IPv4Datagram *getIPDatagram( void ) {
            return _datagram;
        }
        const InterfaceEntry *getFromInterfaceEntry( void ) const {
            return _fromIE;
        }
        const InterfaceEntry *getDestInterfaceEntry( void ) const {
            return _destIE;
        }
        const IPv4Address getNextHopAddr( void ) const {
            return _nextHopAddr;
        }
    };

    typedef boost::shared_ptr< DatagramData > DatagramDataSP;
    typedef std::deque< DatagramDataSP > DatagramDataSPDeque;

    class ReplayBuffer {
        friend std::ostream &::operator<<( std::ostream &os, const ReplayBuffer &replayBuffer );

    private:
        DatagramDataSPDeque _datagramDataSPDeque;

        SimTime _recordStartTime;
        SimTime _lastRecordTime;
        double _recordDuration;
        SimTime _recordEndTime;

        unsigned int _serialNo;
        bool _playing;

    public:
        ReplayBuffer( void ) : _serialNo( 0 ), _playing( false ) { }

        void setRecordDuration( double recordDuration ) {
            _recordStartTime = simTime();
            _lastRecordTime = _recordStartTime;
            _recordDuration = recordDuration;
            _recordEndTime = _recordStartTime + _recordDuration;
        }

    private:
        void addIPDatagram( IPv4Datagram *datagram, const InterfaceEntry *fromIE, const InterfaceEntry *destIE, const IPv4Address &nextHopAddr ) {

            SimTime currentSimTime = simTime();
            datagram->setTimestamp( currentSimTime - _lastRecordTime );
            _lastRecordTime = currentSimTime;

            DatagramDataSP datagramDataSP(  new DatagramData( datagram, fromIE, destIE, nextHopAddr )  );
            _datagramDataSPDeque.push_back( datagramDataSP );
        }

    public:
        void addIPDatagramSerial( IPv4Datagram *datagram, const InterfaceEntry *fromIE, const InterfaceEntry *destIE, const IPv4Address &nextHopAddr ) {
            if ( simTime() > _recordEndTime || _playing  ) {
                return;
            }
            addIPDatagram( datagram, fromIE, destIE, nextHopAddr );
        }

        void addIPDatagramRandom( IPv4Datagram *datagram, const InterfaceEntry *fromIE, const InterfaceEntry *destIE, const IPv4Address &nextHopAddr ) {
            addIPDatagram( datagram, fromIE, destIE, nextHopAddr );
        }

        bool empty( void ) const {
            return _datagramDataSPDeque.empty();
        }

        void setPlaying( bool playing ) {
            _playing = _datagramDataSPDeque.empty() ? false : playing;
        }

        void setPlayingRandom( bool playing ) {
            _playing = playing;
        }

        bool getPlaying( void ) {
            if ( _datagramDataSPDeque.empty() ) _playing = false;
            return _playing;
        }

        bool getPlayingRandom( void ) {
            return _playing;
        }

        double getRecordDuration( void ) const {
            return _recordDuration;
        }

        SimTime getRecordEndTime( void ) const {
            return _recordEndTime;
        }

        SimTime getNextTimeOffset( void ) const {
            return _datagramDataSPDeque.empty() ? -1 : _datagramDataSPDeque[ _serialNo ]->getIPDatagram()->getTimestamp();
        }

        DatagramDataSP getSerial( void ) {
            if ( _datagramDataSPDeque.empty() ) {
                return DatagramDataSP();
            }
            DatagramDataSP datagramDataSP = _datagramDataSPDeque[ _serialNo ];
            if ( ++_serialNo >= _datagramDataSPDeque.size() ) {
                _serialNo = 0;
            }
            return datagramDataSP;
        }

        DatagramDataSP getRandom( void );

        void reset( double recordDuration ) {
            _recordStartTime = simTime();
            _recordDuration = recordDuration;
            _recordEndTime = _recordStartTime + _recordDuration;
            _datagramDataSPDeque.clear();
            _serialNo = 0;
            _playing = false;
        }
    };


    typedef boost::shared_ptr< ReplayBuffer > ReplayBufferSP;

    typedef std::map< NetworkAddressSDPair, ReplayBufferSP, NetworkAddressSDPairComparator > NetworkAddressReplayBufferSPMap;
    typedef std::list< ReplayBufferSP > ReplayBufferSPList;

private:
    bool _hasSerialReplayBufferSPMap;
    NetworkAddressReplayBufferSPMap _serialReplayBufferSPMap;

    bool _hasRandomReplayBufferSPMap;
    NetworkAddressReplayBufferSPMap _randomReplayBufferSPMap;

public:
    //
    // CODE FOR REPLAY ATTACK
    //
    ReplayBufferSPList getSerialReplayBufferSPList( unsigned int sourceIPv4Address, unsigned int destinationIPv4Address );

    ReplayBufferSP &getSerialReplayBufferSP( const NetworkAddress &sourceNetworkAddress, const NetworkAddress &destinationNetworkAddress ) {
        _hasSerialReplayBufferSPMap = true;
        ReplayBufferSP &replayBufferSP = _serialReplayBufferSPMap[ NetworkAddressSDPair( sourceNetworkAddress, destinationNetworkAddress ) ];
        if ( replayBufferSP == 0 ) {
            replayBufferSP = ReplayBufferSP( new ReplayBuffer() );
        }
        return replayBufferSP;
    }

    void removeSerialReplayBufferSP( const NetworkAddress &sourceNetworkAddress, const NetworkAddress &destinationNetworkAddress );


    //
    // CODE FOR OUT-OF-ORDER-PACKETS ATTACK
    //
    ReplayBufferSPList getRandomReplayBufferSPList( unsigned int sourceIPv4Address, unsigned int destinationIPv4Address );

    ReplayBufferSP &getRandomReplayBufferSP( const NetworkAddress &sourceNetworkAddress, const NetworkAddress &destinationNetworkAddress ) {
        _hasRandomReplayBufferSPMap = true;
        ReplayBufferSP &replayBufferSP = _randomReplayBufferSPMap[ NetworkAddressSDPair( sourceNetworkAddress, destinationNetworkAddress ) ];
        if ( replayBufferSP == 0 ) {
            replayBufferSP = ReplayBufferSP( new ReplayBuffer() );
        }
        return replayBufferSP;
    }

    void removeRandomReplayBufferSP( const NetworkAddress &sourceNetworkAddress, const NetworkAddress &destinationNetworkAddress );

    //
    // CODE FOR NODE DELAY ATTACK
    //
    bool _nodeDelayed;
    double _nodeDelayMean;
    double _nodeDelayStdDev;

    // hooks
    typedef std::multimap<int, IHook*> HookList;
    HookList hooks;
    typedef std::list<QueuedDatagramForHook> DatagramQueueForHooks;
    DatagramQueueForHooks queuedDatagramsForHooks;

    static simsignal_t iPv4PromiscousPacket;
  protected:
    // utility: look up interface from getArrivalGate()
    virtual const InterfaceEntry *getSourceInterfaceFrom(cPacket *packet);

    // utility: look up route to the source of the datagram and return its interface
    virtual const InterfaceEntry *getShortestPathInterfaceToSource(IPv4Datagram *datagram);

    // utility: show current statistics above the icon
    virtual void updateDisplayString();

    // utility: processing requested ARP resolution completed
    void arpResolutionCompleted(IARPCache::Notification *entry);

    // utility: processing requested ARP resolution timed out
    void arpResolutionTimedOut(IARPCache::Notification *entry);

    /**
     * Encapsulate packet coming from higher layers into IPv4Datagram, using
     * the given control info. Override if you subclassed controlInfo and/or
     * want to add options etc to the datagram.
     */
    virtual IPv4Datagram *encapsulate(cPacket *transportPacket, IPv4ControlInfo *controlInfo);

    /**
     * Creates a blank IPv4 datagram. Override when subclassing IPv4Datagram is needed
     */
    virtual IPv4Datagram *createIPv4Datagram(const char *name);

    /**
     * Handle IPv4Datagram messages arriving from lower layer.
     * Decrements TTL, then invokes routePacket().
     */
    virtual void handleIncomingDatagram(IPv4Datagram *datagram, const InterfaceEntry *fromIE);

    // called after PREROUTING Hook (used for reinject, too)
    virtual void preroutingFinish(IPv4Datagram *datagram, const InterfaceEntry *fromIE, const InterfaceEntry *destIE, IPv4Address nextHopAddr);

    /**
     * Handle messages (typically packets to be send in IPv4) from transport or ICMP.
     * Invokes encapsulate(), then routePacket().
     */
    virtual void handlePacketFromHL(cPacket *packet);

    /**
     * TODO
     */
    virtual void handlePacketFromARP(cPacket *packet);

    /**
     * Routes and sends datagram received from higher layers.
     * Invokes datagramLocalOutHook(), then routePacket().
     */
    virtual void datagramLocalOut(IPv4Datagram* datagram, const InterfaceEntry* destIE, IPv4Address nextHopAddr);

    /**
     * Handle incoming ARP packets by sending them over to ARP.
     */
    virtual void handleIncomingARPPacket(ARPPacket *packet, const InterfaceEntry *fromIE);

    /**
     * Handle incoming ICMP messages.
     */
    virtual void handleIncomingICMP(ICMPMessage *packet);

    /**
     * Performs unicast routing. Based on the routing decision, it sends the
     * datagram through the outgoing interface.
     */
    virtual void routeUnicastPacket(IPv4Datagram *datagram, const InterfaceEntry *fromIE, const InterfaceEntry *destIE, IPv4Address requestedNextHopAddress);
    virtual void continueRouteUnicastPacket(IPv4Datagram *datagram, const InterfaceEntry *fromIE, const InterfaceEntry *destIE, IPv4Address requestedNextHopAddress);

    // called after FORWARD Hook (used for reinject, too)
    void routeUnicastPacketFinish(IPv4Datagram *datagram, const InterfaceEntry *fromIE, const InterfaceEntry *destIE, IPv4Address nextHopAddr);

    /**
     * Broadcasts the datagram on the specified interface.
     * When destIE is NULL, the datagram is broadcasted on each interface.
     */
    virtual void routeLocalBroadcastPacket(IPv4Datagram *datagram, const InterfaceEntry *destIE);

    /**
     * Determines the output interface for the given multicast datagram.
     */
    virtual const InterfaceEntry *determineOutgoingInterfaceForMulticastDatagram(IPv4Datagram *datagram, const InterfaceEntry *multicastIFOption);

    /**
     * Forwards packets to all multicast destinations, using fragmentAndSend().
     */
    virtual void forwardMulticastPacket(IPv4Datagram *datagram, const InterfaceEntry *fromIE);

    /**
     * Perform reassembly of fragmented datagrams, then send them up to the
     * higher layers using sendToHL().
     */
    virtual void reassembleAndDeliver(IPv4Datagram *datagram);

    // called after LOCAL_IN Hook (used for reinject, too)
    virtual void reassembleAndDeliverFinish(IPv4Datagram *datagram);

    /**
     * Decapsulate and return encapsulated packet after attaching IPv4ControlInfo.
     */
    virtual cPacket *decapsulate(IPv4Datagram *datagram);

    /**
     * Call PostRouting Hook and continue with fragmentAndSend() if accepted
     */
    virtual void fragmentPostRouting(IPv4Datagram *datagram, const InterfaceEntry *ie, IPv4Address nextHopAddr);

    /**
     * Fragment packet if needed, then send it to the selected interface using
     * sendDatagramToOutput().
     */
    virtual void fragmentAndSend(IPv4Datagram *datagram, const InterfaceEntry *ie, IPv4Address nextHopAddr);

    /**
     * Send datagram on the given interface.
     */
    virtual void sendDatagramToOutput(IPv4Datagram *datagram, const InterfaceEntry *ie, IPv4Address nextHopAddr);

    virtual MACAddress resolveNextHopMacAddress(cPacket *packet, IPv4Address nextHopAddr, const InterfaceEntry *destIE);

    virtual void sendPacketToIeee802NIC(cPacket *packet, const InterfaceEntry *ie, const MACAddress& macAddress, int etherType);

    virtual void sendPacketToNIC(cPacket *packet, const InterfaceEntry *ie);

  public:
    C2WIPv4() :
     _nodeAttack( false ),
     _hasListeners( false ),
     _hasFilters( false ),
     _hasNetworkAttack( false ),
     _hasSerialReplayBufferSPMap( false ),
     _hasRandomReplayBufferSPMap( false ),
     _nodeDelayed( false ),
     _nodeDelayMean( 0.0 ),
     _nodeDelayStdDev( 0.0 ) { rt = NULL; ift = NULL; arp = NULL; arpOutGate = NULL; }

  protected:
    virtual int numInitStages() const { return 6; }

    void recordNetworkInformation( void );

    virtual void receiveChangeNotification( int category, const cPolymorphic *details );
    virtual void initialize( int stage );
    virtual void handleMessage( cMessage *msg );

    /**
     * Processing of IPv4 datagrams. Called when a datagram reaches the front
     * of the queue.
     */
    virtual void endService(cPacket *packet);

    // NetFilter functions:
  protected:
    /**
     * called before a packet arriving from the network is routed
     */
    IHook::Result datagramPreRoutingHook(IPv4Datagram* datagram, const InterfaceEntry* inIE, const InterfaceEntry*& outIE, IPv4Address& nextHopAddr);

    /**
     * called before a packet arriving from the network is delivered via the network
     */
    IHook::Result datagramForwardHook(IPv4Datagram* datagram, const InterfaceEntry* inIE, const InterfaceEntry*& outIE, IPv4Address& nextHopAddr);

    /**
     * called before a packet is delivered via the network
     */
    IHook::Result datagramPostRoutingHook(IPv4Datagram* datagram, const InterfaceEntry* inIE, const InterfaceEntry*& outIE, IPv4Address& nextHopAddr);

    /**
     * called before a packet arriving from the network is delivered locally
     */
    IHook::Result datagramLocalInHook(IPv4Datagram* datagram, const InterfaceEntry* inIE);

    /**
     * called before a packet arriving locally is delivered
     */
    IHook::Result datagramLocalOutHook(IPv4Datagram* datagram, const InterfaceEntry*& outIE, IPv4Address& nextHopAddr);
   
    const IPv4RouteRule * checkInputRule(const IPv4Datagram*);
    const IPv4RouteRule * checkOutputRule(const IPv4Datagram*, const InterfaceEntry*);
    const IPv4RouteRule * checkOutputRuleMulticast(const IPv4Datagram*);

  public:
    /**
     * registers a Hook to be executed during datagram processing
     */
    virtual void registerHook(int priority, IHook* hook);

    /**
     * unregisters a Hook to be executed during datagram processing
     */
    virtual void unregisterHook(int priority, IHook* hook);

    /**
     * drop a previously queued datagram
     */
    void dropQueuedDatagram(const IPv4Datagram * datagram);

    /**
     * re-injects a previously queued datagram
     */
    void reinjectQueuedDatagram(const IPv4Datagram * datagram);

    /**
     * send packet on transportOut gate specified by protocolId
     */
    void sendOnTransPortOutGateByProtocolId(cPacket *packet, int protocolId);

    /**
     * ILifecycle method
     */
    virtual bool handleOperationStage(LifecycleOperation *operation, int stage, IDoneCallback *doneCallback);

    /// cListener method
    virtual void receiveSignal(cComponent *source, simsignal_t signalID, cObject *obj);

  protected:
    virtual bool isNodeUp();
    virtual void stop();
    virtual void start();
    virtual void flush();
};

std::ostream &operator<<( std::ostream &os, const C2WIPv4::ReplayBufferSP &replayBufferSP );

std::ostream &operator<<( std::ostream &os, const C2WIPv4::DatagramDataSP &datagramDataSP );

#endif

