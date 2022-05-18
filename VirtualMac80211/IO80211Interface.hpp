

#ifndef ItlNetworkInterface_hpp
#define ItlNetworkInterface_hpp

#include "IOEthernetInterface.h"
#include "apple/apple80211_ioctl.h"

#define VMLog(fmt, x...)\
do\
{\
IOLog("%s: " fmt, "VM80211", ##x);\
}while(0)

typedef int apple80211_postMessage_tlv_types;

class IO80211Interface : public IOEthernetInterface
{
    OSDeclareDefaultStructors( IO80211Interface )
    
public:
    virtual bool init( IONetworkController * controller ) APPLE_KEXT_OVERRIDE;
    
protected:
    
    virtual void free() APPLE_KEXT_OVERRIDE;
    
    virtual SInt32 performCommand(IONetworkController * controller,
                                  unsigned long         cmd,
                                  void *                arg0,
                                  void *                arg1) APPLE_KEXT_OVERRIDE;
    
    static int performGatedCommand(void *, void *, void *, void *, void *);
    
    UInt64 IO80211InterfaceUserSpaceToKernelApple80211Request(void *arg, apple80211req *req, unsigned long ctl);
    
    int apple80211_ioctl(IO80211Interface *netif, UInt64 method, apple80211req *a6);
    
    int apple80211_ioctl_set(IO80211Interface *netif, apple80211req *a6);
    
    int apple80211_ioctl_get(IO80211Interface *netif, apple80211req *a6);
};

#endif /* ItlNetworkInterface_hpp */
