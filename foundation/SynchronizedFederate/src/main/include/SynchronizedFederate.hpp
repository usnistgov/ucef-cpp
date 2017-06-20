/*
 * Copyright (c) 2008, Institute for Software Integrated Systems, Vanderbilt University
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the author appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE VANDERBILT UNIVERSITY BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE VANDERBILT
 * UNIVERSITY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE VANDERBILT UNIVERSITY SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE VANDERBILT UNIVERSITY HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * @author Harmon Nine
 */

#ifndef _SYNCHRONIZED_FEDERATE_HPP
#define _SYNCHRONIZED_FEDERATE_HPP

#ifndef RTI_USES_STD_FSTREAM
#define RTI_USES_STD_FSTREAM
#endif

#include <string>
#include <set>
#include <map>

#include "NullFederateAmbassador.hh"
#include "RTI.hh"
#include "fedtime.hh"
#include "InteractionRoot.hpp"
#include "ObjectRoot.hpp"
#include "SimEnd.hpp"

#include "FederateLogger.hpp"

#ifndef C2W_FED_LOGGER_CLS
#define C2W_FED_LOGGER_CLS C2WConsoleLogger
#endif 

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif



class SynchronizedFederate : public NullFederateAmbassador {

public:
	static const std::string FEDERATION_MANAGER_NAME;
	enum TimeAdvanceMode { SF_TIME_ADVANCE_REQUEST, SF_NEXT_EVENT_REQUEST, SF_TIME_ADVANCE_REQUEST_AVAILABLE, SF_NEXT_EVENT_REQUEST_AVAILABLE };


private:
    RTI::RTIambassador *_rti;

	std::string _federateId;
	std::string _federationId;
	std::string _lockFileName;


public:
	static const std::string ReadyToPopulateSynch;
	static const std::string ReadyToRunSynch;
	static const std::string ReadyToResignSynch;

private:
	typedef std::set< std::string > StringSet;
	StringSet _achievedSynchronizationPoints;

	bool _timeConstrainedNotEnabled;
	bool _timeRegulationNotEnabled;
	bool _timeAdvanceNotGranted;
	bool _simEndNotSubscribed;

	double _currentTime;
	double _lookahead;
	TimeAdvanceMode _timeAdvanceMode;

public:
	void setLookahead( double lookahead ) { _lookahead = lookahead; }
	double getLookahead() { return _lookahead; }
	void setTimeAdvanceMode ( TimeAdvanceMode timeAdvanceMode ) { _timeAdvanceMode = timeAdvanceMode; }
	TimeAdvanceMode getTimeAdvanceMode() { return _timeAdvanceMode; }

private:
	bool getTimeAdvanceNotGranted( void ) { return _timeAdvanceNotGranted; }
	void setTimeAdvanceNotGranted( bool timeAdvanceNotGranted ) { _timeAdvanceNotGranted = timeAdvanceNotGranted; }

protected:
	static C2WLogger* _logger;

	SynchronizedFederate( void ) : _federateId( "" ), _federationId( "" ), _timeConstrainedNotEnabled( true ), _timeRegulationNotEnabled( true ), _simEndNotSubscribed( true ), _currentTime( 0 ), _lookahead( 0 )
	{
		  setpgid( 0, 0 );
		  _lockFileName = getenv( "EXEDIR" );
		  if ( !_lockFileName.empty() ) {
		      _lockFileName += "/";
		  }
		  _lockFileName += "__lock__";

		  _timeAdvanceMode = SF_TIME_ADVANCE_REQUEST;
	}

	virtual ~SynchronizedFederate()
	 throw (RTI::FederateInternalError) { destroyRTI(); }

	RTI::RTIambassador *getRTI( void ) { return _rti; }

	void createRTI( void );

	void destroyRTI( void ) {
		// delete _rti;
		_rti = 0;
	}

	void joinFederation( const std::string &federation_id, const std::string &federate_id, bool ignoreLockFile = true );
	
	std::string getFederateId( void ) const { return _federateId; }
	std::string getFederationId( void ) const { return _federationId; }
	std::string getFederationManagerName( void ) const { return SynchronizedFederate::FEDERATION_MANAGER_NAME; }

