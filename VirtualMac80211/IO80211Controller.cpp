#include "IO80211Controller.hpp"
#include "AirportIOCTL.h"

OSDefineMetaClassAndAbstractStructors(IO80211Controller, IOEthernetController)

IOReturn sGetPOWER(IONetworkInterface *inf, struct apple80211req *data)
{
    IOReturn ret;
    struct apple80211_power_data pd;
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    ret = ctl->getPOWER(OSDynamicCast(IO80211Interface, inf), &pd);
    if (!ret) {
        size_t len = sizeof(struct apple80211_power_data);
        if (data->req_len < sizeof(struct apple80211_power_data)) {
            len = data->req_len;
        }
        copyout(&pd, (user_addr_t)data->req_data, len);
    }
    return ret;
}

IOReturn sGetSSID(IONetworkInterface *inf, struct apple80211req *data)
{
    IOReturn ret;
    struct apple80211_ssid_data pd;
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    ret = ctl->getSSID(OSDynamicCast(IO80211Interface, inf), &pd);
    copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    return ret;
}

IOReturn sGetBSSID(IONetworkInterface *inf, struct apple80211req *data)
{
    IOReturn ret;
    struct apple80211_bssid_data pd;
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    ret = ctl->getBSSID(OSDynamicCast(IO80211Interface, inf), &pd);
    copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    return ret;
}

IOReturn sGetCardCapa(IONetworkInterface *inf, struct apple80211req *data)
{
    IOReturn ret;
    struct apple80211_capability_data pd;
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    ret = ctl->getCARD_CAPABILITIES(OSDynamicCast(IO80211Interface, inf), &pd);
    if (!ret) {
        size_t len = sizeof(struct apple80211_capability_data) - sizeof(uint32_t);
        if (data->req_len < sizeof(struct apple80211_capability_data) - sizeof(uint32_t)) {
            len = data->req_len;
        }
        copyout(((char *)&pd) + 4, (user_addr_t)data->req_data, len);
    }
    return ret;
}

IOReturn sGetOpMode(IONetworkInterface *inf, struct apple80211req *data)
{
    IOReturn ret;
    struct apple80211_opmode_data pd;
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    ret = ctl->getOP_MODE(OSDynamicCast(IO80211Interface, inf), &pd);
    copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    return ret;
}

IOReturn sGetPhyMode(IONetworkInterface *inf, struct apple80211req *data)
{
    IOReturn ret;
    struct apple80211_phymode_data pd;
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    ret = ctl->getPHY_MODE(OSDynamicCast(IO80211Interface, inf), &pd);
    copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    return ret;
}

IOReturn sGetRSSI(IONetworkInterface *inf, struct apple80211req *data)
{
    IOReturn ret;
    struct apple80211_rssi_data pd;
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    ret = ctl->getRSSI(OSDynamicCast(IO80211Interface, inf), &pd);
    copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    return ret;
}

IOReturn sGetNoise(IONetworkInterface *inf, struct apple80211req *data)
{
    IOReturn ret;
    struct apple80211_noise_data pd;
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    ret = ctl->getNOISE(OSDynamicCast(IO80211Interface, inf), &pd);
    copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    return ret;
}

IOReturn sGetRate(IONetworkInterface *inf, struct apple80211req *data)
{
    IOReturn ret;
    struct apple80211_rate_data pd;
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    ret = ctl->getRATE(OSDynamicCast(IO80211Interface, inf), &pd);
    copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    return ret;
}

IOReturn sGetChannel(IONetworkInterface *inf, struct apple80211req *data)
{
    IOReturn ret;
    struct apple80211_channel_data pd;
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    ret = ctl->getCHANNEL(OSDynamicCast(IO80211Interface, inf), &pd);
    copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    return ret;
}

IOReturn sGetAuthType(IONetworkInterface *inf, struct apple80211req *data)
{
    IOReturn ret;
    struct apple80211_authtype_data pd;
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    ret = ctl->getAUTH_TYPE(OSDynamicCast(IO80211Interface, inf), &pd);
    copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    return ret;
}

IOReturn sGetState(IONetworkInterface *inf, struct apple80211req *data)
{
    IOReturn ret;
    struct apple80211_state_data pd;
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    ret = ctl->getSTATE(OSDynamicCast(IO80211Interface, inf), &pd);
    copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    return ret;
}

