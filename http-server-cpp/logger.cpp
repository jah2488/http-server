//
//  logger.cpp
//  http-server-cpp
//
//  Created by justin Herrick on 6/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "logger.h"

void Logger::pout( const char *msg )
{
    time_t now;
    time(&now);
    printf("[%d][%s]: %s \n", getpid(), ctime(&now), msg ); 
}

void Logger::perr( const char *msg )
{
    perror(msg);
}