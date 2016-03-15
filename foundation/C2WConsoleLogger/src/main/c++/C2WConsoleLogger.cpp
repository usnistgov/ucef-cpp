/*
 * Copyright (c) 2016, Institute for Software Integrated Systems, Vanderbilt University
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
#include <boost/lexical_cast.hpp>

#include "C2WConsoleLogger.hpp"
#include "TypeMedley.hpp"

#include <iostream>
#include <stdio.h>

#if __cplusplus <= 199711L
#define TIME_LOG_FORMAT "%.3f"
#else
#define TIME_LOG_FORMAT "%.3lf"
#endif

void C2WConsoleLogger::init( const ArgVector &argVector ){
	//No initialization needed for the console
}

void C2WConsoleLogger::addLog( const std::string &interaction, const std::string &fed, bool publish ) {

	std::string values;

	if ( publish ) {
		values = "'" + fed+"_pub_"+interaction + "', '" + interaction + "', '', '', '" + fed + "', ''";
	} else {
		values = "'" + interaction+"_sub_"+fed + "', '" + interaction + "', '', '', '', '" + fed + "'";
	}

	std::cout << values;
}

void C2WConsoleLogger::addLog( const std::string &object, const std::string &attribute, const std::string &fed, bool publish ) {

	std::string values;

	if ( publish ) {
		values = "'" + fed+"_pub_"+object+"_"+attribute + "', '', '" + object + "', '" + attribute + "', '" + fed + "', ''";
	} else {
		values = "'" + object+"_"+attribute+"_sub_"+fed + "', '', '" + object + "', '" + attribute + "', '', '" + fed + "'";
	}

	std::cout << values;
}

void C2WConsoleLogger::addLog( const std::string &federate ) {

	std::string values = "'" + federate + "', '', '', '', '', ''";
	std::cout << values;
}

void C2WConsoleLogger::addLog(
 const std::string &id, const std::string &parameter, const std::string &value, const std::string &ptype, double time, const std::string &level
) {

	std::string logIdString = boost::lexical_cast< std::string >( ++_logId );
	std::string levelString = level.empty() ? "''" : "'" + level + "'";

	char timeCharArray[60];
	snprintf( timeCharArray, 60, TIME_LOG_FORMAT, time );
	std::string timeString( timeCharArray );

	std::string values = "'" + id + "', " + timeString + ", '" + parameter + "', '" + value + "', '" + ptype +
	 "', " + levelString + ", '" + logIdString + "'";

	std::cout << values;
}

void C2WConsoleLogger::addLog( const std::string &id, const InteractionIf &interactionRoot, double time, const std::string &level ) {

	std::string logIdString = boost::lexical_cast< std::string >( ++_logId );
	std::string levelString = level.empty() ? "''" : "'" + level + "'";

	char timeCharArray[60];
	snprintf( timeCharArray, 60, TIME_LOG_FORMAT, time );
	std::string timeString( timeCharArray );

	StringVector stringVector = interactionRoot.getParameterNames();
	if ( stringVector.empty() ) {
		std::string values = "'" + id + "', " + timeString + ", '', '', '', " + levelString +", '" + logIdString + "'";

		std::cout << values;
	}
	for( StringVector::iterator issItr = stringVector.begin() ; issItr != stringVector.end() ; ++issItr ) {

		std::string parameterName( *issItr );

		TypeMedley parameterValue = interactionRoot.getParameter( parameterName );
		std::string parameterValueString( parameterValue );
		std::string parameterTypeString( parameterValue.getTypeName() );

		if ( parameterTypeString == "std::string" ) {
			parameterTypeString = "String";
		}

		std::string values = "'" + id + "', " + timeString + ", '" + parameterName + "', '" + parameterValueString + "', '" + parameterTypeString +
		 "', " + levelString +", '" + logIdString + "'";

		std::cout << values;
	}

}

void C2WConsoleLogger::addLog( const std::string &id, const ObjectIf &objectRoot, double time, const std::string &level ) {

	std::string logIdString = boost::lexical_cast< std::string >( ++_logId );

	char timeCharArray[60];
	snprintf( timeCharArray, 60, TIME_LOG_FORMAT, time );
	std::string timeString( timeCharArray );
	std::string levelString = level.empty() ? "''" : "'" + level + "'";

	StringVector stringVector = objectRoot.getAttributeNames();
	if ( stringVector.empty() ) {
		std::string values = "'" + id + "', " + timeString + ", '', '', '', " + levelString +", '" + logIdString + "'";

		std::cout << values;
	}
	for( StringVector::iterator ossItr = stringVector.begin() ; ossItr != stringVector.end() ; ++ossItr ) {

		std::string attributeName( *ossItr );

		TypeMedley attributeValue = objectRoot.getAttribute( attributeName );
		std::string attributeValueString( attributeValue );
		std::string attributeTypeString( attributeValue.getTypeName() );

		if ( attributeTypeString == "std::string" ) {
			attributeTypeString = "String";
		}

		std::string values = "'" + id + "', " + timeString + ", '" + attributeName + "', '" + attributeValueString + "', '" + attributeTypeString +
		 "', " + levelString +", '" + logIdString + "'";

		std::cout << values;
	}

}

void C2WConsoleLogger::addLog( const std::string &federate, const std::string &info, const std::string &level, double time ) {
	char timeCharArray[60];
	snprintf( timeCharArray, 60, TIME_LOG_FORMAT, time );
	std::string timeString( timeCharArray );


	std::string values = "'" + federate + "', " + timeString + ", '', '" + info + "', '', '" + level + "'";
	std::cout << values;
}

int C2WConsoleLogger::getLogLevel( const std::string &loglevel ) {

	int level = 5;

	if ( loglevel == "HIGHLY_IMPORTANT" ) {
		level = 0;
	} else if ( loglevel == "IMPORTANT" ) {
		level = 1;
	} else if ( loglevel == "NORMAL" ) {
		level = 2;
	} else if ( loglevel == "DETAILED" ) {
		level = 3;
	} else if ( loglevel == "HIGHLY_DETAILED" ) {
		level = 4;
	}

	return level;
}
