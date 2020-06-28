#include "IO80211Interface.hpp"
#include "IO80211Controller.hpp"
#include <IOKit/IOLib.h>
#include <IOKit/IOService.h>
#include <IOKit/network/IOEthernetController.h>
#include <IOKit/network/IOOutputQueue.h>
#include <IOKit/IOCommandGate.h>

class VirtualMac80211 : public IO80211Controller {
    OSDeclareDefaultStructors(VirtualMac80211)
    
public:
    bool init(OSDictionary *properties) override;
    void free() override;
    IOService* probe(IOService* provider, SInt32* score) override;
    bool start(IOService *provider) override;
    void stop(IOService *provider) override;
    IOReturn getHardwareAddress(IOEthernetAddress* addrP) override;
    IOReturn enable(IONetworkInterface *netif) override;
    IOReturn disable(IONetworkInterface *netif) override;
    UInt32 outputPacket(mbuf_t, void * param) override;
    IOReturn setPromiscuousMode(IOEnetPromiscuousMode mode) override;
    IOReturn setMulticastMode(IOEnetMulticastMode mode) override;
    IOReturn setMulticastList(IOEthernetAddress* addr, UInt32 len) override;
    bool configureInterface(IONetworkInterface *netif) override;
    virtual IONetworkInterface * createInterface() override;
    
    bool setupUserClient();
    bool createMediumTables(const IONetworkMedium **primary);
    
    void releaseAll();
    
protected:
    IOEthernetInterface *fNetIf;
    IONetworkStats *fpNetStats;
};
