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
    if (ret == kIOReturnSuccess) {
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
    if (ret == kIOReturnSuccess) {
        if (data->req_len < pd.ssid_len) {
            data->req_len = pd.ssid_len;
        }
        copyout(pd.ssid_bytes, (user_addr_t)data->req_data, data->req_len);
    }
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
    if (ret == kIOReturnSuccess) {
        data->req_len = 6;
        copyout(pd.bssid.octet, (user_addr_t)data->req_data, 6);
    }
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
    if (ret == kIOReturnSuccess) {
        size_t len = sizeof(struct apple80211_capability_data) - sizeof(uint32_t);
        if (data->req_len < sizeof(struct apple80211_capability_data) - sizeof(uint32_t)) {
            len = data->req_len;
        }
        copyout(pd.capabilities, (user_addr_t)data->req_data, len);
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
    if (ret == kIOReturnSuccess) {
        data->req_val = pd.op_mode;
    }
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
    if (ret == kIOReturnSuccess) {
        copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    }
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
    if (ret == kIOReturnSuccess) {
        copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    }
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
    if (ret == kIOReturnSuccess) {
        copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    }
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
    if (ret == kIOReturnSuccess) {
        copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    }
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
    if (ret == kIOReturnSuccess) {
        copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    }
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
    if (ret == kIOReturnSuccess) {
        copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    }
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
    if (ret == kIOReturnSuccess) {
        data->req_val = pd.state;
    }
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
    if (ret == kIOReturnSuccess) {
        copyout(pd.cc, (user_addr_t)data->req_data, sizeof(pd.cc));
    }
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
    if (ret == kIOReturnSuccess) {
        copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    }
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
    if (ret == kIOReturnSuccess) {
        copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    }
    return ret;
}

IOReturn sGetPowerSave(IONetworkInterface *inf, struct apple80211req *data)
{
    IOReturn ret;
    struct apple80211_powersave_data pd;
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    ret = ctl->getPOWERSAVE(OSDynamicCast(IO80211Interface, inf), &pd);
    if (ret == kIOReturnSuccess) {
        data->req_val = pd.powersave_level;
    }
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
    if (ret == kIOReturnSuccess) {
        copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    }
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
    if (ret == kIOReturnSuccess) {
        data->req_val = pd.int_mit;
    }
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
    if (ret == kIOReturnSuccess) {
        data->req_len = min(data->req_len, sizeof(struct apple80211_sup_channel_data));
        copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    }
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
    if (ret == kIOReturnSuccess) {
        data->req_len = min(data->req_len, sizeof(struct apple80211_sup_channel_data));
        copyout(&pd, (user_addr_t)data->req_data, data->req_len);
    }
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
    if (ret == kIOReturnSuccess) {
        data->req_len = min(pd.string_len, sizeof(struct apple80211_version_data));
        copyout((uint8_t *)&pd + offsetof(struct apple80211_version_data, string), (user_addr_t)data->req_data, min(pd.string_len, sizeof(pd.string)));
    }
    return ret;
}

IOReturn sGetScanResult(IONetworkInterface *inf, struct apple80211req *data)
{
    IOReturn ret;
    struct apple80211_scan_result *sr;
    struct apple80211_scan_result *result;
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    if (data->req_flag == 8) {
        if (data->req_len != sizeof(struct apple80211_scan_result)) {
            VMLog("%s Invalid req len %lld\n", __FUNCTION__, data->req_len);
            return kIOReturnBadArgument;
        }
    } else if (data->req_len != 144) {
        VMLog("%s Invalid req len %lld\n", __FUNCTION__, data->req_len);
        return kIOReturnBadArgument;
    }
    ret = ctl->getSCAN_RESULT(OSDynamicCast(IO80211Interface, inf), &sr);
    if (sr == NULL || ret != kIOReturnSuccess) {
        return ret;
    }
    result = (struct apple80211_scan_result *)IOMalloc(sizeof(struct apple80211_scan_result));
    if (data->req_flag == 8) {
        if (copyin((user_addr_t)data->req_data, result, sizeof(struct apple80211_scan_result))) {
            VMLog("%s %d copy fail\n", __FUNCTION__, __LINE__);
            ret = kIOReturnIOError;
            goto freeNow;
        }
        result->version = sr->version;
        result->asr_channel.version = sr->asr_channel.version;
        result->asr_channel.channel = sr->asr_channel.channel;
        result->asr_channel.flags = sr->asr_channel.flags;
        result->asr_age = sr->asr_age;
        result->asr_beacon_int = sr->asr_beacon_int;
        result->asr_cap = sr->asr_cap;
        memcpy(result->asr_bssid, sr->asr_bssid, sizeof(sr->asr_bssid));
        result->asr_ssid_len = sr->asr_ssid_len;
        memcpy(result->asr_ssid, sr->asr_ssid, sizeof(sr->asr_ssid));
        result->asr_nrates = sr->asr_nrates;
        memcpy(result->asr_rates, sr->asr_rates, sizeof(sr->asr_rates));
        result->asr_snr = sr->asr_snr;
    } else {
        if (copyin((user_addr_t)data->req_data, result, 144)) {
            VMLog("%s %d copy fail\n", __FUNCTION__, __LINE__);
            ret = kIOReturnIOError;
            goto freeNow;
        }
        result->version = sr->version;
        result->asr_channel.version = sr->asr_channel.version;
        result->asr_channel.channel = sr->asr_channel.channel;
        result->asr_channel.flags = sr->asr_channel.flags;
        result->asr_age = sr->asr_age;
        result->asr_beacon_int = sr->asr_beacon_int;
        result->asr_cap = sr->asr_cap;
        memcpy(result->asr_bssid, sr->asr_bssid, sizeof(sr->asr_bssid));
        result->asr_ssid_len = sr->asr_ssid_len;
        memcpy(result->asr_ssid, sr->asr_ssid, sizeof(sr->asr_ssid));
        result->asr_nrates = sr->asr_nrates;
        memcpy(result->asr_rates, sr->asr_rates, sizeof(sr->asr_rates));
        result->asr_snr = sr->asr_snr;
        VMLog("%s %d Return result %d\n", __FUNCTION__, __LINE__, data->req_flag);
    }
    copyout(result, (user_addr_t)data->req_data, data->req_len);
freeNow:
    IOFree(result, sizeof(struct apple80211_scan_result));
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
    if (copyin((user_addr_t)data->req_data, &pd, sizeof(struct apple80211_power_data))) {
        return kIOReturnError;
    }
    ret = ctl->setPOWER(OSDynamicCast(IO80211Interface, inf), &pd);
    OSDynamicCast(IO80211Interface, inf)->postMessage(APPLE80211_M_POWER_CHANGED);
    return ret;
}

IOReturn sSetScanRequest(IONetworkInterface *inf, struct apple80211req *data)
{
    struct apple80211_scan_data sd;
    
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    bzero(&sd, sizeof(sd));
    if (copyin((user_addr_t)data->req_data, &sd, sizeof(struct apple80211_scan_data))) {
        return kIOReturnError;
    };
    return ctl->setSCAN_REQ(OSDynamicCast(IO80211Interface, inf), &sd);
}

IOReturn sSetScanRequestMultiple(IONetworkInterface *inf, struct apple80211req *data)
{
    struct apple80211_scan_multiple_data sd;
    
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    bzero(&sd, sizeof(sd));
    if (copyin((user_addr_t)data->req_data, &sd, sizeof(struct apple80211_scan_multiple_data))) {
        return kIOReturnError;
    };
    return ctl->setSCAN_REQ_MULTIPLE(OSDynamicCast(IO80211Interface, inf), &sd);
}

IOReturn sSetScanCacheClear(IONetworkInterface *inf, struct apple80211req *data)
{
    IO80211Controller *ctl = OSDynamicCast(IO80211Controller, inf->getController());
    if (!ctl) {
        return kIOReturnError;
    }
    return ctl->setSCANCACHE_CLEAR(OSDynamicCast(IO80211Interface, inf));
}
