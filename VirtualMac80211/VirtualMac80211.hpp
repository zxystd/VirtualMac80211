#include "IO80211Interface.hpp"
#include "IO80211Controller.hpp"
#include <IOKit/IOLib.h>
#include <IOKit/IOService.h>
#include <IOKit/network/IOEthernetController.h>
#include <IOKit/network/IOOutputQueue.h>
#include <IOKit/IOCommandGate.h>

void *vmAirportInstance;

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
    virtual const OSString* newVendorString() const override;
    virtual const OSString* newModelString() const override;
    virtual const OSString* newRevisionString() const override;
    
    bool setupUserClient();
    bool createMediumTables(const IONetworkMedium **primary);
    
    void releaseAll();
    
    virtual IOReturn getPOWER(IO80211Interface *interface, struct apple80211_power_data *pd) override;
    
    virtual IOReturn setPOWER(IO80211Interface *interface, struct apple80211_power_data *pd) override;
    
    virtual IOReturn getCARD_CAPABILITIES(IO80211Interface *interface, struct apple80211_capability_data *cd) override;
    
    virtual IOReturn getOP_MODE(IO80211Interface *interface, struct apple80211_opmode_data *od) override;
    
    virtual IOReturn getPHY_MODE(IO80211Interface *interface, struct apple80211_phymode_data *pd) override;
    
    virtual IOReturn getRSSI(IO80211Interface *interface, struct apple80211_rssi_data *rd) override;
    
    virtual IOReturn getNOISE(IO80211Interface *interface, struct apple80211_noise_data *nd) override;
    
    virtual IOReturn getBSSID(IO80211Interface *interface, struct apple80211_bssid_data *bd) override;
    
    virtual IOReturn getRATE(IO80211Interface *interface, struct apple80211_rate_data *rd) override;
    
    virtual IOReturn getAUTH_TYPE(IO80211Interface *interface, struct apple80211_authtype_data *ad) override;
    
    virtual IOReturn getSSID(IO80211Interface *interface, struct apple80211_ssid_data *sd) override;
    
    virtual IOReturn getSTATE(IO80211Interface *interface, struct apple80211_state_data *sd) override;
    
    virtual IOReturn getCOUNTRY_CODE(IO80211Interface *interface, struct apple80211_country_code_data *cd) override;
    
    virtual IOReturn getMCS(IO80211Interface *interface, struct apple80211_mcs_data *md) override;
    
    virtual IOReturn getTXPOWER(IO80211Interface *interface, struct apple80211_txpower_data *txd) override;
    
    virtual IOReturn getPROTMODE(IO80211Interface *interface, struct apple80211_protmode_data *pd) override;
    
    virtual IOReturn getPOWERSAVE(IO80211Interface *interface, struct apple80211_powersave_data *pd) override;
    
    virtual IOReturn getINT_MIT(IO80211Interface *interface, struct apple80211_intmit_data *imd) override;
    
    virtual IOReturn getSUPPORTED_CHANNELS(IO80211Interface *interface, struct apple80211_sup_channel_data *ad) override;
    
    virtual IOReturn getHW_SUPPORTED_CHANNELS(IO80211Interface *interface, struct apple80211_sup_channel_data *ad) override;
    
    virtual IOReturn getDRIVER_VERSION(IO80211Interface *interface,
                                         struct apple80211_version_data *hv) override;
    
    virtual IOReturn getHARDWARE_VERSION(IO80211Interface *interface,
                                         struct apple80211_version_data *hv) override;
    
public:
    uint8_t power_state;
    bool isAssociating;
    uint8_t curState;
    bool scanEnd;

protected:
    IOEthernetInterface *fNetIf;
    IONetworkStats *fpNetStats;
    
};
