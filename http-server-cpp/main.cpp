//
//  main.cpp
//  http-server-cpp
//
//  Created by justin Herrick on 6/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <stdexcept>
#include "server.h"

void start( const char *port_number, const char *dir)
{
    printf("Starting Server...\n"); 
    
    Server server;
    server.start( port_number, dir );
}

int main( int arg_count, char** arguments )
{
    const char *port_number = "3000";
    const char *dir;
    if (arg_count > 1) {
        dir = arguments[1];
    } else {
        dir = "/Users/Justin/8thlight/tasks/http-server-cpp/http-server-cpp";
    }
    try
    {
        start( port_number, dir );
    }
    catch ( std::runtime_error error) 
    {
        printf("Error: %s\n\n", error.what() );
    }
}