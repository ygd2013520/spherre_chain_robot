/******************************************************************************
Copyright (C) 2018-2020 aitos.io

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#define  USE_CREATE_PERSIST_WALLET //if expect create a persist wallet, uncomment this definition
//#define  USE_LOAD_PERSIST_WALLET   // if expect load a persist wallet, uncomment this definition


#include "boatiotsdk.h"
#include "my_contract.h"
#include "boat_demo_fiscobcos.h"
#include <pthread.h>
#include <applibs/wificonfig.h>
#include <applibs/log.h>
#include <string.h>

static uint8_t mSsid[WIFICONFIG_SSID_MAX_LENGTH];
static char mPsk[WIFICONFIG_WPA2_KEY_MAX_BUFFER_SIZE];

BoatFiscobcosWallet *g_fiscobcos_wallet_ptr;

static int mWifiId = -1;

int boat_connect_wifi(const uint8_t* ssid, const char* psk)
{
	if (WifiConfig_ForgetAllNetworks() < 0)
    {
		BoatLog(BOAT_LOG_NORMAL, "ERROR: WifiConfig_ForgetAllNetworks failed: %s (%d).\n", strerror(errno), errno);
	}
	else
    {
		mWifiId = WifiConfig_AddNetwork();
		if (mWifiId < 0)
        {
			BoatLog(BOAT_LOG_CRITICAL, "ERROR: WifiConfig_AddNetwork failed: %s (%d).\n", strerror(errno), errno);
		}
		else if (WifiConfig_SetSecurityType(mWifiId, WifiConfig_Security_Wpa2_Psk) < 0)
        {
			BoatLog(BOAT_LOG_CRITICAL, "ERROR: WifiConfig_SetSecurityType failed: %s (%d).\n", strerror(errno), errno);
		}
		else if (WifiConfig_SetPSK(mWifiId, psk, strnlen(psk, WIFICONFIG_WPA2_KEY_MAX_BUFFER_SIZE)) < 0)
        {
			BoatLog(BOAT_LOG_CRITICAL, "ERROR: WifiConfig_SetPSK failed: %s (%d).\n", strerror(errno), errno);
		}
		else if (WifiConfig_SetSSID(mWifiId, ssid, strnlen(ssid, WIFICONFIG_SSID_MAX_LENGTH)) < 0)
        {
			BoatLog(BOAT_LOG_CRITICAL, "ERROR: WifiConfig_SetSSID failed: %s (%d).\n", strerror(errno), errno);
		}
		else if (WifiConfig_SetTargetedScanEnabled(mWifiId, true) < 0)
        {
			BoatLog(BOAT_LOG_CRITICAL, "ERROR: WifiConfig_SetTargetedScanEnabled failed: %s (%d).\n", strerror(errno), errno);
		}
		else if (WifiConfig_SetNetworkEnabled(mWifiId, true))
        {
			BoatLog(BOAT_LOG_CRITICAL, "ERROR: WifiConfig_SetNetworkEnabled failed: %s (%d).\n", strerror(errno), errno);
		}
		else
        {
			BoatLog(BOAT_LOG_CRITICAL, "SUCCESS: wifi");
		}
	}
	return mWifiId;
}

int boat_set_wifi_name_passwd(void)
{
    memset(mSsid, 0x00, sizeof(mSsid));
    strncpy(mSsid, "Robot", 5);

    memset(mPsk, 0x00, sizeof(mPsk));
    strncpy(mPsk, "88888888", 8);

    return 0;
}

#if defined( USE_CREATE_PERSIST_WALLET )
__BOATSTATIC BOAT_RESULT fiscobcos_createPersistWallet(BCHAR *wallet_name)
{
    BSINT32 index;
    BoatFiscobcosWalletConfig wallet_config;

	/* wallet_config value assignment */
	strncpy( (char*)wallet_config.prikeyId, 
			 "boatkey/fiscobcos_client_bmm.key", 
			 BOAT_KEYID_MAX_LEN - 1 );
    strncpy( wallet_config.node_url_str, "http://192.168.31.105:8545", BOAT_NODE_URL_MAX_LEN - 1 );

	/* create fiscobcos wallet */
   index = BoatWalletCreate(BOAT_PROTOCOL_FISCOBCOS, wallet_name, &wallet_config, sizeof(BoatFiscobcosWalletConfig));
    //index = BoatWalletCreate(BOAT_PROTOCOL_FISCOBCOS, NULL, &wallet_config, sizeof(BoatFiscobcosWalletConfig));
    if( index == BOAT_ERROR )
	{
        BoatLog(BOAT_LOG_CRITICAL, "create persist wallet failed.");
        return BOAT_ERROR;
    }

    g_fiscobcos_wallet_ptr = BoatGetWalletByIndex( index );

    return BOAT_SUCCESS;
}
#endif

