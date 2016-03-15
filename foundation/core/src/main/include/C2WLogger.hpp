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
 * @author Feng Shi
 */

#ifndef _C2WLOGGER_HPP
#define _C2WLOGGER_HPP

#include <string>

#include "StringCollections.hpp"

#include "InteractionIf.hpp"
#include "ObjectIf.hpp"

class C2WLogger {

public:
	virtual void init( const ArgVector &argVector ) = 0;
	virtual void addLog(const std::string &interaction, const std::string &fed, bool publish=true) = 0;
	virtual void addLog(const std::string &object, const std::string &attribute, const std::string &fed, bool publish=true) = 0;
	virtual void addLog(const std::string &federate) = 0;
	virtual void addLog( const std::string &id, const std::string &parameter, const std::string &value, const std::string &ptype, double time = 0, const std::string &level = "" ) = 0;
	virtual void addLog( const std::string &id, const InteractionIf &interactionRoot, double time = 0, const std::string &level = "" ) = 0;
	virtual void addLog( const std::string &id, const ObjectIf &objectRoot, double time = 0, const std::string &level = "" ) = 0;
	virtual void addLog(const std::string &federate, const std::string &info, const std::string &level, double time) = 0;
	virtual int getLogLevel(const std::string &loglevel) = 0;
};

#endif //_C2WLOGGER
