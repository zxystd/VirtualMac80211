

#ifndef IO80211Controller_hpp
#define IO80211Controller_hpp

#include <IOKit/IOService.h>
#include "IOEthernetController.h"
#include "IO80211Interface.hpp"

class IO80211Controller : public IOEthernetController {
    OSDeclareAbstractStructors(IO80211Controller)
    
public:
    
    virtual IOReturn getSSID(IO80211Interface *interface, struct apple80211_ssid_data *sd) {
        return kIOReturnError;
    };
    
    virtual IOReturn setSSID(IO80211Interface *interface, struct apple80211_ssid_data *sd) {
        return kIOReturnError;
    }
    
    virtual IOReturn getAUTH_TYPE(IO80211Interface *interface, struct apple80211_authtype_data *ad) {
        return kIOReturnError;
    }
    
    virtual IOReturn getCHANNEL(IO80211Interface *interface,
                                struct apple80211_channel_data *cd) {
        return kIOReturnError;
    }
    
    virtual IOReturn getPROTMODE(IO80211Interface *interface, struct apple80211_protmode_data *pd) {
        return kIOReturnError;
    }
    
    virtual IOReturn getTXPOWER(IO80211Interface *interface,
                                struct apple80211_txpower_data *txd) {
        return kIOReturnError;
    }
    
    virtual IOReturn getRATE(IO80211Interface* interface,
                             struct apple80211_rate_data* rd) {
        return kIOReturnError;
    }
    
    virtual IOReturn getBSSID(IO80211Interface* interface,
                      struct apple80211_bssid_data* bd) {
        return kIOReturnError;
    }
    
    virtual IOReturn setSCANCACHE_CLEAR(IO80211Interface *interface) {
        return kIOReturnSuccess;
    }
    
    virtual IOReturn setSCAN_REQ(IO80211Interface* interface,
                                 struct apple80211_scan_data* sd) {
        return kIOReturnError;
    }
    
    virtual IOReturn setSCAN_REQ_MULTIPLE(IO80211Interface* interface,
                                          struct apple80211_scan_multiple_data* sd) {
        return kIOReturnError;
    }
    
    virtual IOReturn getSCAN_RESULT(IO80211Interface* interface,
                            apple80211_scan_result** sr) {
        return kIOReturnError;
    }
    
    virtual IOReturn getCARD_CAPABILITIES(IO80211Interface* interface,
                                  struct apple80211_capability_data* cd) {
        return kIOReturnError;
    }
    
    virtual IOReturn getSTATE(IO80211Interface* interface,
                              struct apple80211_state_data* sd) {
        return kIOReturnError;
    }
    
    virtual IOReturn setSTATE(IO80211Interface* interface,
                              struct apple80211_state_data* sd) {
        return kIOReturnError;
    }
    
    virtual IOReturn getPHY_MODE(IO80211Interface* interface,
                                 struct apple80211_phymode_data* pd) {
        return kIOReturnError;
    }
    
    virtual IOReturn getOP_MODE(IO80211Interface* interface,
                                struct apple80211_opmode_data* od) {
        return kIOReturnError;
    }
    
    virtual IOReturn getRSSI(IO80211Interface* interface,
                             struct apple80211_rssi_data* rd) {
        return kIOReturnError;
    }
    
    virtual IOReturn getNOISE(IO80211Interface* interface,
                      struct apple80211_noise_data* nd) {
        return kIOReturnError;
    }
    
    virtual IOReturn getINT_MIT(IO80211Interface* interface,
                                struct apple80211_intmit_data* imd) {
        return kIOReturnError;
    }
    
    virtual IOReturn getPOWER(IO80211Interface* interface,
                              struct apple80211_power_data* pd) {
        return kIOReturnError;
    }
    
    virtual IOReturn setPOWER(IO80211Interface* interface,
                      struct apple80211_power_data* pd) {
        return kIOReturnError;
    }
    
    virtual IOReturn getCOUNTRY_CODE(IO80211Interface* interface,
                             struct apple80211_country_code_data* cd) {
        return kIOReturnError;
    }
    
    virtual IOReturn getMCS(IO80211Interface* interface, struct apple80211_mcs_data* md) {
        return kIOReturnError;
    }
    
    virtual IOReturn getPOWERSAVE(IO80211Interface* interface, struct apple80211_powersave_data *pd) {
        return kIOReturnError;
    }
    
    virtual IOReturn getSUPPORTED_CHANNELS(IO80211Interface* interface, struct apple80211_sup_channel_data *pd) {
        return kIOReturnError;
    }
    
    virtual IOReturn getHW_SUPPORTED_CHANNELS(IO80211Interface* interface, struct apple80211_sup_channel_data *pd) {
        return kIOReturnError;
    }
    
    virtual IOReturn getDRIVER_VERSION(IO80211Interface *interface,
                                         struct apple80211_version_data *hv) {
        return kIOReturnError;
    }
    
    virtual IOReturn getHARDWARE_VERSION(IO80211Interface *interface,
                                         struct apple80211_version_data *hv) {
        return kIOReturnError;
    }
    
    virtual IO80211Interface *getNetworkInterface() 
    {
        return _netIf;
    }
    
    virtual bool attachInterface(IONetworkInterface ** interface, bool doRegister = true) override 
    {
        if (IOEthernetController::attachInterface(interface, doRegister)) {
            _netIf = OSDynamicCast(IO80211Interface, *interface);
            return true;
        }
        return false;
    };
    
    virtual void detachInterface(IONetworkInterface * interface, bool sync = false) override 
    {
        IOEthernetController::detachInterface(interface, sync);
        _netIf = NULL;
    }
    
    virtual bool setLinkStatus(
                               UInt32                  status,
                               const IONetworkMedium * activeMedium = 0,
                               UInt64                  speed        = 0,
                               OSData *                data         = 0) override;
    
private:
    IO80211Interface *_netIf;
};

#endif /* IO80211Controller_hpp */