	void enableTimeConstrained( void ) throw( RTI::FederateNotExecutionMember );
	void enableTimeRegulation( double time, double lookahead )
	 throw( RTI::InvalidFederationTime, RTI::InvalidLookahead, RTI::FederateNotExecutionMember );
	void enableTimeRegulation( double lookahead )
	 throw( RTI::InvalidFederationTime, RTI::InvalidLookahead, RTI::FederateNotExecutionMember ) {
		enableTimeRegulation( 0, lookahead );
	}

	void resignFederationExecution( RTI::ResignAction resignAction );
	void resignFederationExecution( void ) {
		resignFederationExecution( RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES );
	}

	void enableAsynchronousDelivery( void ) { getRTI()->enableAsynchronousDelivery(); }

	void ensureSimEndSubscription( void ) {
		if ( _simEndNotSubscribed ) {
			// Auto-subscribing also ensures that there is no filter set for SimEnd
			SimEnd::subscribe( getRTI() );
			_simEndNotSubscribed = false;
		}
	}

	void readyToPopulate( void ) throw( RTI::FederateNotExecutionMember, RTI::RTIinternalError ) {
		ensureSimEndSubscription();
    	achieveSynchronizationPoint( ReadyToPopulateSynch );
    }
    void readyToRun( void ) throw( RTI::FederateNotExecutionMember, RTI::RTIinternalError ) {
    	achieveSynchronizationPoint( ReadyToRunSynch );
    }
    void readyToResign( void ) throw( RTI::FederateNotExecutionMember, RTI::RTIinternalError ) {
    	achieveSynchronizationPoint( ReadyToResignSynch );
    }
	void achieveSynchronizationPoint( const std::string &label )
	 throw( RTI::FederateNotExecutionMember, RTI::RTIinternalError );

public:
    double getCurrentTime( void );
    double getLBTS( void );
    double getMinTSOTimestamp( void );
    double getCurrentTimeLocal( void ) { return _currentTime; }

    class ATRCallback {
    public:
		typedef boost::shared_ptr< ATRCallback > SP;
    	virtual void execute( void ) = 0;
		virtual SP clone( void ) = 0;
    };

	//template< typename T >
	//class ATRCallback : public ATRCallbackBase {
	//private:
	//	T _executor;
	//public:
	//	ATRCallback( const T &executor) : _executor( executor ) { }
	//	void execute( void ) {
	//
	//	}
	//};

private:
	typedef std::multimap< double, ATRCallback::SP > ATRQueue;
	static ATRQueue &getATRQueue( void ) {
		static ATRQueue atrQueue;
		return atrQueue;
	}

	static const double _stepSize;

	static void noMoreATRs( void ) { getMoreATRs() = false; }

	class AdvanceTimeRequest {
	private:
		double _requestedTime;
		ATRCallback::SP _atrCallbackSP;

	public:
		AdvanceTimeRequest( void ) :
		 _requestedTime( -1 ), _atrCallbackSP( (ATRCallback *)0 ) { }
		AdvanceTimeRequest( ATRQueue::value_type init ) :
		 _requestedTime( init.first ), _atrCallbackSP( init.second ) { }
		AdvanceTimeRequest( double requestedTime, ATRCallback &atrCallback ) :
		 _requestedTime( requestedTime ), _atrCallbackSP( atrCallback.clone() ) { }

		bool isNull( void ) { return _atrCallbackSP == 0; }
		void setNull( void ) { _atrCallbackSP = ATRCallback::SP( (ATRCallback *)0 ); }
		double getRequestedTime( void ) { return _requestedTime; }
		ATRCallback &getATRCallback( void ) { return *_atrCallbackSP; }

		operator ATRQueue::value_type( void ) {
			return std::make_pair( _requestedTime, _atrCallbackSP );
		}
	};

public:
	static bool &getMoreATRs( void ) {
		static bool moreATRs = true;
		return moreATRs;
	}

