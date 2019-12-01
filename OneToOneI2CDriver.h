/*
 * OneToOneI2CDriver.h
 *
 *  Created on: Sep 24, 2016
 *      Author: Raz Aloni
 */

#ifndef ONETOONEI2CDRIVER_H_
#define ONETOONEI2CDRIVER_H_
#include <F2837xD_Device.h>
#define I2CA_Succesful 0x00
#define I2CA_Busy 0x01
#define I2CA_NACK 0x02
#define I2CA_MastFail 0x03
#define I2CA_StartFail 0x04
extern Uint16 I2CA_BusyCnt;
extern Uint16 I2CA_NACKCnt;
extern Uint16 I2CA_StartFailCnt;
extern Uint16 I2CA_MastFailCnt;
extern Uint16 I2CA_Status;
extern Uint16 I2CA_Write[10];
extern Uint16 I2CA_Read[10];
/*
 * <summary>
 * 	Initializes the I2C to run in Master Mode for a One-To-One connection
 * </summary>
 * <param="slaveAddress">Address of the slave device to write to</param>
 * <param="sysClkMhz">System Clock Frequency in Mhz</param>
 * <param="I2CClkKHz">Desired I2C Clock Frequency in KHz</param>
 */
void I2C_O2O_Master_Init(Uint16 slaveAddress, float32 sysClkMhz, float32 I2CClkKHz);

/*
 * <summary>
 * 	Sends bytes via I2C
 * </summary>
 * <param="values">Pointer to array of bytes to send</param>
 * <param-"length">Length of array</param>
 */
Uint16 I2C_O2O_SendBytes(Uint16 * const values, Uint16 length);

Uint16 I2C_O2O_ReadBytes(Uint16 * const values, Uint16 length);
#endif /* ONETOONEI2CDRIVER_H_ */
