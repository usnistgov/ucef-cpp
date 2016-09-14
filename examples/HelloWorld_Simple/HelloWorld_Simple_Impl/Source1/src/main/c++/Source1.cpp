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

#include "Source1.hpp"

void Source1::initialize( void ) {

    Source1ATRCallback src2ATRCb( *this );
    putAdvanceTimeRequest(  _currentTime, src2ATRCb );

    readyToPopulate();
    readyToRun();
}

void Source1::execute( void ) {
    Ping1SP pingSP = create_Ping1();
    pingSP->set_Count( -_ix );
    
    std::cout << "Source1: Sending Ping interaction #-" << _ix << std::endl;
    pingSP->sendInteraction( getRTI(), _currentTime + getLookAhead() );
    
    _currentTime += 1;

    Source1ATRCallback src2ATRCb( *this );
    putAdvanceTimeRequest(  _currentTime, src2ATRCb );
    
    ++_ix;
}


int main( int argc, char *argv[] ) {

	std::cout << "Creating Source1 object" << std::endl;
	Source1 Source1( argc, argv );
	std::cout << "Source1 created" << std::endl;

	std::cout << "Initializing Source1" << std::endl;
	Source1.initialize();
	std::cout << "Source1 initialized" << std::endl;

	std::cout << "Running Source1" << std::endl;
	Source1.run();

	return 0;
}
