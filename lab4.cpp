/* Cameron Chen and Axel Sundstrom
   CSS 503 - Lab 4: UDP Multicast
   This lab introduces UDP multicast communication in C++
   You will code a simple client-server program that multicasts a given message from a single client to multiple servers.
*/

/* Methods
   * UdpSocket(char group[], int port): constructor opens a UDP datagram socket and has it participate in a given group address (e.g. "238.255.255.255") at a given port
   * ~UdpSocket(): destructor which closes the UDP socket 
   * int getClientSocket(): uses socket descriptor as a client multicast socket and returns a socket descriptor - socket descriptor not directly used 
   * int getServerSocket(): uses socket descriptor as a server multicast socket and returns a socket descriptor - socket descriptor not directly used
   * bool multicast(char buf[]): has a client multicast send a message in buf[] to all servers belonging to teh same group@port and returns true upon a sucessful send
   * bool recv(char buf[], int size): has a server receive a multicast message into a given buf[] with the size and returns true upon a successful receive
*/

#include "udp_multicast.h"
#include <iostream>
using namespace std;

#define SIZE 1024

int main(int argc, char *argv[]) {
    // validate arguments
    if (argc < 3) {
        cerr << "usage: lab4 group port [message]" << endl;
        return -1;
    }
    char *group = argv[1];
    int port = atoi(argv[2]);
    if (port < 5001) {
        cerr << "usage: lab4 group port [message]" << endl;
    }
    char *message = (argc == 4) ? argv[3] : NULL;

    // if message is null, the program should behave as a server, else it should behave as a client
    UdpMulticast udp(group, port);
    if (message) {
        // client - sends messages
        if (udp.getClientSocket() < 0) {
            cerr << "Failed to create client socket" << endl;
            return -1;
        }
        if (!udp.multicast(message)) {
            cerr << "Failed to send multicast message" << endl;
            return -1;
        }
        cout << "Multicast message sent: " << message << endl;
    } else {
        // server - receives messages 
        if (udp.getServerSocket() < 0) {
            cerr << "Failed to create server socket" << endl;
            return -1;
        }
        char buf[SIZE];
        while (true) {
            if (udp.recv(buf, SIZE)) {
                cout << "Received multicast message: " << buf << endl;
            } else {
                cerr << "Failed to receive multicast message" << endl;
            }
        }
    }
    return 0;
}