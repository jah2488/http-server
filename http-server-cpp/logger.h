//
//  logger.h
//  http-server-cpp
//
//  Created by justin Herrick on 6/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef logger_h
#define logger_h

class Logger
{
public:
    void pout( const char *msg );
    void perr( const char *msg );
};


#endif
