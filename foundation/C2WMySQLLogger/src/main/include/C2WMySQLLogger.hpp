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

#ifndef _C2WMYSQLLOGGER_HPP
#define _C2WMYSQLLOGGER_HPP

//#ifdef USE_MYSQL_LOGGING
	#include "mysql_connection.h"
	#include <cppconn/driver.h>
	#include <cppconn/exception.h>
	#include <cppconn/resultset.h>
	#include <cppconn/statement.h>
//#endif

#include <string>

#include "C2WLogger.hpp"

class C2WMySQLLogger : public C2WLogger {

private:
	std::string _username;
	std::string _password;

//#ifdef USE_MYSQL_LOGGING
	sql::Driver *_sqlDriver;
	sql::Connection *_sqlConnection;
	sql::Statement *_sqlStatement;

	int _logId;

//#endif
public:
	class ExecuteUpdate {

		private:
			std::string _command;

		public:
			ExecuteUpdate( const std::string &command ) : _command( command ) { }
			void operator()( void );
	};

	C2WMySQLLogger( void ) : _username( "root" ), _password( "c2wt" ), _sqlConnection( 0 ), _sqlStatement( 0 ), _logId( -1 ) {
		 _sqlDriver = get_driver_instance();
	}

	sql::Statement *getSqlStatement( void ) {
		return _sqlStatement;
	}

public:
	~C2WMySQLLogger( void ) {
		delete _sqlStatement;
		delete _sqlConnection;
	}

	static C2WMySQLLogger &get_singleton( void ) {
		static C2WMySQLLogger c2wLogger;
		return c2wLogger;
	}

	void createDB( const std::string &dbName );

	//void init( const std::string &db_name, const std::string &dbURL = "localhost" );
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

#endif //_C2WMYSQLLOGGER_HPP
