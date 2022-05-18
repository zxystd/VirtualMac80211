

#include "IO80211Interface.hpp"
#include <sys/errno.h>
#include "AirportIOCTL.h"

#define super IOEthernetInterface
OSDefineMetaClassAndStructors( IO80211Interface, IOEthernetInterface )

bool IO80211Interface::init(IONetworkController *controller)
{
    VMLog("%s\n", __FUNCTION__);
    if ( super::init(controller) == false )
        return false;
    setInterfaceSubType(3);
    return true;
}

void IO80211Interface::free()
{
    VMLog("%s\n", __FUNCTION__);
    super::free();
}

SInt32 IO80211Interface::performCommand( IONetworkController * controller,
                                           unsigned long         cmd,
                                           void *                arg0,
                                           void *                arg1 )
{
//    VMLog("%s cmd=%lu, arg0=%p, arg1=%p\n", __FUNCTION__, cmd, arg0, arg1);
    uint64_t v7;
    
    if ( controller == NULL ) return EINVAL;
    
    if (cmd <= 3223349704LL) {
        if (cmd != 2149607880LL) {
            v7 = 2150132168LL;
            goto label_6;
        }
        return controller->executeCommand(
                                          this,
                                          (IONetworkController::Action)
                                          &IO80211Interface::performGatedCommand,
                                          this,
                                          controller,
                                          &cmd,
                                          arg0,
                                          arg1 );
    }
    if (cmd == 3223873993LL) {
        return controller->executeCommand(
                                          this,
                                          (IONetworkController::Action)
                                          &IO80211Interface::performGatedCommand,
                                          this,
                                          controller,
                                          &cmd,
                                          arg0,
                                          arg1 );
    }
    v7 = 3223349705LL;
label_6:
    if (cmd == v7) {
        return controller->executeCommand(
                                          this,
                                          (IONetworkController::Action)
                                          &IO80211Interface::performGatedCommand,
                                          this,
                                          controller,
                                          &cmd,
                                          arg0,
                                          arg1 );
    }
    return super::performCommand(controller, cmd, arg0, arg1);
}

UInt64 IO80211Interface::IO80211InterfaceUserSpaceToKernelApple80211Request(void *arg, apple80211req *req, unsigned long ctl)
{
    uint8_t *a1 = (uint8_t *)arg;
    uint8_t *req_ptr = (uint8_t*)req;
    if (a1 == NULL) {
        VMLog("%s a1==NULL\n", __FUNCTION__);
        return 0;
    }
    *(uint64_t*)(req_ptr + 8) = *((uint64_t*)((uint8_t*)a1 + 8));
    *(uint64_t*)req_ptr = *(uint64_t*)a1;
    *(uint32_t*)(req_ptr + 0x10) = *((uint32_t*)((uint8_t*)a1 + 0x10));
    *(uint32_t*)(req_ptr + 0x14) = *((uint32_t*)((uint8_t*)a1 + 0x14));
    *(uint32_t*)(req_ptr + 0x18) = *((uint32_t*)((uint8_t*)a1 + 0x18));
    if ( ctl != 3223873993LL && ctl != 2150132168LL ) {
        req->req_data = (void *)(uint64_t)(*((uint32_t*)((uint8_t*)a1 + 0x1C)));
        req->req_flag = 4;
    } else {
        req->req_data = (void *)*((uint64_t*)((uint8_t*)a1 + 0x20));
        req->req_flag = 8;
    }
//    VMLog("%s %s req_len=%d req_val=%d req_type=%d req_flag=%d\n", __FUNCTION__, req->req_if_name, req->req_len, req->req_val, req->req_type, req->req_flag);
    return (UInt64)req->req_data;
}

