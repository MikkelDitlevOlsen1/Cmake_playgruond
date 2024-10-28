#include <pigpio.h>
#include <iostream>
#include <unistd.h> // for usleep
// Define the pin numbers (Broadcom numbering)
const int resetpin =  25;

void initializePins() {
   gpioSetMode(resetpin,PI_OUTPUT);
}

int main() {
    // Initialize pigpio
    int sleptime=200;
    std::cout << "Initializing pigpio" << std::endl;
    if (gpioInitialise() < 0) {
    	std::cout << "error" << std::endl;
        return 1;
    }
    std::cout << "QCA now reset" << std::endl;
    gpioWrite(resetpin, 0);
    usleep(sleptime);
    gpioWrite(resetpin, 1);
    gpioTerminate();
    
    return 0;
}