	static void putAdvanceTimeRequest( double time, ATRCallback &atrCallback ) {
		getATRQueue().insert(  std::make_pair( time, atrCallback.clone() )  );
	}

private:
	static AdvanceTimeRequest takeNextAdvanceTimeRequest( void ) {
		if ( getATRQueue().empty() ) return AdvanceTimeRequest();
		AdvanceTimeRequest advanceTimeRequest( *getATRQueue().begin() );
		getATRQueue().erase( getATRQueue().begin() );
		return advanceTimeRequest;
	}

public:
	virtual void timeConstrainedEnabled( const RTI::FedTime &t )
	 throw( RTI::InvalidFederationTime, RTI::EnableTimeConstrainedWasNotPending, RTI::FederateInternalError ) { _timeConstrainedNotEnabled = false; }
	virtual void timeRegulationEnabled( const RTI::FedTime &t )
	 throw ( RTI::InvalidFederationTime, RTI::EnableTimeRegulationWasNotPending, RTI::FederateInternalError ) { _timeRegulationNotEnabled = false; }
	virtual void timeAdvanceGrant( const RTI::FedTime &fedTime )
 	 throw( RTI::InvalidFederationTime, RTI::TimeAdvanceWasNotInProgress, RTI::FederateInternalError ) { _timeAdvanceNotGranted = false; }
    virtual void federationSynchronized( const char *label )
	 throw( RTI::FederateInternalError ) { _achievedSynchronizationPoints.insert( label ); }

private:
	class InteractionRootSPComparator {
	public:
		bool operator()( const InteractionRoot::SP &interactionRootSP1, const InteractionRoot::SP &interactionRootSP2 ) {
			if ( interactionRootSP1->getTime() < interactionRootSP2->getTime() ) {
			    return true;
			} else if ( interactionRootSP1->getTime() > interactionRootSP2->getTime() ) {
			    return false;
			} else {
			    boost::shared_ptr< C2WInteractionRoot > c2wInteractionRootSP1(   boost::static_pointer_cast< C2WInteractionRoot >( interactionRootSP1 )  );
			    boost::shared_ptr< C2WInteractionRoot > c2wInteractionRootSP2(   boost::static_pointer_cast< C2WInteractionRoot >( interactionRootSP2 )  );
			    if ( c2wInteractionRootSP1 != 0 && c2wInteractionRootSP2 != 0 ) {
			        if ( c2wInteractionRootSP1->get_actualLogicalGenerationTime() < c2wInteractionRootSP2->get_actualLogicalGenerationTime() ) {
			            return true;
			        } else if ( c2wInteractionRootSP2->get_actualLogicalGenerationTime() < c2wInteractionRootSP1->get_actualLogicalGenerationTime() ) {
			            return false;
			        }
			    }
			}

			return false;
		}
	};

	typedef std::multiset< InteractionRoot::SP, InteractionRootSPComparator > InteractionQueue;
	static InteractionQueue &getInteractionQueue( void ) {
		static InteractionQueue interactionQueue;
		return interactionQueue;
	}

public:
	static void addInteraction( InteractionRoot::SP interactionRootSP ) {
	    getInteractionQueue().insert( interactionRootSP );
	}

	static InteractionRoot::SP getNextInteraction( void ) {
		if ( getInteractionQueue().empty() ) return InteractionRoot::SP( (InteractionRoot*)0 );

		InteractionQueue::iterator inqItr = getInteractionQueue().begin();
		InteractionRoot::SP interactionRootSP = *inqItr;
		getInteractionQueue().erase( inqItr );
	    return interactionRootSP;
	}

	bool unmatchingFedFilterProvided( InteractionRoot::SP interactionRootSP ) {
	    boost::shared_ptr< C2WInteractionRoot > c2wInteractionRootSP(   boost::static_pointer_cast< C2WInteractionRoot >( interactionRootSP )  );
	    std::string fedFilter = c2wInteractionRootSP->get_federateFilter();
	    if( fedFilter.length() > 0 ) {
            boost::algorithm::trim(fedFilter);
            if( ( fedFilter.length() > 0 ) && ( fedFilter.compare( getFederateId() ) != 0 ) ) {
                // std::cout << "Filtering due to fed filter: " << fedFilter << std::endl;
                // std::cout << "Filtered interaction was: " << interactionRootSP << std::endl;
                return true;
            }
	    }
	    return false;
	}

