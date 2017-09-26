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
 */

#include "Sink1.hpp"

void Sink1::initialize( void ) {

	Sink1ATRCallback sk2ATRCb( *this );
	// putAdvanceTimeRequest(  _currentTime, sk2ATRCb  );

    // readyToPopulate();
    // readyToRun();

_currentTime = 0;
    
    if (this->get_IsLateJoiner()) {
        _currentTime = getLBTS() - getLookAhead();
        disableTimeRegulation();
    }

    putAdvanceTimeRequest(  _currentTime, sk2ATRCb );

    if(!this->get_IsLateJoiner()){
        readyToPopulate();
        readyToRun();
    }

//	_pingCount.registerObject( getRTI() );
//	_pingCount.updateAttributeValues( getRTI(), _currentTime + getLookAhead() );

}

void Sink1::execute( void ) {

	static const double timeOrderOffsetIncrement = 0.00001;
	
    InteractionRoot::SP interactionRootSP;

    _currentTime += 1;

	double timeOrderOffset = 0;

	while(  ( interactionRootSP = getNextInteraction() ) != 0  ) {
		boost::shared_ptr< Ping1 > pingSP(   boost::static_pointer_cast< Ping1 >( interactionRootSP )  );
		std::cout << "Sink1: Received Ping interaction #" << pingSP->get_Count() << std::endl;
//		_pingCount.set_RunningCount( _pingCount.get_RunningCount() + 1 );
//		_pingCount.updateAttributeValues( getRTI(), _currentTime + getLookAhead() + timeOrderOffset );
		timeOrderOffset += timeOrderOffsetIncrement;
    }
    
	Sink1ATRCallback sk2ATRCb( *this );
	putAdvanceTimeRequest(  _currentTime, sk2ATRCb  );

}

int main( int argc, char *argv[] ) {

	// Sink1 Sink1( argc, argv );

    FederateConfigParser *parse_obj = new FederateConfigParser();
    FederateConfig *fedconfigObj = parse_obj->parseArgs(argc, argv);
    Sink1 sink1(fedconfigObj);
  

	sink1.initialize();
	sink1.run();


	return 0;
}