int IO80211Interface::performGatedCommand(void * target,
                                             void * arg1_ctr,
                                             void * arg2_cmd,
                                             void * arg3_0,
                                             void * arg4_1)
{
//    VMLog("%s\n", __FUNCTION__);
    apple80211req req;
    UInt64 method;
    IO80211Interface *that = (IO80211Interface *)target;
    if (!arg1_ctr) {
        return 22LL;
    }
    UInt64 cmd = *(UInt64 *)arg2_cmd;
    bzero(&req, sizeof(apple80211req));
    that->IO80211InterfaceUserSpaceToKernelApple80211Request(arg4_1, &req, cmd);
    if ((cmd | 0x80000) == 2150132168LL) {
        method = IOCTL_SET;
    } else {
        method = IOCTL_GET;
    }
    return that->apple80211_ioctl(that, method, &req);
}

int IO80211Interface::apple80211_ioctl(IO80211Interface *netif, UInt64 method, apple80211req *a6)
{
//    VMLog("%s\n", __FUNCTION__);
    if (method == IOCTL_GET) {
        return apple80211_ioctl_get(netif, a6);
    } else {
        return apple80211_ioctl_set(netif, a6);
    }
}

int IO80211Interface::apple80211_ioctl_get(IO80211Interface *netif, apple80211req *req)
{
    int ret;
    uint32_t index = req->req_type - 1;
//    VMLog("%s %d\n", __FUNCTION__, index);
    if (index > 0x160) {
        return 102;
    }
    ret = 102;
    switch (index) {
        case 0://ssid
            ret = sGetSSID(this, req);
            break;
        case 1://auth type
            ret = sGetAuthType(this, req);
            break;
        case 3://channel
            ret = sGetChannel(this, req);
            break;
        case 4://powersave
            ret = sGetPowerSave(this, req);
            break;
        case 5://protmode
            ret = sGetProtMode(this, req);
            break;
        case 6://tx power
            ret = sGetTxPower(this, req);
            break;
        case 7://rate
            ret = sGetRate(this, req);
            break;
        case 8://bssid
            ret = sGetBSSID(this, req);
            break;
//        case 10://scan result
//
//            break;
        case 11://card capa
            ret = sGetCardCapa(this, req);
            break;
        case 12://state req
            ret = sGetState(this, req);
            break;
        case 13://phy mode
            ret = sGetPhyMode(this, req);
            break;
        case 14://op mode
            ret = sGetOpMode(this, req);
            break;
        case 15://rssi
            ret = sGetRSSI(this, req);
            break;
        case 16://noise
            ret = sGetNoise(this, req);
            break;
        case 17://int mit
            ret = sGetIntMit(this, req);
            break;
        case 18://power
            ret = sGetPOWER(this, req);
            break;
//        case 20://association result
//
//            break;
        case 26://supported channels
            ret = sGetSupportedChannels(this, req);
            break;
        case 42:
            ret = sGetVersion(this, req);
            break;
        case 50://country code
            ret = sGetCountryCode(this, req);
            break;
        case 56://mcs
            ret = sGetMCS(this, req);
            break;
        case 95://vir if role
            ret = 6;
            break;
        case 215://roam profile
            
            break;
        case 253:
            ret = sGetHWSupportedChannels(this, req);
            break;
        case 352:
            ret = 6;
            break;
            
        default:
            VMLog("%s unhandle ioctl index=%d\n", __FUNCTION__, index);
            break;
    }
    return ret;
}

int IO80211Interface::apple80211_ioctl_set(IO80211Interface *netif, apple80211req *req)
{
    int ret;
    uint32_t index = req->req_type - 1;
//    VMLog("%s %d\n", __FUNCTION__, index);
    if (index > 0x160) {
        return 102;
    }
    ret = 102;
    switch (index) {
        case 0:
            
            break;
        case 9: //scan req
            ret = sSetScanRequest(this, req);
            break;
        case 18://power
            ret = sSetPOWER(this, req);
            break;
        case 215://roam profile
            ret = kIOReturnSuccess;
            break;
            
        default:
            VMLog("%s unhandle ioctl index=%d\n", __FUNCTION__, index);
            break;
    }
    return ret;
}
