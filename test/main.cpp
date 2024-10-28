#include <stdio.h>
#include <string>
#include <chrono>
#include <thread>
#include <pigpio.h>
#include <iostream>
#include <unistd.h> // for usleep
// Define the pin numbers (Broadcom numbering)
const int RELAY_PIN_1 = 12;
const int RELAY_PIN_2 = 16;
const int CP_COMM_PIN = 1;
const int SIM_PIN_2 = 27;
const int SIM_PIN_1 = 22;
const int PP_STATE_PIN = 13;
const int PWM_PIN = 20;
const int cp_con2 = 26;
const int cp_con1 = 19;
const int Motor_lock =  24;
const int Motor_open =  18;
const int I2CAddr[3] = {0x50, 0x51, 0x52}; //0x50 == CableReading, 0x51 == CP Line Reading, 0x52 == Temp Sensor
//const string CPState[3] = {"CP State A", "CP State B", "CP State C"}; //Unit: Volt

void startIEC(){

   gpioWrite(cp_con1, 1);

}

void stopIEC(){

   gpioWrite(cp_con1, 0);

}

int ADCCode(int addr, int readAmount){

    int handle, byteData, i;
    //Parameters: I2C line 1, ADC address, flags
    handle = i2cOpen(1,addr,0);
    for(i = 0; i < readAmount; i++){
        byteData =i2cReadByteData(handle,0);
        printf("Data: %d, addr: %d\n",byteData, addr);
        fflush(stdout);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
   }
   return byteData;
}

int tempSensorReading(){
   int temp, tempConverted = 0;
   temp = ADCCode(I2CAddr[2],100);
   tempConverted = temp/1;
   printf("Temperature is: %d degrees celcius\n", tempConverted);
   return tempConverted;
}

void CPStateRead(){

   ADCCode(I2CAddr[1],100);

}

int ReadCableCapabilities(){

   int I2CAddr = 0x50, readAmount = 1, retval = 0;
   int AMPRatingConversion[2] = {16,32};
   retval = ADCCode(I2CAddr, readAmount);

   switch(retval){

   case 5:
      return AMPRatingConversion[0];
         break;

   case 2:
      return AMPRatingConversion[1];
         break;

   default:
      return -1;
         break;

   }

}

void initializePins() {
    gpioSetMode(RELAY_PIN_1, PI_OUTPUT);
    gpioSetMode(RELAY_PIN_2, PI_OUTPUT);
    gpioSetMode(CP_COMM_PIN, PI_OUTPUT);
    gpioSetMode(SIM_PIN_1, PI_OUTPUT);
    gpioSetMode(SIM_PIN_2, PI_OUTPUT);
    gpioSetMode(PP_STATE_PIN, PI_OUTPUT);
    gpioSetMode(PWM_PIN, PI_OUTPUT);
    gpioSetMode(cp_con1, PI_OUTPUT);
    gpioSetMode(cp_con2,PI_OUTPUT);

    gpioSetMode(Motor_lock,PI_OUTPUT);
    gpioSetMode(Motor_open,PI_OUTPUT);
}
void lock_lock(){
    gpioWrite(Motor_lock, 1);
    usleep(1000*600); // short sleep
    gpioWrite(Motor_lock, 0);
}

void open_lock(){
    gpioWrite(Motor_open, 1);
    usleep(1000*600); // short sleep
    gpioWrite(Motor_open, 0);
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
    usleep(100000*200); // short sleep
    gpioWrite(SIM_PIN_2, 1);
    std::cout << "Setting "<< SIM_PIN_2<<" pins to 1" << std::endl;
}

void set_cp_con_discharge(){
    gpioWrite(cp_con1, 1);
    gpioWrite(cp_con2,0);
    std::cout << "Setting " << cp_con1 << " pin to 1" << std::endl;
}

void set_cp_con_charge(){
    gpioWrite(cp_con1, 0);
    gpioWrite(cp_con2, 1);
    std::cout << "Setting " << cp_con2 << " pin to 1" << std::endl;
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

void startPWMSignal() { // Set PWM frequency to 1000 Hz
    gpioSetPWMfrequency(PWM_PIN, 1000);

    // Set PWM duty cycle to 5% (range is 0-255, so 5% of 255 is approximately 12)
    int dutyCycle = 200;
    gpioPWM(PWM_PIN, dutyCycle);
    std::cout << "Setting" << PWM_PIN << " dutecyel 5% 1000 hz" << std::endl;
    usleep(2000000);
}

void setAllPinsToZero() {
    gpioWrite(RELAY_PIN_1, 0);
    gpioWrite(RELAY_PIN_2, 0);
    gpioWrite(CP_COMM_PIN, 0);
    gpioWrite(SIM_PIN_1, 0);
    gpioWrite(SIM_PIN_2, 0);
    gpioWrite(PP_STATE_PIN, 0);
    gpioWrite(PWM_PIN, 0);
    gpioWrite(cp_con1, 0);
}

void enableSim(){
    gpioWrite(SIM_PIN_1, 1);
    usleep(1000000);
    gpioWrite(SIM_PIN_2, 1);
}
int main() {

    // Initialize pigpio
    int sleptime=5000000;
    std::cout << "Initializing pigpio" << std::endl;
    if (gpioInitialise() < 0) {
    	std::cout << "Initializing pigpio" << std::endl;
        return 1;
    }
    initializePins();
    lock_lock();
    int AMPRating;

    AMPRating = ReadCableCapabilities();

    printf("Cable is rated for: %d AMPs\n", AMPRating);
//    tempSensorReading();
    //std::cout << "now its hige" << std::endl;
//    enableSim();
    startIEC();
    std::cout << "Started charging" << std::endl;
    usleep(sleptime*5);
    open_lock();
    stopIEC();
    //gpioWrite(PWM_PIN, 1);
    //startPWMSignal();
    	
/*
    std::cout << "Initializing GPIO pins" << std::endl;
    initializePins();
    //gpioWrite(SIM_PIN_1,1);
    startPWMSignal();

    usleep(sleptime);
    set_cp_con_discharge();
    
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

    usleep(sleptime*50);
    std::cout << "Terminating pigpio" << std::endl;

*/
    setAllPinsToZero();
    gpioTerminate();
    
    return 0;
}
