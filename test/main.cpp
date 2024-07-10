#include <pigpio.h>
#include <iostream>
#include <unistd.h> // for usleep
// Define the pin numbers (Broadcom numbering)
const int RELAY_PIN_1 = 12;
const int RELAY_PIN_2 = 16;
const int CP_COMM_PIN = 26;
const int SIM_PIN_1 = 13;
const int SIM_PIN_2 = 19;
const int PP_STATE_PIN = 24;
const int PWM_PIN = 20;
const int CP_controle = 25

void initializePins() {
    gpioSetMode(RELAY_PIN_1, PI_OUTPUT);
    gpioSetMode(RELAY_PIN_2, PI_OUTPUT);
    gpioSetMode(CP_COMM_PIN, PI_OUTPUT);
    gpioSetMode(SIM_PIN_1, PI_OUTPUT);
    gpioSetMode(SIM_PIN_2, PI_OUTPUT);
    gpioSetMode(PP_STATE_PIN, PI_OUTPUT);
    gpioSetMode(PWM_PIN, PI_OUTPUT);
    gpioSetMode(CP_controle, PI_OUTPUT);
}

void stopCharging() {

    gpioWrite(RELAY_PIN_1, 0);
    gpioWrite(RELAY_PIN_2, 0);
    std::cout << "Setting "<< RELAY_PIN_1<<" pins to 0" << std::endl;
    std::cout << "Setting "<< RELAY_PIN_2<<" pins to 0" << std::endl;
    usleep(100000); // short sleep
    gpioWrite(CP_COMM_PIN, 1);
    std::cout << "Setting "<< CP_COMM_PIN<<" pins to 1" << std::endl;
    usleep(100000); // short sleep
    gpioWrite(SIM_PIN_1, 1);
    std::cout << "Setting "<< SIM_PIN_1<<" pins to 1" << std::endl;
    usleep(100000); // short sleep
    gpioWrite(SIM_PIN_2, 1);
    std::cout << "Setting "<< SIM_PIN_2<<" pins to 1" << std::endl;
}

void cp_controle(){
    gpioWrite(CP_controle, 0);
    std::cout << "Setting " << CP_controle << " pin to 0" << std::endl;
}
void startCharging() {
    gpioWrite(SIM_PIN_1, 0);
    std::cout << "Setting " << SIM_PIN_1 << " pin to 0" << std::endl;
    usleep(100000); // short sleep
    gpioWrite(SIM_PIN_2, 0);
    std::cout << "Setting " << SIM_PIN_2 << " pin to 0" << std::endl;
    usleep(100000); // short sleep
    gpioWrite(CP_COMM_PIN, 0);
    std::cout << "Setting " << CP_COMM_PIN << " pin to 0" << std::endl;
    usleep(100000); // short sleep
    gpioWrite(RELAY_PIN_1, 1);
    std::cout << "Setting " << RELAY_PIN_1 << " pin to 1" << std::endl;
    gpioWrite(RELAY_PIN_2, 1);
    std::cout << "Setting " << RELAY_PIN_2 << " pin to 1" << std::endl;
}

void setPPStateNotCharging() {
    gpioWrite(PP_STATE_PIN, 0);
    std::cout << "Setting " << PP_STATE_PIN << " pin to 0" << std::endl;
}

void startPWMSignal() {
    // Set PWM frequency to 1000 Hz
    gpioSetPWMfrequency(PWM_PIN, 1000);

    // Set PWM duty cycle to 5% (range is 0-255, so 5% of 255 is approximately 12)
    int dutyCycle = 255 * 0.05;
    gpioPWM(PWM_PIN, dutyCycle);
    std::cout << "Setting" << PWM_PIN << " dutecyel 5% 1000 hz" << std::endl;
}

void setAllPinsToZero() {
    gpioWrite(RELAY_PIN_1, 0);
    gpioWrite(RELAY_PIN_2, 0);
    gpioWrite(CP_COMM_PIN, 0);
    gpioWrite(SIM_PIN_1, 0);
    gpioWrite(SIM_PIN_2, 0);
    gpioWrite(PP_STATE_PIN, 0);
    gpioWrite(PWM_PIN, 0);
}
int main() {
    // Initialize pigpio
    int sleptime=1000000;
    std::cout << "Initializing pigpio" << std::endl;
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed!" << std::endl;
        return 1;
    }

    
    std::cout << "Initializing GPIO pins" << std::endl;
    initializePins();

    usleep(sleptime);
    CP_controle();
    
    std::cout << "Setting PP state to not charging" << std::endl;
    setPPStateNotCharging();

    usleep(sleptime);
    std::cout << "Stopping charging simulation" << std::endl;
    stopCharging();

    usleep(sleptime);
    std::cout << "Starting PWM signal" << std::endl;
    startPWMSignal();

    usleep(sleptime);
    std::cout << "Simulating delay before starting charging again" << std::endl;
    usleep(sleptime);
    usleep(5000000); // sleep for 5 seconds
    std::cout << "Starting charging simulation" << std::endl;
    startCharging();

    usleep(sleptime);
    std::cout << "Terminating pigpio" << std::endl;

    setAllPinsToZero()
    gpioTerminate();
    

    

       return 0;
}
