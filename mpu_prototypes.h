/*
 * FreeRTOS Kernel V10.2.1
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/FreeRTOS
 *
 * 1 tab == 4 spaces!
 */

/*
 * When the MPU is used the standard (non MPU) API functions are mapped to
 * equivalents that start "MPU_", the prototypes for which are defined in this
 * header files.  This will cause the application code to call the MPU_ version
 * which wraps the non-MPU version with privilege promoting then demoting code,
 * so the kernel code always runs will full privileges.
 */


#ifndef MPU_PROTOTYPES_H
#define MPU_PROTOTYPES_H

/* MPU versions of tasks.h API functions. */
BaseType_t MPU_xTaskCreate( TaskFunction_t pxTaskCode, const char * const pcName, const uint16_t usStackDepth, void * const pvParameters, UBaseType_t uxPriority, TaskHandle_t * const pxCreatedTask ) FreeRTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskCreateStatic( TaskFunction_t pxTaskCode, const char * const pcName, const uint32_t ulStackDepth, void * const pvParameters, UBaseType_t uxPriority, StackType_t * const puxStackBuffer, StaticTask_t * const pxTaskBuffer ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskCreateRestricted( const TaskParameters_t * const pxTaskDefinition, TaskHandle_t *pxCreatedTask ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskCreateRestrictedStatic( const TaskParameters_t * const pxTaskDefinition, TaskHandle_t *pxCreatedTask ) FreeRTOS_SYSTEM_CALL;
void MPU_vTaskAllocateMPURegions( TaskHandle_t xTask, const MemoryRegion_t * const pxRegions ) FreeRTOS_SYSTEM_CALL;
void MPU_vTaskDelete( TaskHandle_t xTaskToDelete ) FreeRTOS_SYSTEM_CALL;
void MPU_vTaskDelay( const TickType_t xTicksToDelay ) FreeRTOS_SYSTEM_CALL;
void MPU_vTaskDelayUntil( TickType_t * const pxPreviousWakeTime, const TickType_t xTimeIncrement ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskAbortDelay( TaskHandle_t xTask ) FreeRTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskPriorityGet( const TaskHandle_t xTask ) FreeRTOS_SYSTEM_CALL;
eTaskState MPU_eTaskGetState( TaskHandle_t xTask ) FreeRTOS_SYSTEM_CALL;
void MPU_vTaskGetInfo( TaskHandle_t xTask, TaskStatus_t *pxTaskStatus, BaseType_t xGetFreeStackSpace, eTaskState eState ) FreeRTOS_SYSTEM_CALL;
void MPU_vTaskPrioritySet( TaskHandle_t xTask, UBaseType_t uxNewPriority ) FreeRTOS_SYSTEM_CALL;
void MPU_vTaskSuspend( TaskHandle_t xTaskToSuspend ) FreeRTOS_SYSTEM_CALL;
void MPU_vTaskResume( TaskHandle_t xTaskToResume ) FreeRTOS_SYSTEM_CALL;
void MPU_vTaskStartScheduler( void ) FreeRTOS_SYSTEM_CALL;
void MPU_vTaskSuspendAll( void ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskResumeAll( void ) FreeRTOS_SYSTEM_CALL;
TickType_t MPU_xTaskGetTickCount( void ) FreeRTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskGetNumberOfTasks( void ) FreeRTOS_SYSTEM_CALL;
char * MPU_pcTaskGetName( TaskHandle_t xTaskToQuery ) FreeRTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetHandle( const char *pcNameToQuery ) FreeRTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskGetStackHighWaterMark( TaskHandle_t xTask ) FreeRTOS_SYSTEM_CALL;
configSTACK_DEPTH_TYPE MPU_uxTaskGetStackHighWaterMark2( TaskHandle_t xTask ) FreeRTOS_SYSTEM_CALL;
void MPU_vTaskSetApplicationTaskTag( TaskHandle_t xTask, TaskHookFunction_t pxHookFunction ) FreeRTOS_SYSTEM_CALL;
TaskHookFunction_t MPU_xTaskGetApplicationTaskTag( TaskHandle_t xTask ) FreeRTOS_SYSTEM_CALL;
void MPU_vTaskSetThreadLocalStoragePointer( TaskHandle_t xTaskToSet, BaseType_t xIndex, void *pvValue ) FreeRTOS_SYSTEM_CALL;
void * MPU_pvTaskGetThreadLocalStoragePointer( TaskHandle_t xTaskToQuery, BaseType_t xIndex ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskCallApplicationTaskHook( TaskHandle_t xTask, void *pvParameter ) FreeRTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetIdleTaskHandle( void ) FreeRTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskGetSystemState( TaskStatus_t * const pxTaskStatusArray, const UBaseType_t uxArraySize, uint32_t * const pulTotalRunTime ) FreeRTOS_SYSTEM_CALL;
TickType_t MPU_xTaskGetIdleRunTimeCounter( void ) FreeRTOS_SYSTEM_CALL;
void MPU_vTaskList( char * pcWriteBuffer ) FreeRTOS_SYSTEM_CALL;
void MPU_vTaskGetRunTimeStats( char *pcWriteBuffer ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotify( TaskHandle_t xTaskToNotify, uint32_t ulValue, eNotifyAction eAction, uint32_t *pulPreviousNotificationValue ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskNotifyWait( uint32_t ulBitsToClearOnEntry, uint32_t ulBitsToClearOnExit, uint32_t *pulNotificationValue, TickType_t xTicksToWait ) FreeRTOS_SYSTEM_CALL;
uint32_t MPU_ulTaskNotifyTake( BaseType_t xClearCountOnExit, TickType_t xTicksToWait ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskNotifyStateClear( TaskHandle_t xTask ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskIncrementTick( void ) FreeRTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetCurrentTaskHandle( void ) FreeRTOS_SYSTEM_CALL;
void MPU_vTaskSetTimeOutState( TimeOut_t * const pxTimeOut ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskCheckForTimeOut( TimeOut_t * const pxTimeOut, TickType_t * const pxTicksToWait ) FreeRTOS_SYSTEM_CALL;
void MPU_vTaskMissedYield( void ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGetSchedulerState( void ) FreeRTOS_SYSTEM_CALL;

/* MPU versions of queue.h API functions. */
BaseType_t MPU_xQueueGenericSend( QueueHandle_t xQueue, const void * const pvItemToQueue, TickType_t xTicksToWait, const BaseType_t xCopyPosition ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueReceive( QueueHandle_t xQueue, void * const pvBuffer, TickType_t xTicksToWait ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xQueuePeek( QueueHandle_t xQueue, void * const pvBuffer, TickType_t xTicksToWait ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueSemaphoreTake( QueueHandle_t xQueue, TickType_t xTicksToWait ) FreeRTOS_SYSTEM_CALL;
UBaseType_t MPU_uxQueueMessagesWaiting( const QueueHandle_t xQueue ) FreeRTOS_SYSTEM_CALL;
UBaseType_t MPU_uxQueueSpacesAvailable( const QueueHandle_t xQueue ) FreeRTOS_SYSTEM_CALL;
void MPU_vQueueDelete( QueueHandle_t xQueue ) FreeRTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueCreateMutex( const uint8_t ucQueueType ) FreeRTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueCreateMutexStatic( const uint8_t ucQueueType, StaticQueue_t *pxStaticQueue ) FreeRTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueCreateCountingSemaphore( const UBaseType_t uxMaxCount, const UBaseType_t uxInitialCount ) FreeRTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueCreateCountingSemaphoreStatic( const UBaseType_t uxMaxCount, const UBaseType_t uxInitialCount, StaticQueue_t *pxStaticQueue ) FreeRTOS_SYSTEM_CALL;
TaskHandle_t MPU_xQueueGetMutexHolder( QueueHandle_t xSemaphore ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueTakeMutexRecursive( QueueHandle_t xMutex, TickType_t xTicksToWait ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueGiveMutexRecursive( QueueHandle_t pxMutex ) FreeRTOS_SYSTEM_CALL;
void MPU_vQueueAddToRegistry( QueueHandle_t xQueue, const char *pcName ) FreeRTOS_SYSTEM_CALL;
void MPU_vQueueUnregisterQueue( QueueHandle_t xQueue ) FreeRTOS_SYSTEM_CALL;
const char * MPU_pcQueueGetName( QueueHandle_t xQueue ) FreeRTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueGenericCreate( const UBaseType_t uxQueueLength, const UBaseType_t uxItemSize, const uint8_t ucQueueType ) FreeRTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueGenericCreateStatic( const UBaseType_t uxQueueLength, const UBaseType_t uxItemSize, uint8_t *pucQueueStorage, StaticQueue_t *pxStaticQueue, const uint8_t ucQueueType ) FreeRTOS_SYSTEM_CALL;
QueueSetHandle_t MPU_xQueueCreateSet( const UBaseType_t uxEventQueueLength ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueAddToSet( QueueSetMemberHandle_t xQueueOrSemaphore, QueueSetHandle_t xQueueSet ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueRemoveFromSet( QueueSetMemberHandle_t xQueueOrSemaphore, QueueSetHandle_t xQueueSet ) FreeRTOS_SYSTEM_CALL;
QueueSetMemberHandle_t MPU_xQueueSelectFromSet( QueueSetHandle_t xQueueSet, const TickType_t xTicksToWait ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueGenericReset( QueueHandle_t xQueue, BaseType_t xNewQueue ) FreeRTOS_SYSTEM_CALL;
void MPU_vQueueSetQueueNumber( QueueHandle_t xQueue, UBaseType_t uxQueueNumber ) FreeRTOS_SYSTEM_CALL;
UBaseType_t MPU_uxQueueGetQueueNumber( QueueHandle_t xQueue ) FreeRTOS_SYSTEM_CALL;
uint8_t MPU_ucQueueGetQueueType( QueueHandle_t xQueue ) FreeRTOS_SYSTEM_CALL;

/* MPU versions of timers.h API functions. */
TimerHandle_t MPU_xTimerCreate( const char * const pcTimerName, const TickType_t xTimerPeriodInTicks, const UBaseType_t uxAutoReload, void * const pvTimerID, TimerCallbackFunction_t pxCallbackFunction ) FreeRTOS_SYSTEM_CALL;
TimerHandle_t MPU_xTimerCreateStatic( const char * const pcTimerName, const TickType_t xTimerPeriodInTicks, const UBaseType_t uxAutoReload, void * const pvTimerID, TimerCallbackFunction_t pxCallbackFunction, StaticTimer_t *pxTimerBuffer ) FreeRTOS_SYSTEM_CALL;
void * MPU_pvTimerGetTimerID( const TimerHandle_t xTimer ) FreeRTOS_SYSTEM_CALL;
void MPU_vTimerSetTimerID( TimerHandle_t xTimer, void *pvNewID ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerIsTimerActive( TimerHandle_t xTimer ) FreeRTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTimerGetTimerDaemonTaskHandle( void ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerPendFunctionCall( PendedFunction_t xFunctionToPend, void *pvParameter1, uint32_t ulParameter2, TickType_t xTicksToWait ) FreeRTOS_SYSTEM_CALL;
const char * MPU_pcTimerGetName( TimerHandle_t xTimer ) FreeRTOS_SYSTEM_CALL;
void MPU_vTimerSetReloadMode( TimerHandle_t xTimer, const UBaseType_t uxAutoReload ) FreeRTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetPeriod( TimerHandle_t xTimer ) FreeRTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetExpiryTime( TimerHandle_t xTimer ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerCreateTimerTask( void ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerGenericCommand( TimerHandle_t xTimer, const BaseType_t xCommandID, const TickType_t xOptionalValue, BaseType_t * const pxHigherPriorityTaskWoken, const TickType_t xTicksToWait ) FreeRTOS_SYSTEM_CALL;

/* MPU versions of event_group.h API functions. */
EventGroupHandle_t MPU_xEventGroupCreate( void ) FreeRTOS_SYSTEM_CALL;
EventGroupHandle_t MPU_xEventGroupCreateStatic( StaticEventGroup_t *pxEventGroupBuffer ) FreeRTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupWaitBits( EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToWaitFor, const BaseType_t xClearOnExit, const BaseType_t xWaitForAllBits, TickType_t xTicksToWait ) FreeRTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupClearBits( EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToClear ) FreeRTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupSetBits( EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToSet ) FreeRTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupSync( EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToSet, const EventBits_t uxBitsToWaitFor, TickType_t xTicksToWait ) FreeRTOS_SYSTEM_CALL;
void MPU_vEventGroupDelete( EventGroupHandle_t xEventGroup ) FreeRTOS_SYSTEM_CALL;
UBaseType_t MPU_uxEventGroupGetNumber( void* xEventGroup ) FreeRTOS_SYSTEM_CALL;

/* MPU versions of message/stream_buffer.h API functions. */
size_t MPU_xStreamBufferSend( StreamBufferHandle_t xStreamBuffer, const void *pvTxData, size_t xDataLengthBytes, TickType_t xTicksToWait ) FreeRTOS_SYSTEM_CALL;
size_t MPU_xStreamBufferReceive( StreamBufferHandle_t xStreamBuffer, void *pvRxData, size_t xBufferLengthBytes, TickType_t xTicksToWait ) FreeRTOS_SYSTEM_CALL;
size_t MPU_xStreamBufferNextMessageLengthBytes( StreamBufferHandle_t xStreamBuffer ) FreeRTOS_SYSTEM_CALL;
void MPU_vStreamBufferDelete( StreamBufferHandle_t xStreamBuffer ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xStreamBufferIsFull( StreamBufferHandle_t xStreamBuffer ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xStreamBufferIsEmpty( StreamBufferHandle_t xStreamBuffer ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xStreamBufferReset( StreamBufferHandle_t xStreamBuffer ) FreeRTOS_SYSTEM_CALL;
size_t MPU_xStreamBufferSpacesAvailable( StreamBufferHandle_t xStreamBuffer ) FreeRTOS_SYSTEM_CALL;
size_t MPU_xStreamBufferBytesAvailable( StreamBufferHandle_t xStreamBuffer ) FreeRTOS_SYSTEM_CALL;
BaseType_t MPU_xStreamBufferSetTriggerLevel( StreamBufferHandle_t xStreamBuffer, size_t xTriggerLevel ) FreeRTOS_SYSTEM_CALL;
StreamBufferHandle_t MPU_xStreamBufferGenericCreate( size_t xBufferSizeBytes, size_t xTriggerLevelBytes, BaseType_t xIsMessageBuffer ) FreeRTOS_SYSTEM_CALL;
StreamBufferHandle_t MPU_xStreamBufferGenericCreateStatic( size_t xBufferSizeBytes, size_t xTriggerLevelBytes, BaseType_t xIsMessageBuffer, uint8_t * const pucStreamBufferStorageArea, StaticStreamBuffer_t * const pxStaticStreamBuffer ) FreeRTOS_SYSTEM_CALL;



#endif /* MPU_PROTOTYPES_H */