IOReturn sGetCountryCode(IONetworkInterface *inf, struct apple80211req *data)
{
    IOReturn ret;
    struct apple80211_country_code_data pd;
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    ret = ctl->getCOUNTRY_CODE(OSDynamicCast(IO80211Interface, inf), &pd);
    copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    return ret;
}

IOReturn sGetMCS(IONetworkInterface *inf, struct apple80211req *data)
{
    IOReturn ret;
    struct apple80211_mcs_data pd;
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    ret = ctl->getMCS(OSDynamicCast(IO80211Interface, inf), &pd);
    copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    return ret;
}

IOReturn sGetTxPower(IONetworkInterface *inf, struct apple80211req *data)
{
    IOReturn ret;
    struct apple80211_txpower_data pd;
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    ret = ctl->getTXPOWER(OSDynamicCast(IO80211Interface, inf), &pd);
    copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    return ret;
}

IOReturn sGetPowerSave(IONetworkInterface *inf, struct apple80211req *data)
{
    IOReturn ret;
    struct apple80211_txpower_data pd;
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    ret = ctl->getTXPOWER(OSDynamicCast(IO80211Interface, inf), &pd);
    copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    return ret;
}

IOReturn sGetProtMode(IONetworkInterface *inf, struct apple80211req *data)
{
    IOReturn ret;
    struct apple80211_protmode_data pd;
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    ret = ctl->getPROTMODE(OSDynamicCast(IO80211Interface, inf), &pd);
    copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    return ret;
}

IOReturn sGetIntMit(IONetworkInterface *inf, struct apple80211req *data)
{
    IOReturn ret;
    struct apple80211_intmit_data pd;
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    ret = ctl->getINT_MIT(OSDynamicCast(IO80211Interface, inf), &pd);
    copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    return ret;
}


IOReturn sGetSupportedChannels(IONetworkInterface *inf, struct apple80211req *data)
{
    IOReturn ret;
    struct apple80211_sup_channel_data pd;
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    ret = ctl->getSUPPORTED_CHANNELS(OSDynamicCast(IO80211Interface, inf), &pd);
    data->req_len = min(data->req_len, sizeof(struct apple80211_sup_channel_data));
    copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    return ret;
}

IOReturn sGetHWSupportedChannels(IONetworkInterface *inf, struct apple80211req *data)
{
    IOReturn ret;
    struct apple80211_sup_channel_data pd;
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    ret = ctl->getHW_SUPPORTED_CHANNELS(OSDynamicCast(IO80211Interface, inf), &pd);
    data->req_len = min(data->req_len, sizeof(struct apple80211_sup_channel_data));
    copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    return ret;
}

IOReturn sGetVersion(IONetworkInterface *inf, struct apple80211req *data) 
{
    IOReturn ret;
    struct apple80211_version_data pd;
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    ret = ctl->getDRIVER_VERSION(OSDynamicCast(IO80211Interface, inf), &pd);
    data->req_len = min(pd.string_len, sizeof(struct apple80211_version_data));
    copyout((uint8_t *)&pd + offsetof(struct apple80211_version_data, string), (user_addr_t)data->req_data, min(pd.string_len, sizeof(pd.string)));
    return ret;
}

IOReturn sSetPOWER(IONetworkInterface *inf, struct apple80211req *data)
{
    IOReturn ret;
    struct apple80211_power_data pd;
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    copyin((user_addr_t)data->req_data, &pd, sizeof(struct apple80211_power_data));
    ret = ctl->setPOWER(OSDynamicCast(IO80211Interface, inf), &pd);
    return ret;
}

IOReturn sSetScanRequest(IONetworkInterface *inf, struct apple80211req *data)
{
    struct apple80211_scan_data sd;
    
    
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    copyin((user_addr_t)data->req_data, &sd, min(sizeof(struct apple80211_scan_data), data->req_len));
    return ctl->setSCAN_REQ(OSDynamicCast(IO80211Interface, inf), &sd);
}

IOReturn sSetScanCacheClear(IONetworkInterface *inf, struct apple80211req *data)
{
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    return ctl->setSCANCACHE_CLEAR(OSDynamicCast(IO80211Interface, inf));
}
