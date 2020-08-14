#pragma once

#include "mbed.h"
#include "platform/mbed_critical.h"

#include "stm32f767xx.h"
#include "stm32f7xx_ll_dma2d.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_system.h"
#include "stm32f7xx_ll_exti.h"
#include "stm32f7xx_ll_cortex.h"
#include "stm32f7xx_ll_utils.h"
#include "stm32f7xx_ll_pwr.h"
#include "stm32f7xx_ll_dma.h"
#include "stm32f7xx_ll_tim.h"
#include "stm32f7xx_ll_usart.h"
#include "stm32f7xx.h"
#include "stm32f7xx_ll_gpio.h"

#include "Driver.hpp"




namespace Copter::Engine
{
    class DShot: public Driver
    {
    public:
        /**
         * @brief: Supported GPIO pins for DShot
         */
        enum class Pin
        {
            PB6,
            PB8,
            PC6,
            PC7
        };
        /**
         * @brief: Constructor
         * @param dShotSpeed: Speed of Dshot protocol. Valid values are 150, 300, 600, 1200
         * @param pin: The pin for the protocol to output to
         */
        DShot(int dShotSpeed, Pin pin);
        /**
         * @brief: Default deconstructor
         */
        ~DShot() = default;

        /**
         * @brief: Sets up hardware configuration
         * @return: Whether the hardware was succesfully setup
         */
        bool setup() override;

        /**
         * @brief: Increase the speed by a given amount
         * @param ramp: Value to increase the current speed by
         */
        void incThrottle(const float && ramp) override;

        /**
         * @brief: Decrease the speed by a given amount
         * @param ramp: Value to decrease the current speed by
         */
        void decThrottle(const float && ramp) override;

        /**
         * @brief: Send the DShot signal
         */
        void sendSignal(const units::velocity::speed_t & speed) override;


    private:
        /**
         * @brief: Base structure for hardware configuration
         */
        struct Config
        {
            TIM_TypeDef* tim;
            uint32_t timChannel;

            DMA_TypeDef* dma;
            uint32_t stream;
            uint32_t address;
            IRQn_Type irqn;
            void (*enDMAReq)(TIM_TypeDef*);
            uint32_t dmaChannel;
            uint32_t dmaInit;

            GPIO_TypeDef* gpioReg;
            uint32_t pin;
            uint32_t alternate;
            uint32_t gpioInit;
        };

        /**
         * @brief: Get the hardware configurations associated with the given pin
         * @todo: Look at removing function in favour of build system determining config
         * @return Config struct of hardware configurations for a given Pin
         */
        Config getConfig(Pin);

        /**
         * @brief: Variables
         */
        const int mDShotSpeed;
        float mSpeed;

        uint16_t mDShotValues = 0;
        uint8_t mDSRequestTLM = 0;
        uint32_t mDSCounterTLM = 0;

        uint8_t mDShotBitWidth = 0;
        uint8_t mDShotOne = 0;
        uint8_t mDShotZero = 0;
        uint16_t mUseDShotPrescaler = 1;
        __IO uint16_t mDSBufferPWM[18];

        Pin mPin;
        DMA_TypeDef* mDMA;
        uint32_t mStream;
    };
}





