.syntax unified
.thumb

.global Reset_Handler
.type Reset_Handler, %function
Reset_Handler:

init_data:
    ldr r2, =_start_init_data
    ldr r3, =_start_data
    ldr r1, =_end_data

init_data_loop:
    cmp r3, r1
    bhs zero_bss
    ldr r0, [r2]
    str r0, [r3]
    adds r2, r2, #4
    adds r3, r3, #4
    b init_data_loop

zero_bss:
    ldr r3, =_start_bss
    ldr r1, =_end_bss
    movs r0, #0

zero_bss_loop:
    cmp r3, r1
    bhs init_finished
    str r0, [r3]
    adds r3, r3, #4
    b zero_bss_loop

init_finished:
    bl SystemInit
    bl __libc_init_array
    bl main

infinite_loop:
    b infinite_loop

.size Reset_Handler, .-Reset_Handler

.section .vectors, "a"
.word _end_stack
.word Reset_Handler
.word NMI_Handler
.word HardFault_Handler
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word SVC_Handler
.word 0
.word 0
.word PendSV_Handler
.word SysTick_Handler
.word WWDG_IRQHandler
.word 0
.word RTC_IRQHandler
.word FLASH_IRQHandler
.word RCC_IRQHandler
.word EXTI0_1_IRQHandler
.word EXTI2_3_IRQHandler
.word EXTI4_15_IRQHandler
.word 0
.word DMA1_Channel1_IRQHandler
.word DMA1_Channel2_3_IRQHandler
.word DMA1_Channel4_5_IRQHandler
.word ADC1_IRQHandler
.word TIM1_BRK_UP_TRG_COM_IRQHandler
.word TIM1_CC_IRQHandler
.word 0
.word TIM3_IRQHandler
.word 0
.word 0
.word TIM14_IRQHandler
.word 0
.word TIM16_IRQHandler
.word TIM17_IRQHandler
.word I2C1_IRQHandler
.word 0
.word SPI1_IRQHandler
.word 0
.word USART1_IRQHandler
.word 0
.word 0
.word 0
.word 0

.type Dummy_Handler, %function
Dummy_Handler:
    b Dummy_Handler

.weak NMI_Handler
.thumb_set NMI_Handler, Dummy_Handler

.weak HardFault_Handler
.thumb_set HardFault_Handler, Dummy_Handler

.weak SVC_Handler
.thumb_set SVC_Handler, Dummy_Handler

.weak PendSV_Handler
.thumb_set PendSV_Handler, Dummy_Handler

.weak SysTick_Handler
.thumb_set SysTick_Handler, Dummy_Handler

.weak WWDG_IRQHandler
.thumb_set WWDG_IRQHandler, Dummy_Handler

.weak RTC_IRQHandler
.thumb_set RTC_IRQHandler, Dummy_Handler

.weak FLASH_IRQHandler
.thumb_set FLASH_IRQHandler, Dummy_Handler

.weak RCC_IRQHandler
.thumb_set RCC_IRQHandler, Dummy_Handler

.weak EXTI0_1_IRQHandler
.thumb_set EXTI0_1_IRQHandler, Dummy_Handler

.weak EXTI2_3_IRQHandler
.thumb_set EXTI2_3_IRQHandler, Dummy_Handler

.weak EXTI4_15_IRQHandler
.thumb_set EXTI4_15_IRQHandler, Dummy_Handler

.weak DMA1_Channel1_IRQHandler
.thumb_set DMA1_Channel1_IRQHandler, Dummy_Handler

.weak DMA1_Channel2_3_IRQHandler
.thumb_set DMA1_Channel2_3_IRQHandler, Dummy_Handler

.weak DMA1_Channel4_5_IRQHandler
.thumb_set DMA1_Channel4_5_IRQHandler, Dummy_Handler

.weak ADC1_IRQHandler
.thumb_set ADC1_IRQHandler, Dummy_Handler

.weak TIM1_BRK_UP_TRG_COM_IRQHandler
.thumb_set TIM1_BRK_UP_TRG_COM_IRQHandler, Dummy_Handler

.weak TIM1_CC_IRQHandler
.thumb_set TIM1_CC_IRQHandler, Dummy_Handler

.weak TIM3_IRQHandler
.thumb_set TIM3_IRQHandler, Dummy_Handler

.weak TIM14_IRQHandler
.thumb_set TIM14_IRQHandler, Dummy_Handler

.weak TIM16_IRQHandler
.thumb_set TIM16_IRQHandler, Dummy_Handler

.weak TIM17_IRQHandler
.thumb_set TIM17_IRQHandler, Dummy_Handler

.weak I2C1_IRQHandler
.thumb_set I2C1_IRQHandler, Dummy_Handler

.weak SPI1_IRQHandler
.thumb_set SPI1_IRQHandler, Dummy_Handler

.weak USART1_IRQHandler
.thumb_set USART1_IRQHandler, Dummy_Handler
