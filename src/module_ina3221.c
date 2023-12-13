#include "module_ina3221.h"

static uint8_t buffer[2];

uint16_t ina_getReg(INA3221_DEVICE *device, INA3221_REG reg)
{
	uint8_t ret = -1;
	uint8_t regByte = reg;
	ret = device->write(device->address << 1, &regByte, 1);
	ret = device->read(device->address << 1, buffer, 2);
	if(ret){
		return (((uint16_t)buffer[0]) << 8) + buffer[1];
	}
	return -1;
}


uint8_t ina_setReg(INA3221_DEVICE *device, INA3221_REG reg, uint16_t data)
{
	uint8_t ret = -1;
	uint8_t cfg[3] = {reg, data >> 8, data};
	ret = device->write(device->address << 1, cfg, 3);
	return ret;
}


uint8_t ina_init(INA3221_DEVICE *device)
{
	uint8_t ret = -1;
	uint16_t inaConfig = INA3221_CH3_EN | INA3221_CH2_EN | INA3221_CH1_EN; //enable all channels
	inaConfig += INA3221_VBUS_CT2; //1.1 ms bus conversion time
	inaConfig += INA3221_VSH_CT2; // 1.1 ms shunt conversion time
	inaConfig += INA3221_MODE0 | INA3221_MODE1 | INA3221_MODE2; //shunt and bus, continuous
	ret = ina_setReg(device, INA3221_REG_CONFIG, INA3221_RST);
	ret = ina_setReg(device, INA3221_REG_CONFIG, inaConfig);
	return ret;
}


uint16_t ina_getVbus(INA3221_DEVICE *device, INA3221_REG vbus)
{
	return ina_getReg(device, vbus);
}
