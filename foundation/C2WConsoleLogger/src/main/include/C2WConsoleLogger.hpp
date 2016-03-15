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
 * @author Andras Nadas
 */

#ifndef _C2WCONSOLELOGGER_HPP
#define _C2WCONSOLELOGGER_HPP

#include <string>
#include <vector>

#include "C2WLogger.hpp"

class C2WConsoleLogger : public C2WLogger {

private:
	int _logId;

	C2WConsoleLogger( void ) : _logId( -1 ){}

public:
	
	static C2WConsoleLogger &get_singleton( void ) {
		static C2WConsoleLogger c2wLogger;
		return c2wLogger;
	}

	void init( const ArgVector &argVector );
	void addLog(const std::string &interaction, const std::string &fed, bool publish=true);
 	void addLog(const std::string &object, const std::string &attribute, const std::string &fed, bool publish=true);
	void addLog(const std::string &federate);
	void addLog( const std::string &id, const std::string &parameter, const std::string &value, const std::string &ptype, double time = 0, const std::string &level = "" );
	void addLog( const std::string &id, const InteractionIf &interactionRoot, double time = 0, const std::string &level = "" );
	void addLog( const std::string &id, const ObjectIf &objectRoot, double time = 0, const std::string &level = "" );
	void addLog(const std::string &federate, const std::string &info, const std::string &level, double time);
	int getLogLevel(const std::string &loglevel);
};

#endif //_C2WCONSOLELOGGER_HPP