#if defined( USE_LOAD_PERSIST_WALLET )
__BOATSTATIC BOAT_RESULT fiscobcos_loadPersistWallet(BCHAR *wallet_name)
{
	BSINT32 index;

	/* create fiscobcos wallet */
    index = BoatWalletCreate(BOAT_PROTOCOL_FISCOBCOS, wallet_name, NULL, sizeof(BoatFiscobcosWalletConfig));
    if( index == BOAT_ERROR )
	{
        BoatLog(BOAT_LOG_CRITICAL, "load wallet failed.");
        return BOAT_ERROR;
    }
    g_fiscobcos_wallet_ptr = BoatGetWalletByIndex( index );

    return BOAT_SUCCESS;
}
#endif

BOAT_RESULT fiscobcos_helloworld_set(BoatFiscobcosWallet *wallet_ptr, char *chaindata)
{
    BCHAR           *result_str;
    BoatFiscobcosTx tx_ctx;
    BOAT_RESULT     result;
    
    /* Set Contract Address */
    result = BoatFiscobcosTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, 
								 "0x11E1A300", //gasprice
							     "0x33333333", //gaslimit
							     "0xc16e00D176a515F2324D691D91407C4DBd24860c",
								 "0x01", //chainid
								 "0x01"  //groupid
								);
    if( result != BOAT_SUCCESS )
	{
        BoatLog(BOAT_LOG_NORMAL, "BoatFiscobcosTxInit fails.");
        return BOAT_ERROR;
    }
    //BoatLog(BOAT_LOG_NORMAL, "HELLO FISCOBCOS");
    //result_str = my_contract_set(&tx_ctx, "HELLO FISCOBCOS!");
    result_str = Hello_set(&tx_ctx, chaindata);
    if( result_str == NULL )
	{
        BoatLog(BOAT_LOG_NORMAL, "my_contract_set failed: %s.", result_str);
		return BOAT_ERROR;
    }
	BoatLog(BOAT_LOG_NORMAL, "set returns: %s", result_str);
    
    result_str = Hello_get(&tx_ctx);
    if( result_str == NULL )
	{
        BoatLog(BOAT_LOG_NORMAL, "my_contract_get failed: %s.", result_str);
		return BOAT_ERROR;
    }
	BoatLog(BOAT_LOG_NORMAL, "get returns: %s", result_str);

    return BOAT_SUCCESS;
}

