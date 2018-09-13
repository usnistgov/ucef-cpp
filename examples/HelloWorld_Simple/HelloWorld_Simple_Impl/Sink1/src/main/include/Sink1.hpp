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

#ifndef _SINK1_CLASS
#define _SINK1_CLASS

#include "Sink1Base.hpp"
#include "FederateConfigParser.h"
#include "FederateConfig.h"

class Sink1 : public Sink1Base {
private:
    double _currentTime;
//	PingCount _pingCount;
    
public:
    typedef Sink1Base Super;
//     Sink1( int argc, char *argv[] ) : Super( argc, argv ), _currentTime( 0 ) {
// //		_pingCount.set_SinkName( "Sink1" );
// //		_pingCount.set_RunningCount( 0 );
// 	}
	Sink1(FederateConfig *fedconfigObj): Super(fedconfigObj) {
       
	}

	virtual ~Sink1( void )
	 throw (RTI::FederateInternalError) { }

    class Sink1ATRCallback : public ATRCallback {
    private:
    	Sink1 &_Sink1;
    public:
    	Sink1ATRCallback( Sink1 &Sink1 ) : _Sink1( Sink1 ) { }
    	virtual void execute( void ) { _Sink1.execute(); }
		virtual SP clone( void ) { return SP(  new Sink1ATRCallback( *this )  ); }
    };

    void initialize( void );
    void execute( void );
};

#endif
