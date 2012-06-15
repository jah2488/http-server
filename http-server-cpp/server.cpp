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
#include <time.h>
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

char getRoute(char * route)
{
    
    std::ifstream file;
    file.open("./index.html");
    
    file.seekg(0, std::ios::end);
    long length = file.tellg();
    file.seekg(0, std::ios::beg);
    
    printf("File Length : %ld\n", length);
    char file_buffer[length];
    
    file.read(file_buffer,length);
    file.close();
    
    
    return *file_buffer;
    // content length goes from 130 - 60 and output is truncated to '<'
}

void handleConnection(int socket_file_descriptor, int new_socket_file_descriptor)
{
    close(socket_file_descriptor);
    
    //-- request headers -- extract function
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
    //-- request headers
    
    //-- load file -- extract function
    std::ifstream file;
    file.open("./index.html");
    
    file.seekg(0, std::ios::end);
    long length = file.tellg();
    file.seekg(0, std::ios::beg);
    
    printf("[%d]File Length : %ld\n", getpid(),length);
    char file_buffer[length];
    
    file.read(file_buffer,length);
    file.close();  
    //-- load file
    
    //-- generate timeDate stamp
    time_t now;
    time(&now);
    char date[39]; 
    strftime(date, sizeof(date), "Date: %a, %d %b %Y %X %Z\r\n\r\n", gmtime(&now));
    //-- generate timeDate stamp
   
    //-- response -- extract function
    send(new_socket_file_descriptor, "HTTP/1.1 200\r\n", 14, 0);
    send(new_socket_file_descriptor, "Content-type: text/html\r\n", 25, 0);
    send(new_socket_file_descriptor, "Server: jherrick-server/0.1\r\n", 29, 0);
    send(new_socket_file_descriptor, date, sizeof(date), 0);
    send(new_socket_file_descriptor, file_buffer, sizeof(file_buffer), 0);
    close(new_socket_file_descriptor);
    //-- response
    
    exit(0);
}

void Server::start( const char *port_number, const char *dir )
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
    
    printf("Server is running and waiting for connections on port %s\n", port_number);
     
    // - change working dir -
    if (chdir( dir ) == -1) { 
        printf("Error in Changing Directory: %d\n", errno);
    } else {
        char crdir[FILENAME_MAX];
        printf("Directory Changed to: %s\n", getcwd(crdir, sizeof(crdir)));
    }
    // - change workind dir -
    
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