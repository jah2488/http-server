//
//  main.cpp
//  http-server-cpp
//
//  Created by justin Herrick on 6/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <stdexcept>
//#include <igloo/igloo_alt.h>
#include "server.h"
//using namespace igloo;

void start( const char *port_number )
{
    printf("Starting Server...\n"); 
    
    Server server;
    server.start( port_number );
}

int main( int arg_count, char** arguments )
{
//    TestRunner::RunAllTests();
    
    const char *port_number = "3000";
    
    try
    {
        start( port_number );
    }
    catch ( std::runtime_error error) 
    {
        printf("Error: %s\n\n", error.what() );
    }
}