#include <iostream>
#include "ILI9341.h"
#include <unistd.h>
#include<ctime>

using namespace std;

int main()
{
    GPIO led(134);
    led.Write(GPIO::HIGH);
    ILI9341 lcd("/dev/spidev32766.0", 133, 132);
    lcd.begin(true);
    clock_t begin = clock();

    for(int i = 0; i < 10; ++i){
    lcd.fillScreen(ILI9341_ALICEBLUE);
    usleep(500000);
    lcd.fillScreen(ILI9341_YELLOW);
    usleep(500000);
    lcd.fillScreen(ILI9341_BLUE);
    usleep(500000);
    lcd.fillScreen(ILI9341_RED);
    usleep(500000);
    lcd.fillScreen(ILI9341_GREEN);
    usleep(500000);
    lcd.fillScreen(ILI9341_PURPLE);
    usleep(500000);
    lcd.fillScreen(ILI9341_ORANGE);
    usleep(500000);
    lcd.fillScreen(ILI9341_ALICEBLUE);
    usleep(500000);
    lcd.fillScreen(ILI9341_YELLOW);
    usleep(500000);
    lcd.fillScreen(ILI9341_BLUE);
    usleep(500000);
    }
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("Elapsed: %.2f\n", elapsed_secs);

    //usleep(500000);
    return 0;
}
