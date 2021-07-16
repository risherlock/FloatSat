
#include <stdio.h>

#include "UdpHW_common.h"

UDPOutLx udpOut(-5001, (const char*)"localhost"); // minus for multicast to this port

int main([[gnu::unused]] int argc, [[gnu::unused]] char *argv[]) {
  
  printf("Sending to localhost:xxxx\n");
  char inputline[1300];
  while(1) {
    printf("Data to send:");
    fgets(inputline, 1300, stdin);
    
     udpOut.send(inputline, 1300);
  }
}
