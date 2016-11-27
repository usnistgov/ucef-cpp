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

#include "SynchronizedFederate.hpp"


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

C2WLogger* SynchronizedFederate::_logger = &C2W_FED_LOGGER_CLS::get_singleton();

const std::string SynchronizedFederate::FEDERATION_MANAGER_NAME( "manager" );

const std::string SynchronizedFederate::ReadyToPopulateSynch( "readyToPopulate" );
const std::string SynchronizedFederate::ReadyToRunSynch( "readyToRun" );
const std::string SynchronizedFederate::ReadyToResignSynch( "readyToResign" );


const double SynchronizedFederate::_stepSize = 0.2;

void SynchronizedFederate::createRTI( void ) {

	bool rtiNotPresent = true;
	
	if ( SynchronizedFederate::FEDERATION_MANAGER_NAME.compare( getFederateId() ) != 0 ) {
        // Himanshu: This is a regular federate, wait 20 seconds for federation manager to initialize first
	    std::cout << "Regular federate waiting 20 secs for Federation Manager to initialize" << std:: endl << std::flush;
#ifdef _WIN32
            Sleep( 20000 );
#else
            usleep( 20000000 );
#endif
	}
	while( rtiNotPresent ) {
		try {
			std::cout << "acquiring connection to RTI ... " << std::flush;
			_rti = new RTI::RTIambassador();
			std::cout << "done." << std::endl;

			rtiNotPresent = false;
		} catch ( RTI::RTIinternalError ) {
#ifdef _WIN32
			Sleep( 500 );
#else
			usleep( 500000 );
#endif
		}
	}

}

void SynchronizedFederate::joinFederation( const std::string &federation_id, const std::string &federate_id, bool ignoreLockFile ) {

    std::cout << "[" << federate_id << "] federate joining federation [" << federation_id << "] ... " << std::flush;

	_federateId = federate_id;
	_federationId = federation_id;
	

	bool federationNotPresent = true;
	while( federationNotPresent ) {
		try {


		    if(!ignoreLockFile) {
                int descriptor;
                int counter = 0;
                while(   (  descriptor = open( _lockFileName.c_str(), O_RDONLY | O_CREAT | O_EXCL, 0777 )  )  <  0   ) {
                    if ( errno == EEXIST || errno == EBUSY ) {
                        if ( counter++ >= 60 ) {
                            std::cerr << "ERROR: [" << federate_id << "] federate:  could not open lock file \"" << _lockFileName << "\": timeout after 60 seconds.  Exiting." << std::endl;
                            exit(1);
                        }
                        std::cout << "Waiting for federation to be created.." << std::endl;
                        usleep( 1000000 );
                    } else {
                        std::cerr << "ERROR: [" << federate_id << "] federate:  could not open lock file \"" << _lockFileName << "\": " << sys_errlist[ errno ] << ".  Exiting." << std::endl;
                        exit(1);
                    }
                }
                close( descriptor );
		    }

		    getRTI()->joinFederationExecution( federate_id.c_str(), federation_id.c_str(), this );

		    if(!ignoreLockFile) {
		        remove( _lockFileName.c_str() );
		    }

			federationNotPresent = false;
		} catch ( RTI::FederateAlreadyExecutionMember & ) {
			std::cout << "failed (already execution member)." << std::endl;
			return;
		} catch ( ... ) {
#ifdef _WIN32
			Sleep( 500 );
#else
			usleep( 500000 );
#endif
		}
	}

	// IT APPEARS THERE IS A RACE CONDITION THAT OCCURS BETWEEN A SUCCESSFUL CALL TO "joinFederationExecution" AND "enableTimeConstrained"
	// FOR THE RTI, AND, GENERALLY, THE LATTER IS CALLED IMMEDIATELY AFTER THE FORMER.
	// IF THE LATTER IS CALLED TOO QUICKLY AFTER THE FORMER, A SEGFAULT OCCURS IN THE JVM'S JNI DUE TO A NULL POINTER.
	// THE BELOW "WAITING PERIOD" SEEMS TO WORK-AROUND THIS PROBLEM.
#ifdef _WIN32
	Sleep( 3000 );
#else
	usleep( 3000000 );
#endif

	std::cout << "done." << std::endl;

}

