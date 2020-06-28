

#include "IO80211Interface.hpp"
#include <sys/errno.h>

#define super IOEthernetInterface
OSDefineMetaClassAndStructors( IO80211Interface, IOEthernetInterface )

bool IO80211Interface::init(IONetworkController *controller)
{
    IOLog("%s\n", __FUNCTION__);
    if ( super::init(controller) == false )
        return false;
    setInterfaceSubType(3);
    return true;
}

void IO80211Interface::free()
{
    IOLog("%s\n", __FUNCTION__);
    super::free();
}

SInt32 IO80211Interface::performCommand( IONetworkController * controller,
                                           unsigned long         cmd,
                                           void *                arg0,
                                           void *                arg1 )
{
    IOLog("%s cmd=%lu, arg0=%p, arg1=%p\n", __FUNCTION__, cmd, arg0, arg1);
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
    UInt64 result;
    UInt32 v5;
    uint8_t *a1 = (uint8_t *)arg;
    uint8_t *req_ptr = (uint8_t*)req;
    if (a1 == NULL) {
        IOLog("%s a1==NULL\n", __FUNCTION__);
        return 0;
    }
    IOLog("%s %s\n", __FUNCTION__, a1);
    if ( ctl != 3223873993LL && ctl != 2150132168LL ) {
        *(uint64_t*)(req_ptr + 8) = *((uint64_t*)((uint8_t*)a1 + 8));
        *(uint64_t*)req_ptr = *(uint64_t*)a1;
        *(uint32_t*)(req_ptr + 0x10) = *((uint32_t*)((uint8_t*)a1 + 0x10));
        *(uint32_t*)(req_ptr + 0x14) = *((uint32_t*)((uint8_t*)a1 + 0x14));
        *(uint32_t*)(req_ptr + 0x18) = *((uint32_t*)((uint8_t*)a1 + 0x18));
        result = (uint64_t)(*((uint32_t*)((uint8_t*)a1 + 0x1C)));
        v5 = 4;
    } else {
        *(uint64_t*)(req_ptr + 8) = *((uint64_t*)((uint8_t*)a1 + 8));
        *(uint64_t*)req_ptr = *(uint64_t*)a1;
        *(uint32_t*)(req_ptr + 0x10) = *((uint32_t*)((uint8_t*)a1 + 0x10));
        *(uint32_t*)(req_ptr + 0x14) = *((uint32_t*)((uint8_t*)a1 + 0x14));
        *(uint32_t*)(req_ptr + 0x18) = *((uint32_t*)((uint8_t*)a1 + 0x18));
        result = *((uint64_t*)((uint8_t*)a1 + 0x20));
        v5 = 8;
    }
    *(uint64_t*)(req_ptr + 0x20) = result;
    *(uint32_t*)(req_ptr + 0x28) = (uint32_t)v5;
    return result;
}

int IO80211Interface::performGatedCommand(void * target,
                                             void * arg1_ctr,
                                             void * arg2_cmd,
                                             void * arg3_0,
                                             void * arg4_1)
{
    IOLog("%s\n", __FUNCTION__);
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
    IOLog("%s\n", __FUNCTION__);
    if (method == IOCTL_GET) {
        return apple80211_ioctl_get(netif, a6);
    } else {
        return apple80211_ioctl_set(netif, a6);
    }
}

int IO80211Interface::apple80211_ioctl_get(IO80211Interface *netif, apple80211req *a6)
{
    uint32_t index = a6->req_type - 1;
    IOLog("%s %d\n", __FUNCTION__, index);
    if (index > 0x160) {
        return 102;
    }
    return kIOReturnSuccess;
}

int IO80211Interface::apple80211_ioctl_set(IO80211Interface *netif, apple80211req *a6)
{
    uint32_t index = a6->req_type - 1;
    IOLog("%s %d\n", __FUNCTION__, index);
    if (index > 0x160) {
        return 102;
    }
    return kIOReturnSuccess;
}