	virtual void receiveInteraction(
	 RTI::InteractionClassHandle theInteraction,
	 const RTI::ParameterHandleValuePairSet& theParameters,
	 const RTI::FedTime& theTime,
     const char *theTag,
	 RTI::EventRetractionHandle theHandle
	)
	 throw ( RTI::InteractionClassNotKnown, RTI::InteractionParameterNotKnown, RTI::InvalidFederationTime, RTI::FederateInternalError) {
		if ( getMoreATRs() ) {
			InteractionRoot::SP interactionRootSP = InteractionRoot::create_interaction( theInteraction, theParameters, theTime );
			if ( !unmatchingFedFilterProvided(interactionRootSP) ) {
                RTIfedTime rtitime(theTime);
                double ltime = rtitime.getTime();
                handleIfSimEnd(interactionRootSP, ltime);
                addInteraction( interactionRootSP );
                interactionRootSP->createLog( ltime, false );
			}
		}
	}

	virtual void receiveInteraction(
	 RTI::InteractionClassHandle theInteraction,
	 const RTI::ParameterHandleValuePairSet& theParameters,
     const char *theTag
	)
	 throw ( RTI::InteractionClassNotKnown, RTI::InteractionParameterNotKnown, RTI::FederateInternalError) {
		if ( getMoreATRs() ) {
			InteractionRoot::SP interactionRootSP = InteractionRoot::create_interaction( theInteraction, theParameters );
            if ( !unmatchingFedFilterProvided(interactionRootSP) ) {

                // Himanshu: We normally use only TSO updates, so this shouldn't be
                // called, but due to an RTI bug, it seemingly is getting called. So,
                // for now, use the federate's current time or LBTS whichever is greater
                // as the timestamp
                double assumedTimestamp = 0;
                if( getLBTS() >= getCurrentTime() ) {
                    assumedTimestamp = getLBTS();
                } else {
                    assumedTimestamp = getCurrentTime();
                }
                RTIfedTime rtitime( assumedTimestamp );
                handleIfSimEnd(interactionRootSP, assumedTimestamp);
                addInteraction( interactionRootSP );
                interactionRootSP->createLog( assumedTimestamp, false );
            }
		}
	}

protected:

	void finalizeAndTerminate() {
	        std::cout << getFederateId() << ": resigning from federation...";
            resignFederationExecution();
            std::cout << "done." << std::endl;

            // Wait for 10 seconds for Federation Manager to recognize that the federate has resigned.
#ifdef _WIN32
            Sleep( 10000 );
#else
            usleep( 10000000 );
#endif

            // TODO: Kill the entire process group (like in SynchronizedFederate.java)

            // Exit
            exit(0);
	}

	void handleIfSimEnd(InteractionRoot::SP interactionRootSP, double timestamp) {
		int classHandle = interactionRootSP->getClassHandle();
		if(  SimEnd::match( classHandle )  ) {
			std::cout << getFederateId() << ": SimEnd interaction received, exiting..." << std::endl;
            interactionRootSP->createLog( timestamp, false );
            finalizeAndTerminate();
		}
	}

public:
	void run( void );
	void advanceTime( double time );
	void advanceTimeStep( double timeStep ) {
		advanceTime( _currentTime + timeStep );
	}

	void createLog(
		RTI::InteractionClassHandle theInteraction,
		const RTI::ParameterHandleValuePairSet& theParameters,
		double time=0);

	class ObjectReflector {
	private:
		RTI::ObjectHandle _objectHandle;
		RTI::AttributeHandleValuePairSet *_theAttributes;
		double _time;

		void init( const RTI::AttributeHandleValuePairSet &theAttributes ) {
			for(RTI::ULong ix = 0 ; ix < theAttributes.size() ; (void)++ix ) {
				RTI::ULong valueLength = theAttributes.getValueLength( ix );
				_theAttributes->add( theAttributes.getHandle( ix ), theAttributes.getValuePointer( ix, valueLength ), theAttributes.getValueLength( ix )  );
			}
		}

	public:
		ObjectReflector( void ) : _theAttributes( (RTI::AttributeHandleValuePairSet *)0 ) { }

		ObjectReflector(
		 RTI::ObjectHandle objectHandle,
		 const RTI::AttributeHandleValuePairSet& theAttributes
		) : _objectHandle( objectHandle ), _theAttributes(  RTI::AttributeSetFactory::create( theAttributes.size() )  ), _time( -1 ) {
			init( theAttributes );
		}

