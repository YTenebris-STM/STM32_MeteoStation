#include "i2c.h"

void I2C_Init (void)
{
	// Configure I2C1 for fast-mode communication
	
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

	GPIOB->CRL &= ~(GPIO_CRL_CNF6 | GPIO_CRL_MODE6);
	GPIOB->CRL |= GPIO_CRL_CNF6_0 | GPIO_CRL_CNF6_1 | GPIO_CRL_MODE6;

	GPIOB->CRL &= ~(GPIO_CRL_CNF7 | GPIO_CRL_MODE7);
	GPIOB->CRL |= GPIO_CRL_CNF7_0 | GPIO_CRL_CNF7_1 | GPIO_CRL_MODE7;

	I2C1->CR1 |= I2C_CR1_SWRST;
	I2C1->CR1 &= ~I2C_CR1_SWRST;
	
	// Set the APB1 clock frequency in MHz

	I2C1->CR2 = 0;
	I2C1->CR2 |= 36; 
	
	// Set the I2C clock divider
	
	I2C1->CCR = 0;
	I2C1->CCR |= 45;
	I2C1->CCR |= I2C_CCR_FS;
	
	// Set the maximum rise time for I2C fast mode

	I2C1->TRISE = 12;
	
	I2C1->CR1 |= I2C_CR1_PE;
}


void I2C_Write (uint8_t addr, uint8_t reg, uint8_t data)
{
	// Write one byte to an I2C device register
	
	while (I2C1->SR2 & I2C_SR2_BUSY);

	I2C1->CR1 |= I2C_CR1_START;
	while (!(I2C1->SR1 & I2C_SR1_SB));

	I2C1->DR = (addr << 1); 
	while (!(I2C1->SR1 & I2C_SR1_ADDR));
	(void)I2C1->SR2;

	I2C1->DR = reg;
	while (!(I2C1->SR1 & I2C_SR1_TXE));

	I2C1->DR = data;
	while (!(I2C1->SR1 & I2C_SR1_TXE));

	I2C1->CR1 |= I2C_CR1_STOP;
}


uint8_t I2C_Read (uint8_t dev_addr, uint8_t reg_addr)
{
	// Read one byte from an I2C device register
	
	uint8_t data = 0;
	
	while (I2C1->SR2 & I2C_SR2_BUSY);
	
	I2C1->CR1 |= I2C_CR1_START;
	while (!(I2C1->SR1 & I2C_SR1_SB));
	
	I2C1->DR = (dev_addr << 1) | 0;
	while (!(I2C1->SR1 & I2C_SR1_ADDR));
	(void)I2C1->SR2;
	
	I2C1->DR = reg_addr;
	while (!(I2C1->SR1 & I2C_SR1_TXE));
	
	I2C1->CR1 |= I2C_CR1_START;
	while (!(I2C1->SR1 & I2C_SR1_SB));
	
	I2C1->DR = (dev_addr << 1) | 1;
	while (!(I2C1->SR1 & I2C_SR1_ADDR));
	(void)I2C1->SR2;
	
	I2C1->CR1 &= ~I2C_CR1_ACK;
	
	while (!(I2C1->SR1 & I2C_SR1_RXNE));
	data = I2C1->DR;
	
	I2C1->CR1 |= I2C_CR1_STOP;
	I2C1->CR1 |= I2C_CR1_ACK;
	
	return data;
}

