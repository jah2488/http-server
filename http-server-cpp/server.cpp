//
//  server.cpp
//  http-server-cpp
//
//  Created by justin Herrick on 6/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include "logger.h"
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "server.h"
#include <iostream>
#include <fstream>
#include <unistd.h>

Server::Server( ){}

void sigchld_handler( int s)
{
    while( waitpid( -1, NULL, WNOHANG) > 0);
}

void *get_in_addr( struct sockaddr *sa)
{
    if ( sa->sa_family == AF_INET){
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

std::string getRoute(char * route)
{
    
    long elapsed_seconds;
    char line[500];
    
    char currentDir[FILENAME_MAX];
    
    chdir("/Users/jherrick/8thLight/tasks/http-server/http-server-cpp");
    getcwd(currentDir, sizeof(currentDir));
    printf("Current Dir is \n %s", currentDir);
    
    std::ifstream file("./index.html");
    
    std::string response;
    
    if (file) {
        while ( !file.eof() ) {
            file.read(line, sizeof(line));
            response.append(line);
            printf("%s\n", line);
        }
        file.close();       
    }
    return response;
}

void handleConnection(int socket_file_descriptor, int new_socket_file_descriptor)
{
    close(socket_file_descriptor);
//            if ( send(new_socket_file_descriptor, "Hello There!", 13, 0) == -1) { perror("Server error on send"); }
    char buffer[500];
    recv(new_socket_file_descriptor, buffer, 500, 0);
    
    char * header;
    header = strtok(buffer, "\n");
    printf("\n=== Request Info ===\n %s", header);
    
    char * method;
    char * route;
    char * http_version;
    
    char *tok = strtok(header, " ");
    method = tok;
    route  = tok = strtok(NULL, " ");
    http_version = tok = strtok(NULL, " ");
    
    printf("\n method : %s\n route : %s\n version : %s\n", method, route, http_version);
    std::string response = getRoute(route);
    // Respond with response header.
    // Send file
    // Check for invalid files.
    // -- send 404s
    write(new_socket_file_descriptor, "Hello~", 6);
    close(new_socket_file_descriptor);
    exit(0);
}

void Server::start( const char *port_number )
{
    /// -- Building and binding socket >> 
    int socket_file_descriptor, new_socket_file_descriptor;
    int yes = 1;
    int rv;
    
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage client_address;
    struct sigaction sig_action;
    
    socklen_t sin_size;
    
    char s[INET6_ADDRSTRLEN];
    
    memset( &hints, 0, sizeof hints );
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;
    
    if (( rv = getaddrinfo( NULL, port_number, &hints, &servinfo )) != 0 ) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }
    
    for ( p = servinfo; p != NULL; p = p->ai_next ) {
        if (( socket_file_descriptor = socket( p->ai_family, p->ai_socktype, p->ai_protocol)) == -1 ){
            perror("Server error on socket creation");
            continue;
        }
        if ( setsockopt( socket_file_descriptor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1 ){
            perror("Critical error setsockopt");
            exit(1); 
        }
        if ( bind( socket_file_descriptor, p->ai_addr, p->ai_addrlen) == -1 ){
            close(socket_file_descriptor);
            perror("Server error on binding to port");
            continue;
        }
        
        break;
    }
    
    if ( p == NULL ){
        fprintf(stderr, "Critical error, failed to bind\n");
        exit(2);
    }
    freeaddrinfo(servinfo);
    /// -- Building and binding socket ^^
    
    if ( listen( socket_file_descriptor, 10) == -1 ){
        perror("Critical error on call to listen");
        exit(1);
    }
    
    sig_action.sa_handler = sigchld_handler;
    sigemptyset(&sig_action.sa_mask);
    sig_action.sa_flags = SA_RESTART;
    if ( sigaction(SIGCHLD, &sig_action, NULL) == -1){
        perror("Critical error on sigaction");
        exit(1);
    }
    
    printf("[%d]Server is running and waiting for connections on port %s\n", getpid(), port_number);
    
    while(1){
        sin_size = sizeof client_address;
        new_socket_file_descriptor = accept( socket_file_descriptor, (struct sockaddr *) &client_address, &sin_size);
        if ( new_socket_file_descriptor == -1){
            perror("Server error on accept");
            continue;
        }
        
        
        inet_ntop( client_address.ss_family, get_in_addr((struct sockaddr *) &client_address), s, sizeof s);
        
        printf("[%d]Server received connection from %s\n", getpid(), s);
        
        if ( !fork() ){
            handleConnection(socket_file_descriptor, new_socket_file_descriptor);
       }
        close(new_socket_file_descriptor);
    }
    
}