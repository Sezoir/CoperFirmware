#include "DShot.hpp"

namespace Copter::Engine
{
    DShot::DShot(PinName pin, int dShotSpeed)
        : mDShotSpeed(dShotSpeed)
        , mDSBufferPWM{0}
        , mPin(pin)
        , mDMA(nullptr)
        , mStream(0)
    {
    }

    bool DShot::setup()
    {
        printf("dshot setup\n");
        // Get the hardware configuration for pin
        auto config = getConfig(this->mPin);
        this->mDMA = config.dma;
        this->mStream = config.stream;

        // Correctly enable clocks required
        core_util_critical_section_enter();
        LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_FOUR_TIMES);
        LL_AHB1_GRP1_EnableClock(config.dmaInit | config.gpioInit);
        core_util_critical_section_exit();

        core_util_critical_section_enter();
        if(config.tim == TIM4)
            LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
        else if(config.tim == TIM8)
            LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM8);
        core_util_critical_section_exit();

        // Get Dshot setup
        while(((216000 / this->mUseDShotPrescaler) / this->mDShotSpeed) > 0xFF)
            this->mUseDShotPrescaler++;
        this->mDShotBitWidth = (216000 / this->mUseDShotPrescaler) / this->mDShotSpeed;
        this->mDShotOne = (uint8_t)(this->mDShotBitWidth * 0.75f);
        this->mDShotZero = (uint8_t)(this->mDShotOne * 0.5f);

        // Setup timers for DShot
        LL_TIM_InitTypeDef timinit;
        timinit.CounterMode = LL_TIM_COUNTERMODE_UP;
        timinit.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
        timinit.Prescaler = this->mUseDShotPrescaler - 1;
        timinit.Autoreload = 0xFFFF;
        timinit.RepetitionCounter = 0;
        core_util_critical_section_enter();
        LL_TIM_Init(config.tim, &timinit);
        core_util_critical_section_exit();

        LL_TIM_OC_InitTypeDef channelbaseconf;
        channelbaseconf.OCMode = LL_TIM_OCMODE_PWM1;
        channelbaseconf.OCState = LL_TIM_OCSTATE_ENABLE;
        channelbaseconf.OCNState = LL_TIM_OCSTATE_DISABLE;
        channelbaseconf.CompareValue = 0;
        channelbaseconf.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
        channelbaseconf.OCNPolarity = LL_TIM_OCPOLARITY_HIGH;
        channelbaseconf.OCIdleState = LL_TIM_OCIDLESTATE_LOW;
        channelbaseconf.OCNIdleState = LL_TIM_OCIDLESTATE_LOW;

        core_util_critical_section_enter();
        LL_TIM_OC_Init(config.tim, config.timChannel, &channelbaseconf);
        LL_TIM_EnableAllOutputs(config.tim);
        core_util_critical_section_exit();

        // DMA
        LL_DMA_InitTypeDef DMA_InitStructure;
        DMA_InitStructure.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
        DMA_InitStructure.Mode = LL_DMA_MODE_NORMAL;
        DMA_InitStructure.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
        DMA_InitStructure.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
        DMA_InitStructure.NbData = 18;
        DMA_InitStructure.Priority = LL_DMA_PRIORITY_VERYHIGH;
        DMA_InitStructure.FIFOMode = LL_DMA_FIFOMODE_ENABLE;
        DMA_InitStructure.FIFOThreshold = LL_DMA_FIFOTHRESHOLD_1_4;
        DMA_InitStructure.MemBurst = LL_DMA_MBURST_SINGLE;
        DMA_InitStructure.PeriphBurst = LL_DMA_PBURST_SINGLE;
        DMA_InitStructure.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD;
        DMA_InitStructure.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD;

        core_util_critical_section_enter();
        LL_DMA_DeInit(config.dma, config.stream);
        core_util_critical_section_exit();
        DMA_InitStructure.PeriphOrM2MSrcAddress = config.address;
        DMA_InitStructure.Channel = config.dmaChannel;
        DMA_InitStructure.MemoryOrM2MDstAddress = (uint32_t) this->mDSBufferPWM;
        core_util_critical_section_enter();
        LL_DMA_Init(config.dma, config.stream, &DMA_InitStructure);
        core_util_critical_section_exit();

        // Connect compares to DMA
        core_util_critical_section_enter();
        config.enDMAReq(config.tim);
        core_util_critical_section_exit();

        core_util_critical_section_enter();
        LL_TIM_OC_EnablePreload(config.tim, config.timChannel);
        core_util_critical_section_exit();

        // Activate timers
        config.tim->ARR = this->mDShotBitWidth;
        config.tim->CNT = 0;
        config.tim->CR1 |= TIM_CR1_CEN;

        // Enable IRQ's
        core_util_critical_section_enter();
        LL_DMA_EnableIT_TC(config.dma, config.stream);
        core_util_critical_section_exit();

        // @todo: correctly set
        NVIC_SetPriority(config.irqn, 0);
        NVIC_EnableIRQ(config.irqn);

        // Init buffers
        this->mDSBufferPWM[0] = 0;
        this->mDSBufferPWM[17] = 0;

        // Init DShot output GPIO's
        LL_GPIO_InitTypeDef gpioinit;
        gpioinit.Pin = config.pin; // | LL_GPIO_PIN_8;
        gpioinit.Mode = LL_GPIO_MODE_ALTERNATE;
        gpioinit.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
        gpioinit.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
        gpioinit.Pull = LL_GPIO_PULL_NO;
        gpioinit.Alternate = config.alternate;
        core_util_critical_section_enter();
        LL_GPIO_Init(config.gpioReg, &gpioinit);
        core_util_critical_section_exit();

        return true;
    }

    void DShot::sendSignal(const units::velocity::speed_t& speed)
    {
        // Convert speed to a dshot valid speed (that is between 0-2047)
        units::velocity::dshot_t dshotSpeed = speed;
        // Set output value as uint16_t
        this->mDShotValues = dshotSpeed.to<uint16_t>();

        if(this->mDSCounterTLM == 166667)
        {
            this->mDSRequestTLM = 1; // Change DS_requestTLM to 1
            this->mDSCounterTLM = 0;
        }
        else
        {
            this->mDSRequestTLM = 0; // Change DS_requestTLM to 1
            this->mDSCounterTLM++;
        }

        //------------------------------------------------------------------------------------->send 4 dshot signals
        // Gen Dshot bits
        // Telemetry update
        this->mDSBufferPWM[12] = this->mDSRequestTLM ? this->mDShotOne : this->mDShotZero;

        // Checksum
        for(uint8_t j = 1; j < 12; j++)
            this->mDSBufferPWM[j] = (this->mDShotValues >> (11 - j) & 0x1) ? this->mDShotOne : this->mDShotZero;

        this->mDSBufferPWM[13] =
            ((this->mDShotValues >> 10 & 0x01) ^ (this->mDShotValues >> 6 & 0x01) ^ (this->mDShotValues >> 2 & 0x01))
                ? this->mDShotOne
                : this->mDShotZero;
        this->mDSBufferPWM[14] =
            ((this->mDShotValues >> 9 & 0x01) ^ (this->mDShotValues >> 5 & 0x01) ^ (this->mDShotValues >> 1 & 0x01))
                ? this->mDShotOne
                : this->mDShotZero;
        this->mDSBufferPWM[15] =
            ((this->mDShotValues >> 8 & 0x01) ^ (this->mDShotValues >> 4 & 0x01) ^ (this->mDShotValues & 0x01))
                ? this->mDShotOne
                : this->mDShotZero;
        this->mDSBufferPWM[16] =
            ((this->mDShotValues >> 7 & 0x01) ^ (this->mDShotValues >> 3 & 0x01) ^ this->mDSRequestTLM)
                ? this->mDShotOne
                : this->mDShotZero;

        // Send Dshot values
        core_util_critical_section_enter();
        LL_DMA_SetDataLength(this->mDMA, this->mStream, 18);
        LL_DMA_EnableStream(this->mDMA, this->mStream);
        core_util_critical_section_exit();
        //-------------------------------------------------------------------------------------<
    }

    DShot::Config DShot::getConfig(PinName pin)
    {
        // Generate correct config based on pin chosen
        switch(pin)
        {
            case PB_6:
            {
                Config config = {.tim = TIM4,
                                 .timChannel = LL_TIM_CHANNEL_CH1,
                                 .dma = DMA1,
                                 .stream = LL_DMA_STREAM_0,
                                 .address = (uint32_t) &TIM4->CCR1,
                                 .irqn = DMA1_Stream0_IRQn,
                                 .enDMAReq = &LL_TIM_EnableDMAReq_CC1,
                                 .dmaChannel = LL_DMA_CHANNEL_2,
                                 .dmaInit = LL_AHB1_GRP1_PERIPH_DMA1,
                                 .gpioReg = GPIOB,
                                 .pin = LL_GPIO_PIN_6,
                                 .alternate = LL_GPIO_AF_2,
                                 .gpioInit = LL_AHB1_GRP1_PERIPH_GPIOB};
                return config;
            }
            case PB_8:
            {
                Config config = {.tim = TIM4,
                                 .timChannel = LL_TIM_CHANNEL_CH3,
                                 .dma = DMA1,
                                 .stream = LL_DMA_STREAM_7,
                                 .address = (uint32_t) &TIM4->CCR3,
                                 .irqn = DMA1_Stream7_IRQn,
                                 .enDMAReq = &LL_TIM_EnableDMAReq_CC3,
                                 .dmaChannel = LL_DMA_CHANNEL_2,
                                 .dmaInit = LL_AHB1_GRP1_PERIPH_DMA1,
                                 .gpioReg = GPIOB,
                                 .pin = LL_GPIO_PIN_8,
                                 .alternate = LL_GPIO_AF_2,
                                 .gpioInit = LL_AHB1_GRP1_PERIPH_GPIOB};
                return config;
            }
            case PC_6:
            {
                Config config = {.tim = TIM8,
                                 .timChannel = LL_TIM_CHANNEL_CH1,
                                 .dma = DMA2,
                                 .stream = LL_DMA_STREAM_2,
                                 .address = (uint32_t) &TIM8->CCR1,
                                 .irqn = DMA2_Stream2_IRQn,
                                 .enDMAReq = &LL_TIM_EnableDMAReq_CC1,
                                 .dmaChannel = LL_DMA_CHANNEL_7,
                                 .dmaInit = LL_AHB1_GRP1_PERIPH_DMA2,
                                 .gpioReg = GPIOC,
                                 .pin = LL_GPIO_PIN_6,
                                 .alternate = LL_GPIO_AF_3,
                                 .gpioInit = LL_AHB1_GRP1_PERIPH_GPIOC};
                return config;
            }
            case PC_7:
            {
                Config config = {.tim = TIM8,
                                 .timChannel = LL_TIM_CHANNEL_CH2,
                                 .dma = DMA2,
                                 .stream = LL_DMA_STREAM_3,
                                 .address = (uint32_t) &TIM8->CCR2,
                                 .irqn = DMA2_Stream3_IRQn,
                                 .enDMAReq = &LL_TIM_EnableDMAReq_CC2,
                                 .dmaChannel = LL_DMA_CHANNEL_7,
                                 .dmaInit = LL_AHB1_GRP1_PERIPH_DMA2,
                                 .gpioReg = GPIOC,
                                 .pin = LL_GPIO_PIN_7,
                                 .alternate = LL_GPIO_AF_3,
                                 .gpioInit = LL_AHB1_GRP1_PERIPH_GPIOC};
                return config;
            }
            default:
                // If config does not exist for a "supported" pin then generate an error
                MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_APPLICATION, MBED_ERROR_CODE_PINMAP_INVALID),
                           "DShot Pin has no valid configuration");
        }
    }
} // namespace Copter::Engine

extern "C" { // C functions in ll drivers call these.
// DShot transmission complete IRQ's
void DMA1_Stream0_IRQHandler(void)
{
    core_util_critical_section_enter();
    LL_DMA_ClearFlag_TC0(DMA1);
    LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_0);
    core_util_critical_section_exit();
}

void DMA2_Stream2_IRQHandler(void)
{

    core_util_critical_section_enter();
    LL_DMA_ClearFlag_TC2(DMA2);
    LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_2);
    core_util_critical_section_exit();
}

void DMA1_Stream7_IRQHandler(void)
{
    core_util_critical_section_enter();
    LL_DMA_ClearFlag_TC7(DMA1);
    LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_7);
    core_util_critical_section_exit();
}

void DMA2_Stream3_IRQHandler(void)
{
    core_util_critical_section_enter();
    LL_DMA_ClearFlag_TC3(DMA2);
    LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_3);
    core_util_critical_section_exit();
}
}