void SynchronizedFederate::enableTimeConstrained( void ) throw( RTI::FederateNotExecutionMember ){
	if ( !_timeConstrainedNotEnabled ) return;

	bool timeConstrainedEnabledNotCalled = true;
	while( timeConstrainedEnabledNotCalled ) {
		try {
			getRTI()->enableTimeConstrained();
			timeConstrainedEnabledNotCalled = false;
		} catch ( RTI::TimeConstrainedAlreadyEnabled & ) {
			return;
		} catch ( RTI::EnableTimeConstrainedPending & ) {
			timeConstrainedEnabledNotCalled = false;
		} catch ( RTI::FederateNotExecutionMember &f ) {
			throw f;
		} catch ( ... ) {
#ifdef _WIN32
			Sleep( 500 );
#else
			usleep( 500000 );
#endif
		}
	}

	try {
		getRTI()->tick();
	} catch( ... ) { }
	while( _timeConstrainedNotEnabled ) {
#ifdef _WIN32
			Sleep( 500 );
#else
			usleep( 500000 );
#endif
		try {
			getRTI()->tick();
		} catch( ... ) { }
	}
}

void SynchronizedFederate::enableTimeRegulation( double time, double lookahead )
	throw( RTI::InvalidFederationTime, RTI::InvalidLookahead, RTI::FederateNotExecutionMember ) {

	if ( !_timeRegulationNotEnabled ) return;

	bool timeRegulationEnabledNotCalled = true;
	while( timeRegulationEnabledNotCalled ) {
		try {
			getRTI()->enableTimeRegulation(  RTIfedTime( time ), RTIfedTime( lookahead )  );
			timeRegulationEnabledNotCalled = false;
		} catch ( RTI::TimeRegulationAlreadyEnabled & ) {
			return;
		} catch ( RTI::EnableTimeRegulationPending & ) {
			timeRegulationEnabledNotCalled = false;
		} catch ( RTI::FederateNotExecutionMember &f ) {
			throw f;
		} catch ( RTI::InvalidFederationTime &i ) {
			throw i;
		} catch ( RTI::InvalidLookahead &i ) {
			throw i;
		} catch ( ... ) {
#ifdef _WIN32
			Sleep( 500 );
#else
			usleep( 500000 );
#endif
		}
	}

	try { getRTI()->tick(); } catch( ... ) { }
	while( _timeRegulationNotEnabled ) {
#ifdef _WIN32
			Sleep( 500 );
#else
			usleep( 500000 );
#endif
		try { getRTI()->tick(); } catch( ... ) { }
	}


	setLookahead( lookahead );
}

void SynchronizedFederate::resignFederationExecution( RTI::ResignAction resignAction ) {
    bool federationNotResigned = true;
    int Counter=10;
    while( federationNotResigned ) {
        try {
            getRTI()->resignFederationExecution( resignAction );
            federationNotResigned = false;
		} catch ( RTI::InvalidResignAction &i ) {
			std::cerr << "WARNING:  Invalid resign action when attempting to resign federation.  Changing resign action to DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES." << std::endl;
			resignAction = RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES;
		} catch ( RTI::FederateNotExecutionMember f ) {
            std::cerr << "WARNING:  While resigning federation:  federate not execution member." << std::endl;
            return;
		} catch ( RTI::FederateOwnsAttributes f ) {
            std::cerr << "WARNING:  While resigning federation:  federate owns attributes.  Releasing attributes." << std::endl;
			resignAction = static_cast< RTI::ResignAction >(  static_cast< int >( resignAction ) | static_cast< int >( RTI::RELEASE_ATTRIBUTES )  );
        } catch ( ... ) {
            std::cerr << "WARNING:  problem encountered while resigning federation execution:  retry" << std::endl;

			if( Counter-- < 1){
			    std::cerr << "Resigned Failed. Exiting from the Federation" << std::endl;
                federationNotResigned = false;
			}

#ifdef _WIN32
			Sleep( 500 );
#else
			usleep( 500000 );
#endif

        }
    }
}

