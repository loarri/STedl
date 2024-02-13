/**
 ******************************************************************************
 * @project_name  STedl
 * @file    main.cpp
 * @author  Lorenzo Arrigoni
 * @mail    loarri@gmail.com
 * @version V1.0.0
 * @date    30 Jan 2024
 * @brief   Simple Example application for using the X_NUCLEO_IKS01A2
 *          MEMS Inertial & Environmental Sensor expansion board plus
 *          an MQ-7 gas sensor plus a LCD 16x2 display.
 ******************************************************************************
 * @attention
 *
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
*/ 

/* Includes */
#include "mbed.h"
#include "XNucleoIKS01A2.h"
#include "MQ7.h"
#include "TextLCD.h"
#include "buzzer.h"

#define DURATA 3 //durata in sec del dato sul display
#define NUM_CYCLES_ENABLE_PRINT_CO 15 //wait MQ-7 sensor stability!

bool enable_print_co =0;
int counter_enable_print_co = 0;

// I2C Communication
I2C i2c_lcd(I2C_SDA , I2C_SCL); // SDA, SCL

/* define buzzer pin */
Beep buzz(D7);

/* Instantiate the LCD display */
// LCD I2C address: 0x4E or 0x27
TextLCD_I2C lcd(&i2c_lcd, 0x4E, TextLCD::LCD16x2); // I2C bus, PCF8574 Slaveaddress, LCD Type

/* Instantiate the expansion board */
static XNucleoIKS01A2 *mems_expansion_board = XNucleoIKS01A2::instance(D14, D15, D4, D5);

/* Instantiate the MQ-7 gas sensor */
int CO_Level                    = 1;
MQ7 CO_Sensor(A0,D6);
//Ticker poll_sensor;

/* Retrieve the composing elements of the expansion board */
static HTS221Sensor *hum_temp = mems_expansion_board->ht_sensor;
static LPS22HBSensor *press_temp = mems_expansion_board->pt_sensor;

/* Helper function for printing floats & doubles */
static char *print_double(char* str, double v, int decimalDigits=2)
{
  int i = 1;
  int intPart, fractPart;
  int len;
  char *ptr;

  /* prepare decimal digits multiplicator */
  for (;decimalDigits!=0; i*=10, decimalDigits--);

  /* calculate integer & fractinal parts */
  intPart = (int)v;
  fractPart = (int)((v-(double)(int)v)*i);

  /* fill in integer part */
  sprintf(str, "%i.", intPart);

  /* prepare fill in of fractional part */
  len = strlen(str);
  ptr = &str[len];

  /* fill in leading fractional zeros */
  for (i/=10;i>1; i/=10, ptr++) {
    if (fractPart >= i) {
      break;
    }
    *ptr = '0';
  }

  /* fill in (rest of) fractional part */
  sprintf(ptr, "%i", fractPart);

  return str;
}

/* check the gas sensor value - NOT USED */
void check_sensor() {
    CO_Level = CO_Sensor.get_CO_value ();
    if ( CO_Level == 1 )
    {
        printf("\r\n*** CO LEVEL IS SAFE/LOW *** \r\n");
    }
    else if ( CO_Level == 2 )
    {
        printf("\r\n*** WARNING!! CO LEVEL IS MODERATE *** \r\n");
    }
    else
    {
        printf("\r\n*** DANGER!!! CO LEVEL IS HIGH *** \r\n");
    }
}



/* Simple main function */
int main() {
  uint8_t id;
  float value1, value2, value3;
  char buffer1[32], buffer2[32], buffer3[32];

  /* setup display */
  lcd.setBacklight(TextLCD::LightOn);
  lcd.setCursor(TextLCD::CurOff_BlkOff);

  /* Enable all sensors */
  hum_temp->enable();
  press_temp->enable();
  
  /* initialize buzzer */
  buzz.nobeep();

  printf("\n\rSTedl! - Environmental data logger - 2024\n\r");
  lcd.printf("STedl!\n");
  lcd.printf("by L. Arrigoni");
  wait(DURATA);
  lcd.cls();

  while(1) {
    printf("\r\n");

    if (counter_enable_print_co < NUM_CYCLES_ENABLE_PRINT_CO ) counter_enable_print_co++;
    if (counter_enable_print_co == NUM_CYCLES_ENABLE_PRINT_CO) {
        enable_print_co =1;
        printf("\n > Enable CO LCD print & buzzer!!\n");
    }
    hum_temp->get_temperature(&value1);   // get Temperature
    hum_temp->get_humidity(&value2);      // get Humidity
    press_temp->get_pressure(&value3);    // get Pressure
   

    printf("[temp] %7s C, [hum] %s%%, [press] %s mbar\r\n", print_double(buffer1, value1), print_double(buffer2, value2),print_double(buffer3, value3));
    
    lcd.clrIcon();
    lcd.printf("Temp.:%7s C", print_double(buffer1, value1));
    wait(DURATA);
    lcd.cls();
    lcd.printf("Hum.: %s%% ", print_double(buffer2, value2));  
    wait(DURATA);
    lcd.cls();
    lcd.printf("P.: %s mbar\n", print_double(buffer3, value3));  
    wait(DURATA);
    lcd.cls();
    CO_Level = CO_Sensor.get_CO_value ();
    printf("\r\n\t valore di CO_level:%d",CO_Level);
    if (enable_print_co) {
        if ( CO_Level == 1 )
        {
            lcd.printf("CO LEVEL IS SAFE/LOW");
        }
        else if ( CO_Level == 2 )
        {
            lcd.printf("WARNING!! CO LEVEL IS MODERATE");
        }
        else
        {
            lcd.printf("DANGER!!! CO LEVEL IS HIGH");
            buzz.beep(2000,3);
        }
        //thread_sleep_for(DURATA);
        //ThisThread::sleep_for(DURATA);
        wait(DURATA);
        lcd.cls();
    }
  }
}
