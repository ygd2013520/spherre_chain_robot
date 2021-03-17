#include "main.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "boat_demo_fiscobcos.h"
#include <iothub_device_client_ll.h>
#include <iothub_client_options.h>
#include <iothubtransportmqtt.h>
#include <iothub.h>

static int init(void);
static bool deinit(int);
extern void boat_demo_entry(void);

int main(int argc, char* argv[])
{
    BoatLog(BOAT_LOG_NORMAL, "INFO: Application starting.\n");

    BoatLog(BOAT_LOG_NORMAL, "INFO: Application init.\n");
    int err = init();
    

    int ret  = connect_wifi_detection();
    if (ret < 0)
    {
         BoatLog(BOAT_LOG_NORMAL, "JELLY:  connect_wifi ERROR.\n");
        return 1;
    }
    
    create_tcp_server();
    boat_demo_entry();

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