/**
 * Returns the current time for this federate.
 *
 * @return the current time for this federate
 */
double SynchronizedFederate::getCurrentTime( void ) {
	RTIfedTime fedTime;

	bool timeNotAcquired = true;
	while( timeNotAcquired ) {
		try {
			getRTI()->queryFederateTime( fedTime );
			timeNotAcquired = false;
		} catch ( RTI::FederateNotExecutionMember & ) {
			std::cerr << "SynchronizedFederate:  getCurrentTime:  ERROR:  Federate not execution member" << std::endl;
		    return -1;
		} catch ( ... ) {
			std::cerr << "SynchronizedFederate:  getCurrentTime:  Exception caught:  " << std::endl;
			return -1;
		}
	}

	return fedTime.getTime();
}

/**
 * Returns the current Lower Bound on Time-Stamps (LBTS) for this federate.
 *
 * @return the current time for this federate
 */
double SynchronizedFederate::getLBTS( void ) {
	RTIfedTime lbtsTime;

	bool timeNotAcquired = true;
	while( timeNotAcquired ) {
		try {
			getRTI()->queryLBTS( lbtsTime );
			timeNotAcquired = false;
		} catch ( RTI::FederateNotExecutionMember & ) {
			std::cerr << "SynchronizedFederate:  getLBTS:  ERROR:  Federate not execution member" << std::endl;
		    return -1;
		} catch ( ... ) {
			std::cerr << "SynchronizedFederate:  getLBTS:  Exception caught:  " << std::endl;
			return -1;
		}
	}

	return lbtsTime.getTime();
}

/**
 * When sending an interaction in timestamp order, we should use
 * currentTime+Lookahead or LBTS whichever is greater. Current Portico RTI
 * implementation dictates to use a timestamp which is greater than or
 * equal to federate's LBTS.
 *
 * @return the timestamp to use for outgoing TSO interactions
 */
double SynchronizedFederate::getMinTSOTimestamp( void ) {
	RTIfedTime lbtsTime;
	RTIfedTime fedTime;

	bool timeNotAcquired = true;
	while( timeNotAcquired ) {
		try {
			getRTI()->queryFederateTime( fedTime );
			getRTI()->queryLBTS( lbtsTime );
			timeNotAcquired = false;
		} catch ( RTI::FederateNotExecutionMember & ) {
			std::cerr << "SynchronizedFederate:  getMinTSOTimestamp:  ERROR:  Federate not execution member" << std::endl;
		    return -1;
		} catch ( ... ) {
			std::cerr << "SynchronizedFederate:  getMinTSOTimestamp:  Exception caught:  " << std::endl;
			return -1;
		}
	}

	fedTime += getLookahead();

	if(fedTime >= lbtsTime)
		return fedTime.getTime();
	else
		return lbtsTime.getTime();
}

void SynchronizedFederate::achieveSynchronizationPoint( const std::string &label )
		throw( RTI::FederateNotExecutionMember, RTI::RTIinternalError ) {
	bool synchronizationPointNotAccepted = true;
	while( synchronizationPointNotAccepted ) {
		try {
			getRTI()->synchronizationPointAchieved( label.c_str() );
			while(  _achievedSynchronizationPoints.find( label ) == _achievedSynchronizationPoints.end()  ) {
#ifdef _WIN32
				Sleep( 500 );
#else
				usleep( 500000 );
#endif
				getRTI()->tick();
			}
			synchronizationPointNotAccepted = false;
		} catch ( RTI::FederateNotExecutionMember &f ) {
			throw f;
		} catch ( RTI::SynchronizationPointLabelWasNotAnnounced & ) {
			if (  _achievedSynchronizationPoints.find( label ) != _achievedSynchronizationPoints.end()  ) {
				synchronizationPointNotAccepted = false;
			} else {
				try {
					getRTI()->tick();
				} catch ( RTI::RTIinternalError &r ) {
					throw r;
				} catch ( ... ) {
#ifdef _WIN32
					Sleep( 500 );
#else
					usleep( 500000 );
#endif
				}
			}
		} catch( ... ) {
#ifdef _WIN32
			Sleep( 500 );
#else
			usleep( 500000 );
#endif
		}
	}
}