BOAT_RESULT fiscobcos_helloworld_verify(BoatFiscobcosWallet *wallet_ptr, char *did)
{
    BCHAR           *result_str;
    BoatFiscobcosTx tx_ctx;
    BOAT_RESULT     result;
    
    /* Set Contract Address */
    result = BoatFiscobcosTxInit(wallet_ptr, &tx_ctx, BOAT_TRUE, 
								 "0x11E1A300", //gasprice
							     "0x33333333", //gaslimit
							     "0xc16e00D176a515F2324D691D91407C4DBd24860c",
								 "0x01", //chainid
								 "0x01"  //groupid
								);
    if( result != BOAT_SUCCESS )
	{
        BoatLog(BOAT_LOG_NORMAL, "BoatFiscobcosTxInit fails.");
        return BOAT_ERROR;
    }
    //BoatLog(BOAT_LOG_NORMAL, "HELLO FISCOBCOS");
    //result_str = my_contract_set(&tx_ctx, "HELLO FISCOBCOS!");
    result_str = Hello_verify(&tx_ctx, did);
    if( result_str == NULL )
	{
        BoatLog(BOAT_LOG_NORMAL, "Hello_verify failed: %s.", result_str);
		return BOAT_ERROR;
    }
	BoatLog(BOAT_LOG_NORMAL, "set returns: %s", result_str);


    return BOAT_SUCCESS;
}

//method: 0: set ,1:verify
void boat_fiscobcos_entry(char *chaindata,int method)
{
	BOAT_RESULT  result  = BOAT_SUCCESS;

	/* step-1: Boat SDK initialization */
    BoatIotSdktInit();
    
	/* step-2: create fiscobcos wallet */
#if defined( USE_CREATE_PERSIST_WALLET )
	BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: create persist wallet.");
	result = fiscobcos_createPersistWallet("boatkey/fiscobcos.cfg");
#elif defined( USE_LOAD_PERSIST_WALLET )
	BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> wallet type: load persist wallet.");
	result = fiscobcos_loadPersistWallet("fiscobcos.cfg");
#else
	BoatLog(BOAT_LOG_NORMAL, ">>>>>>>>>> none wallet type selected.");
	//return -1;
	return ;
#endif	
    if( result != BOAT_SUCCESS )
	{
		 BoatLog(BOAT_LOG_CRITICAL, "fiscobcosWalletPrepare_create failed : %d.", result);
		//return -1;
		return ;
	}
    
	/* step-3: execute 'fiscobcos_call_helloworld' */
    if(method == 0)
    {
        result += fiscobcos_helloworld_set( g_fiscobcos_wallet_ptr ,chaindata);
    }
    else if (method == 1)
    {
        char datastr[512] = {0};
        char datastr_cp[512] = {0};
        sprintf(datastr,"begin verify device id : %s",chaindata);
        fiscobcos_helloworld_set( g_fiscobcos_wallet_ptr ,datastr);
        result  += fiscobcos_helloworld_verify( g_fiscobcos_wallet_ptr ,chaindata);
        sprintf(datastr_cp,"completed verify device id : %s",chaindata);
        fiscobcos_helloworld_set( g_fiscobcos_wallet_ptr ,datastr_cp);
    }
    
	
    if( result != BOAT_SUCCESS )
	{
        BoatLog(BOAT_LOG_NORMAL, "fiscobcos helloworld access Failed: %d.", result);
    }
	else
	{
        BoatLog(BOAT_LOG_NORMAL, "fiscobcos helloworld access Passed.");
    }
	
	/* step-4: Boat SDK Deinitialization */
    BoatIotSdkDeInit();
    
    //return 0;
}

static void* thread_boat_fiscobcos(void* arg)
{
    WifiConfig_ConnectedNetwork network;
    while (1)
    {
        if ( WifiConfig_GetCurrentNetwork(&network) >=0)
        {
            BoatLog(BOAT_LOG_NORMAL, "INFO: SSID \"%.*s\", BSSID %02x:%02x:%02x:%02x:%02x:%02x, Frequency %dMHz, %d dB.\n",
				network.ssidLength, network.ssid, network.bssid[0], network.bssid[1],
				network.bssid[2], network.bssid[3], network.bssid[4], network.bssid[5],
				network.frequencyMHz, network.signalRssi);
            
            BoatSleep(10);
            //boat_fiscobcos_entry("NULL");
            break;
        }
        else
        {
            BoatLog(BOAT_LOG_NORMAL, "wifi cannot connect");
            BoatSleep(1);
        }
    }

    return NULL;
}

