/**
 ******************************************************************************
 * @file    x_nucleo_iks01a2.cpp
 * @author  AST / EST
 * @version V0.0.1
 * @date    9-August-2016
 * @brief   Implementation file for the X_NUCLEO_IKS01A2 singleton class
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
*/ 
    
/* Includes ------------------------------------------------------------------*/
#include "mbed.h"
#include "x_nucleo_iks01a2.h"

/* Static variables ----------------------------------------------------------*/
X_NUCLEO_IKS01A2* X_NUCLEO_IKS01A2::_instance = NULL;


/* Methods -------------------------------------------------------------------*/
/**
 * @brief  Constructor
 */
X_NUCLEO_IKS01A2::X_NUCLEO_IKS01A2(DevI2C *ext_i2c) : dev_i2c(ext_i2c),
    ht_sensor(new HTS221Sensor(*dev_i2c)),
    magnetometer(new LSM303AGR_MAG_Sensor(*dev_i2c)),
    accelerometer(new LSM303AGR_ACC_Sensor(*dev_i2c)),
    pt_sensor(new LPS22HBSensor(*dev_i2c)),
    acc_gyro(new LSM6DSLSensor(*dev_i2c))
{ 
  ht_sensor->Init(NULL);
  magnetometer->Init(NULL);
  accelerometer->Init(NULL);
  pt_sensor->Init(NULL);
  acc_gyro->Init(NULL);
}

/**
 * @brief     Get singleton instance
 * @return    a pointer to the initialized singleton instance of class X_NUCLEO_IKS01A2.
 *            A return value of NULL indicates an out of memory situation.
 * @param[in] ext_i2c (optional) pointer to an instance of DevI2C to be used
 *            for communication on the expansion board. 
 *            Defaults to NULL.
 *            Taken into account only on the very first call of one of the 'Instance' functions.
 *            If not provided a new DevI2C will be created with standard
 *            configuration parameters.
 *            The used DevI2C object gets saved in instance variable dev_i2c.
 */
X_NUCLEO_IKS01A2* X_NUCLEO_IKS01A2::Instance(DevI2C *ext_i2c) {
    if(_instance == NULL) {
        if(ext_i2c == NULL)
            ext_i2c = new DevI2C(IKS01A2_PIN_I2C_SDA, IKS01A2_PIN_I2C_SCL);

        if(ext_i2c != NULL)
            _instance = new X_NUCLEO_IKS01A2(ext_i2c);
    }

    return _instance;
}

/**
 * @brief     Get singleton instance
 * @return    a pointer to the initialized singleton instance of class X_NUCLEO_IKS01A1.
 *            A return value of NULL indicates an out of memory situation.
 * @param[in] sda I2C data line pin.
 *            Taken into account only on the very first call of one of the 'Instance' functions.
 *            A new DevI2C will be created based on parameters 'sda' and 'scl'.
 *            The used DevI2C object gets saved in instance variable dev_i2c.
 * @param[in] scl I2C clock line pin.
 *            Taken into account only on the very first call of one of the 'Instance' functions.
 *            A new DevI2C will be created based on parameters 'sda' and 'scl'.
 *            The used DevI2C object gets saved in instance variable dev_i2c.
 */
X_NUCLEO_IKS01A2* X_NUCLEO_IKS01A2::Instance(PinName sda, PinName scl) {
    if(_instance == NULL) {
        DevI2C *ext_i2c = new DevI2C(sda, scl);

        if(ext_i2c != NULL)
            _instance = new X_NUCLEO_IKS01A2(ext_i2c);
    }

    return _instance;
}
