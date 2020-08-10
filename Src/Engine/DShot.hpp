#pragma once
//#include <cstdint>

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
         * Supported GPIO pins for DShot
         */
        enum class Pin
        {
            PB6,
            PB8,
            PC6,
            PC7
        };
        /**
         * Constructor
         * @param dShotSpeed: Speed of Dshot protocol. Valid values are 150, 300, 600, 1200
         * @param pin: The pin for the protocol to output to
         */
        DShot(int dShotSpeed, Pin pin);
        /**
         * Default deconstructor
         */
        ~DShot() = default;

        /**
         * Sets up hardware configuration
         * @return
         */
        bool setup() override;

        /**
         * Increase the speed by a given amount
         * @param ramp: Value to increase the current speed by
         */
        void incThrottle(const float && ramp) override;

        /**
         * Decrease the speed by a given amount
         * @param ramp: Value to decrease the current speed by
         */
        void decThrottle() override;

        /**
         * Send the DShot signal
         */
        void sendSignal();



    private:
        /**
         * Base structure for hardware configuration
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

            GPIO_TypeDef* gpioReg;
            uint32_t pin;
            uint32_t alternate;
        };

        /**
         * Get the hardware configurations associated with the given pin
         * @todo: Look at removing function in favour of build system determining config
         * @return Config struct of hardware configurations for a given Pin
         */
        Config getConfig(Pin);

        /**
         * Variables
         */
        const int mDShotSpeed;
        float mSpeed;

        uint16_t DshotValues = 0;
        uint8_t DS_request_TLM = 0;
        uint32_t DS_counter_TLM = 0;

        uint8_t DshotBitWidth = 0;
        uint8_t DshotOne = 0;
        uint8_t DshotZero = 0;
        uint16_t UseDshotPrescaler = 1;
        __IO uint16_t DSBufferPWM[1][18];

        DMA_TypeDef* mDMA;
        uint32_t mStream;
        Pin mPin;
    };
}