void SynchronizedFederate::run( void ) {

	_currentTime = getCurrentTime();
	if ( _currentTime < 0 ) return;

	double timeRequest = -1;
	while( true ) {
		AdvanceTimeRequest advanceTimeRequest = takeNextAdvanceTimeRequest();
		if ( advanceTimeRequest.isNull() ) break;  // NO MORE ADVANCE TIME REQUESTS (REMEMBER, WE'RE SINGLE THREADED)

		timeRequest = advanceTimeRequest.getRequestedTime();

		advanceTime( timeRequest );

		advanceTimeRequest.getATRCallback().execute();
	}

	noMoreATRs();

	while( getNextInteraction() != 0 ); // EMPTY THE QUEUE

	// WAIT FOR SIMEND (?)
	while( true ) {
		timeRequest += _stepSize;
		advanceTime( timeRequest );
	}
}

void SynchronizedFederate::advanceTime( double time ) {

	if ( getTimeAdvanceMode() != SF_TIME_ADVANCE_REQUEST_AVAILABLE && getTimeAdvanceMode() != SF_NEXT_EVENT_REQUEST_AVAILABLE &&  time <= _currentTime ) return;

	setTimeAdvanceNotGranted( true );

	bool tarNotCalled = true;
	while( tarNotCalled ) {
		try {
		    if ( getTimeAdvanceMode() == SF_TIME_ADVANCE_REQUEST ) {
		        getRTI()->timeAdvanceRequest(  RTIfedTime( time )  );
		    }
		    else if ( getTimeAdvanceMode() == SF_TIME_ADVANCE_REQUEST_AVAILABLE ) {
                getRTI()->timeAdvanceRequestAvailable(  RTIfedTime( time )  );
		    }
		    else if ( getTimeAdvanceMode() == SF_NEXT_EVENT_REQUEST ) {
                getRTI()->nextEventRequest(  RTIfedTime( time )  );
		    }
		    else if ( getTimeAdvanceMode() == SF_NEXT_EVENT_REQUEST_AVAILABLE ) {
                getRTI()->nextEventRequestAvailable(  RTIfedTime( time )  );
		    }
			tarNotCalled = false;
		} catch ( RTI::FederationTimeAlreadyPassed &f ) {
			std::cerr << "Time already passed detected." << std::endl;
			setTimeAdvanceNotGranted( false );
			tarNotCalled = false;
		} catch ( ... ) { }
	}

	while( getTimeAdvanceNotGranted() ) {
		try { getRTI()->tick(); } catch ( ... ) { }
#ifdef _WIN32
		Sleep( 10 );
#else
		usleep( 10000 );
#endif
	}

	_currentTime = time;
}

void SynchronizedFederate::createLog(
		RTI::ObjectHandle theObject,
		const RTI::AttributeHandleValuePairSet& theAttributes,
		double time)
{
	if(ObjectRoot::getSubAttributeLogMap().empty()) return;
	std::string objectName = ObjectRoot::getObject( theObject )->getClassName() ;
	for(RTI::ULong i=0; i<theAttributes.size(); ++i)
	{
		std::string attribute = ObjectRoot::get_attribute_name(theAttributes.getHandle(i));
		std::map<std::string, std::string>::iterator pos = ObjectRoot::getSubAttributeLogMap().find(attribute);
		if(pos == ObjectRoot::getSubAttributeLogMap().end()) continue;
		std::string loglevel = (*pos).second;
		std::string id = objectName+"_"+attribute+"_sub_"+_federateId;
		static RTI::ULong valueLength;
		char* value = theAttributes.getValuePointer( i, valueLength);
		std::string ptype="";
		ObjectRoot::DatamemberTypeMap::iterator it = ObjectRoot::getDatamemberTypeMap().find( attribute );
		if ( it != ObjectRoot::getDatamemberTypeMap().end() )
			ptype = (*it).second;
		_logger->addLog(id,attribute,std::string(value, valueLength),ptype,time,loglevel);
	}
}
