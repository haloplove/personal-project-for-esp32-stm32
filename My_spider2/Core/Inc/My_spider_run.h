/*
 * My_spider_run.h
 *
 *  Created on: 2022年4月1日
 *      Author: Admin
 */

#ifndef INC_MY_SPIDER_RUN_H_
#define INC_MY_SPIDER_RUN_H_

#include <pca9685.h>
#include <main.h>

class Manager_spidr
{
private:
public:
	void Manager_spider(I2C_HandleTypeDef &hi2c1);
};

#endif /* INC_MY_SPIDER_RUN_H_ */