void boat_demo_entry(void)
{
    pthread_t th;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int ret = pthread_create(&th, &attr, thread_boat_fiscobcos, NULL);
    if (ret != 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "ERROR: Create thread_boat_fiscobcos error\n");
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "INFO: This is the thread_boat_fiscobcos process.\n");
    }
}


//add by jelly
int connect_wifi_detection(void)
{
    boat_set_wifi_name_passwd();
    int ret = boat_connect_wifi(mSsid, mPsk);
    int num = 0;
    if (ret < 0)
    {
        Log_Debug( "JELLY:  boat_connect_wifi ERROR.\n");
        return ret;
    }

    WifiConfig_ConnectedNetwork network;
    while (1)
    {
        if ( WifiConfig_GetCurrentNetwork(&network) >=0)
        {
            Log_Debug( "JELLY:  connect network OK!!!.\n");
            break;
        }
        else
        {
             Log_Debug("JELLY:can not connect network %d times!!!.\n",num);
             BoatSleep(2);
             num++;
        }
    }
    return 0;
}

#include <sys/socket.h>
#include <netinet/in.h>
#define     MAXRECVLEN       1024
#define     PORT                       4321
#define     BACKLOG                 1

void create_tcp_server(void)
{
    //begin create  tcp server
    char buf[MAXRECVLEN] = {0};
    int listenfd, connectfd;   /* socket descriptors */
    struct sockaddr_in server; /* server's address information */
    struct sockaddr_in client; /* client's address information */
    socklen_t addrlen;
    /* Create TCP socket */
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        /* handle exception */
        Log_Debug( "JELLY:socket() error. Failed to initiate a socket!!!.\n");
        return;
    }
 
    /* set socket option */
    int opt = SO_REUSEADDR;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    //bzero(&server, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listenfd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        /* handle exception */
        Log_Debug( "JELLY: Bind() error.!!!.\n");
        return;
    }
    
    if(listen(listenfd, BACKLOG) == -1)
    {
        Log_Debug( "JELLY: listen() error.!!!.\n");
        return;
    }

    addrlen = sizeof(client);
    while(1){
        Log_Debug("JELLY: waitting for ip connect.....\n");
        if((connectfd=accept(listenfd,(struct sockaddr *)&client, &addrlen))==-1)
           {
             Log_Debug( "JELLY: accept() error.!!!.\n");
            return;
           }

        int iret=-1;
        while(1)
        {
            memset(buf,0,MAXRECVLEN);
            iret = recv(connectfd, buf, MAXRECVLEN, 0);
            if(iret>0)
            {
                Log_Debug( "JELLY GET DATA: %s\n",buf);
                char *delim = "$";
                char *p;
                int timenum = 0;
                p = strtok(buf, delim);
                if (p == NULL)
                {
                    send(connectfd, "wrong format", 12, 0); /* send to the client welcome message */
                }
                
                else if(strncmp(p,"set",3) == 0 && strlen(p) == 3)
                {
                    //chain set
                    p = strtok(NULL, delim);
                    Log_Debug( "JELLY GET CHAIN SET DATA: %s\n",p);
                    boat_fiscobcos_entry(p,0);

                    send(connectfd, "set success", 11, 0); /* send to the client welcome message */

                }
                else if (strncmp(p,"verify",6) == 0 && strlen(p) == 6)
                {
                    //verify data
                    p = strtok(NULL, delim);
                    Log_Debug( "JELLY GET verify DATA: %s\n",p);
                    boat_fiscobcos_entry(p,1);

                    send(connectfd, "verify success", 14, 0); /* send to the client welcome message */
                }
                else
                {
                    //格式错误
                    p= NULL;
                    send(connectfd, "wrong format", 12, 0); /* send to the client welcome message */
                }
            }
            else
            {
                close(connectfd);
                break;
            }
            /* print client's ip and port */
            close(connectfd);
        }
    }
    close(listenfd); /* close listenfd  */ 
    return;
}

