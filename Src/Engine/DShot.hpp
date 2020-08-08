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
        explicit DShot(int dShotSpeed);
        ~DShot() = default;

        bool setup() override;
        void incThrottle() override;
        void decThrottle() override;
        void sendSignal();// dont know if needed

    private:
        int mDShotSpeed;

//        uint16_t DshotValues[4] = {0,0,0,0};
//        uint8_t DS_request_TLM[4] = {0,0,0,0};
//        uint32_t DS_counter_TLM = 0;
//
//        uint8_t DshotBitWidth = 0;
//        uint8_t DshotOne = 0;
//        uint8_t DshotZero = 0;
//        uint16_t UseDshotPrescaler = 1;
//        __IO uint16_t DSBufferPWM[4][18];
    };
}





