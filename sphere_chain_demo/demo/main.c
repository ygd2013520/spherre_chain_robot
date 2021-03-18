#include "main.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "boat_demo_fiscobcos.h"
#include <iothub_device_client_ll.h>
#include <iothub_client_options.h>
#include <iothubtransportmqtt.h>
#include <iothub.h>
#include <applibs/gpio.h>
#include <applibs/log.h>

typedef enum {
    ExitCode_Success = 0,
    ExitCode_TermHandler_SigTerm = 1,
    ExitCode_TimerHandler_Consume = 2,
    ExitCode_Init_EventLoop = 3,
    ExitCode_Init_DownloadTimer = 4,
    ExitCode_Main_EventLoopFail = 5,
    ExitCode_InterfaceConnectionStatus_Failed = 6,

    ExitCode_Init_Button = 7,
    ExitCode_Init_ButtonPollTimer = 8,
    ExitCode_ButtonTimer_Consume = 9,
    ExitCode_ButtonTimer_GetButtonState = 10,
    ExitCode_Init_Led = 11,
} ExitCode;

static int Led1GpioFd = -1;


static int init(void);
static bool deinit(int);
extern void boat_demo_entry(void);


static ExitCode InitPeripheralsAndHandlers(void)
{
    //初始化灯sample led
    Led1GpioFd = GPIO_OpenAsOutput(8, GPIO_OutputMode_PushPull, GPIO_Value_High);
    if (Led1GpioFd == -1) {
        Log_Debug("ERROR: Could not open SAMPLE_LED GPIO: %s (%d).\n", strerror(errno), errno);
        return ExitCode_Init_Led;
    }
    return ExitCode_Success;
}


int main(int argc, char* argv[])
{
    BoatLog(BOAT_LOG_NORMAL, "INFO: Application starting.\n");

    BoatLog(BOAT_LOG_NORMAL, "INFO: Application init.\n");
    int err = init();
    InitPeripheralsAndHandlers();
    

    int ret  = connect_wifi_detection();
    //网络正常，点亮灯
    if (ret == 0)
    {
        GPIO_SetValue(Led1GpioFd, GPIO_Value_Low);
    }
    
    if (ret < 0)
    {
         BoatLog(BOAT_LOG_NORMAL, "JELLY:  connect_wifi ERROR.\n");
        return 1;
    }
    
    create_tcp_server();
    //boat_demo_entry();

    if (err < 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "INFO: Application error.\n");
    }
    else
    {
        while (1)
        {
            ;
        }
    }

    while (!deinit(err)) {
        if (DEBUG) BoatLog(BOAT_LOG_NORMAL, "INFO: Application deinit.\n");
    }

    return 0;
}

bool AzureIoT_Initialize(void)
{
    if (IoTHub_Init() != 0) {
        //Log(TAG, "ERROR: failed initializing platform.\n");
        return false;
    }
    return true;
}

/// <summary>
///     Deinitializes the Azure IoT Hub SDK.
/// </summary>
void AzureIoT_Deinitialize(void)
{
    IoTHub_Deinit();
}

static int init(void)
{
    if (!AzureIoT_Initialize())
    {
        BoatLog(BOAT_LOG_NORMAL, "ERROR: Cannot initialize Azure IoT Hub SDK.\n");
        return -1;
    }
    return 1;
}

static bool deinit(int err)
{
    bool fin = true;
    AzureIoT_Deinitialize();
    return fin;
}


/*
 ***************************************************************************
 */
