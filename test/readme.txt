
Yes, you can use the pigpio library for GPIO control on the Raspberry Pi using C++. The pigpio library is highly accurate and provides more advanced features compared to WiringPi. Below are the steps to install pigpio, and example C++ programs to write to and read from GPIO pins.

1. Install pigpio Library
First, you need to install the pigpio library. Open a terminal and execute the following commands:
-sudo apt-get update
-sudo apt-get install pigpio

2. Start the pigpiod Daemon
Before using the library, you need to start the pigpiod daemon, which manages GPIO access:
-sudo pigpiod

3. run code
se if pigpio is there
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed!" << std::endl;
        return 1;
    }

creat a pin numbere to evestigate
int ledPin = 17
