#include <string>
#include <fstream>
#include <iostream>

class GPIO
{
public:
    enum Direction{
        INPUT = 0,
        OUTPUT
    };
    enum Value{
        LOW = 0,
        HIGH = 1
    };
    GPIO(int pin, Direction dir = OUTPUT)
    {
        std::ofstream exportFile;
        exportFile.open ("/sys/class/gpio/export");
        if(!exportFile.is_open())
        {
            std::cout<<"Error opening gpio export "<<pin<<std::endl;
        }
        exportFile << pin;
        exportFile.close();
        exportFile.open("/sys/class/gpio/gpio"+std::to_string(pin)+"/direction");
        if(!exportFile.is_open())
        {
            std::cout<<"Error opening gpio direction "<<pin<<std::endl;
        }
        exportFile<< ((dir==OUTPUT)?"out":"in");
        exportFile.close();

        mOpenPin.open("/sys/class/gpio/gpio"+std::to_string(pin)+"/value");
        if(!mOpenPin.is_open())
        {
            std::cout<<"Error opening gpio value "<<pin<<std::endl;
        }
        mPin = pin;
    }
    ~GPIO()
    {
        Write(LOW);
        mOpenPin.close();
        std::ofstream exportFile;
        exportFile.open ("/sys/class/gpio/unexport");
        exportFile << mPin;
        exportFile.close();
    }

    void Write(Value value)
    {
        mOpenPin<<value;
        mOpenPin.flush();
    }

protected:
private:
    std::ofstream mOpenPin;
    int mPin;
};