		ObjectReflector(
		 RTI::ObjectHandle objectHandle,
		 const RTI::AttributeHandleValuePairSet& theAttributes,
		 double time
		) : _objectHandle( objectHandle ), _theAttributes(  RTI::AttributeSetFactory::create( theAttributes.size() )  ), _time( time ) {
			init( theAttributes );
		}

		ObjectReflector(
		 RTI::ObjectHandle objectHandle,
		 const RTI::AttributeHandleValuePairSet& theAttributes,
		 const RTI::FedTime &fedTime
		) : _objectHandle( objectHandle ), _theAttributes(  RTI::AttributeSetFactory::create( theAttributes.size() )  ), _time(  RTIfedTime( fedTime ).getTime()  ) {
			init( theAttributes );
		}

		void reflect() const {
			if ( _theAttributes == 0 ) {
				std::cerr << "WARNING:  attempt to reflect null ObjectReflector" << std::endl;
				return;
			}
			if ( _time < 0 ) ObjectRoot::reflect( _objectHandle, *_theAttributes );
			else  ObjectRoot::reflect( _objectHandle, *_theAttributes, _time );
        }

		ObjectRoot::SP getObjectRootSP() const { return ObjectRoot::getObject( _objectHandle ); }
        double getTime() const { return _time; }

		bool isNull( void ) const { return _theAttributes == 0; }
	};

private:
	class ObjectReflectorComparator {
	public:
		bool operator()( const ObjectReflector &objectReflector1, const ObjectReflector &objectReflector2 ) {
			return objectReflector1.getTime() < objectReflector2.getTime();
		}
	};

	typedef std::multiset< ObjectReflector, ObjectReflectorComparator > ObjectReflectorQueue;
	static ObjectReflectorQueue &getObjectReflectorQueue( void ) {
		static ObjectReflectorQueue objectReflectorQueue;
		return objectReflectorQueue;
	}

public:
	static void addObjectReflector( const ObjectReflector &objectReflector ) {
		getObjectReflectorQueue().insert( objectReflector );
	}

    static void addObjectReflector( int objectHandle, const RTI::AttributeHandleValuePairSet& theAttributes ) {
        addObjectReflector(  ObjectReflector( objectHandle, theAttributes )  );
    }
	static void addObjectReflector( int objectHandle, const RTI::AttributeHandleValuePairSet& theAttributes, const RTI::FedTime &theTime ) {
        addObjectReflector(  ObjectReflector( objectHandle, theAttributes, theTime )  );
    }

	static ObjectReflector getNextObjectReflector( void ) {
		if ( getObjectReflectorQueue().empty() ) return ObjectReflector();

		ObjectReflectorQueue::iterator orqItr = getObjectReflectorQueue().begin();
		ObjectReflector objectReflector = *orqItr;
	    getObjectReflectorQueue().erase( orqItr );
	    return objectReflector;
	}


	virtual void discoverObjectInstance( RTI::ObjectHandle theObject, RTI::ObjectClassHandle theObjectClass, const char *objectName )
	 throw ( RTI::CouldNotDiscover, RTI::ObjectClassNotKnown, RTI::FederateInternalError) {
		ObjectRoot::discover( theObjectClass, theObject );
	}

	virtual void reflectAttributeValues( RTI::ObjectHandle theObject, const RTI::AttributeHandleValuePairSet& theAttributes, const char *theTag )
	 throw ( RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::FederateOwnsAttributes, RTI::FederateInternalError ) {
		addObjectReflector( theObject, theAttributes );
		createLog(theObject, theAttributes);
	}

	virtual void reflectAttributeValues(
	 RTI::ObjectHandle theObject,
	 const RTI::AttributeHandleValuePairSet& theAttributes,
	 const RTI::FedTime& theTime,
     const char *theTag,
	 RTI::EventRetractionHandle theHandle
	)
	 throw ( RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::FederateOwnsAttributes, RTI::InvalidFederationTime, RTI::FederateInternalError ) {
		addObjectReflector( theObject, theAttributes, theTime );
		RTIfedTime rtitime(theTime);
		double ltime = rtitime.getTime();
		createLog(theObject, theAttributes, ltime);
	}

	void createLog(
		RTI::ObjectHandle theObject,
		const RTI::AttributeHandleValuePairSet& theAttributes,
		double time=0);
};

#endif
