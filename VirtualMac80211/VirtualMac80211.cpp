
#include "VirtualMac80211.hpp"
#include <sys/kpi_mbuf.h>

#include <IOKit/IOTimerEventSource.h>

#define super IO80211Controller
OSDefineMetaClassAndStructors(VirtualMac80211, IO80211Controller)

bool VirtualMac80211::init(OSDictionary *properties) {
    VMLog("Driver init()");
    return super::init(properties);
}

void VirtualMac80211::free() {
    VMLog("Driver free()");
    super::free();
}

IOService* VirtualMac80211::probe(IOService* provider, SInt32 *score) {
    VMLog("Driver probe");
    if (vmAirportInstance != NULL) {
        VMLog("Can't support 2 instances - bye...\n");
        return NULL;
    }
    super::probe(provider, score);
    vmAirportInstance = this;
    return this;
}

bool VirtualMac80211::start(IOService *provider) {
    VMLog("Driver start");
    if (!super::start(provider)) {
        VMLog("Super start call failed!");
        releaseAll();
        return false;
    }
    scanResult = NULL;
    const IONetworkMedium *primaryMedium;
    if (!createMediumTables(&primaryMedium) ||
        !setCurrentMedium(primaryMedium)) {
        releaseAll();
        return false;
    }
    if (!attachInterface((IONetworkInterface **)&fNetIf)) {
        VMLog("attachInterface failed!");
        releaseAll();
        return false;
    }
    scanSource = IOTimerEventSource::timerEventSource(this, &fakeScanDone);
    getWorkLoop()->addEventSource(scanSource);
    registerService();
    return true;
}

void VirtualMac80211::fakeScanDone(OSObject *owner, IOTimerEventSource *sender)
{
    VirtualMac80211 *that = (VirtualMac80211 *)owner;
    that->fNetIf->postMessage(APPLE80211_M_SCAN_DONE);
}

void VirtualMac80211::stop(IOService *provider) {
    VMLog("Driver stop");
    super::stop(provider);
    if (scanResult) {
        if (scanResult->asr_ie_len && scanResult->asr_ie_data) {
            IOFree(scanResult->asr_ie_data, scanResult->asr_ie_len);
            scanResult->asr_ie_data = NULL;
            scanResult->asr_ie_len = 0;
        }
        IOFree(scanResult, sizeof(struct apple80211_scan_result));
        scanResult = NULL;
    }
    detachInterface(fNetIf);
    fNetIf->release();
}

bool VirtualMac80211::createMediumTables(const IONetworkMedium **primary)
{
    IONetworkMedium    *medium;
    
    OSDictionary *mediumDict = OSDictionary::withCapacity(1);
    if (mediumDict == NULL) {
        VMLog("Cannot allocate OSDictionary\n");
        return false;
    }
    
    medium = IONetworkMedium::medium(0x80, 11000000);
    IONetworkMedium::addMedium(mediumDict, medium);
    medium->release();
    if (primary) {
        *primary = medium;
    }
    
    bool result = publishMediumDictionary(mediumDict);
    if (!result) {
        VMLog("Cannot publish medium dictionary!\n");
    }

    mediumDict->release();
    return result;
}

bool VirtualMac80211::setupUserClient()
{
    setProperty("IOUserClientClass", "ItlNetworkUserClient");
    return true;
}

void VirtualMac80211::releaseAll()
{
    VMLog("%s\n", __FUNCTION__);
}

bool VirtualMac80211::configureInterface(IONetworkInterface *netif) {
    IONetworkData *nd;
    
    VMLog("%s\n", __FUNCTION__);
    if (super::configureInterface(netif) == false) {
        VMLog("super failed\n");
        return false;
    }
    
    nd = netif->getNetworkData(kIONetworkStatsKey);
    if (!nd || !(fpNetStats = (IONetworkStats *)nd->getBuffer())) {
        VMLog("network statistics buffer unavailable?\n");
        return false;
    }
    
    return true;
}

IONetworkInterface * VirtualMac80211::createInterface()
{
    IO80211Interface * netif = new IO80211Interface;

    if ( netif && ( netif->init( this ) == false ) )
    {
        netif->release();
        netif = 0;
    }
    return netif;
}

IOReturn VirtualMac80211::disable(IONetworkInterface *netif)
{
    super::disable(netif);
    scanSource->disable();
    setLinkStatus(kIONetworkLinkValid);
    return kIOReturnSuccess;
}

IOReturn VirtualMac80211::enable(IONetworkInterface *netif)
{
    super::enable(netif);
    scanSource->enable();
    setLinkStatus(kIONetworkLinkValid);
    return kIOReturnSuccess;
}

IOReturn VirtualMac80211::getHardwareAddress(IOEthernetAddress *addrP) {
    addrP->bytes[0] = 0x12;
    addrP->bytes[1] = 0x22;
    addrP->bytes[2] = 0x32;
    addrP->bytes[3] = 0x42;
    addrP->bytes[4] = 0x52;
    addrP->bytes[5] = 0x62;
    return kIOReturnSuccess;
}

UInt32 VirtualMac80211::outputPacket(mbuf_t m, void *param)
{
//    VMLog("%s len=%d\n", __FUNCTION__, mbuf_pkthdr_len(m));
    freePacket(m);
    return kIOReturnOutputSuccess;
}

IOReturn VirtualMac80211::setPromiscuousMode(IOEnetPromiscuousMode mode) {
    return kIOReturnSuccess;
}

IOReturn VirtualMac80211::setMulticastMode(IOEnetMulticastMode mode) {
    return kIOReturnSuccess;
}

IOReturn VirtualMac80211::setMulticastList(IOEthernetAddress* addr, UInt32 len) {
    return kIOReturnSuccess;
}

IOReturn VirtualMac80211::getPOWER(IO80211Interface *interface,
                         struct apple80211_power_data *pd)
{
    pd->version = APPLE80211_VERSION;
    pd->num_radios = 1;
    pd->power_state[0] = power_state;
    return kIOReturnSuccess;
}

IOReturn VirtualMac80211::setPOWER(IO80211Interface *interface,
                         struct apple80211_power_data *pd)
{
    if (pd->num_radios > 0) {
        power_state = (pd->power_state[0]);
    }
    
    return kIOReturnSuccess;
}

IOReturn VirtualMac80211::getCARD_CAPABILITIES(IO80211Interface *interface,
                                     struct apple80211_capability_data *cd)
{
    cd->version = APPLE80211_VERSION;
    cd->capabilities[0] = 0xEB;
    cd->capabilities[1] = 0x7E;
    cd->capabilities[2] = 0xFF;
    cd->capabilities[5] |= 8;
    cd->capabilities[3] |= 2;
    cd->capabilities[4] |= 1;
    cd->capabilities[6] |= 8;
    cd->capabilities[8] |= 8;//dfs white list
    cd->capabilities[3] |= 0x21;
    cd->capabilities[4] |= 0x80;
    cd->capabilities[5] |= 4;
    cd->capabilities[2] |= 0xC0;
    cd->capabilities[6] |= 0x84;
    cd->capabilities[3] |= 8;
    cd->capabilities[4] |= 0xAC;
    cd->capabilities[6] |= 1;
    cd->capabilities[7] |= 4;
    cd->capabilities[5] |= 0x80;//isCntryDefaultSupported
    cd->capabilities[7] |= 0x80;
    cd->capabilities[8] |= 0x40;
    cd->capabilities[9] |= 8;
    cd->capabilities[9] |= 0x28;
    return kIOReturnSuccess;
}

IOReturn VirtualMac80211::getOP_MODE(IO80211Interface *interface,
                           struct apple80211_opmode_data *od)
{
    od->version = APPLE80211_VERSION;
    od->op_mode = APPLE80211_M_STA;
    return kIOReturnSuccess;
}

IOReturn VirtualMac80211::getPHY_MODE(IO80211Interface *interface,
                            struct apple80211_phymode_data *pd)
{
    pd->version = APPLE80211_VERSION;
    pd->phy_mode = APPLE80211_MODE_11A
    | APPLE80211_MODE_11B
    | APPLE80211_MODE_11G
    | APPLE80211_MODE_11N
    | APPLE80211_MODE_11AC;
    
    return kIOReturnSuccess;
}

IOReturn VirtualMac80211::getRSSI(IO80211Interface *interface,
                        struct apple80211_rssi_data *rd)
{
    if (curState == APPLE80211_S_RUN) {
        memset(rd, 0, sizeof(*rd));
        rd->version = APPLE80211_VERSION;
        rd->num_radios = 1;
        rd->rssi_unit    = APPLE80211_UNIT_DBM;
        rd->rssi[0]    = rd->aggregate_rssi
        = rd->rssi_ext[0]
        = rd->aggregate_rssi_ext
        = -101;
        return kIOReturnSuccess;
    }
    return kIOReturnError;
}

IOReturn VirtualMac80211::getNOISE(IO80211Interface *interface,
                         struct apple80211_noise_data *nd)
{
    if (curState == APPLE80211_S_RUN) {
        memset(nd, 0, sizeof(*nd));
        nd->version = APPLE80211_VERSION;
        nd->num_radios = 1;
        nd->noise_unit = APPLE80211_UNIT_DBM;
        nd->noise[0]    = nd->aggregate_noise
        = nd->noise_ext[0]
        = nd->aggregate_noise_ext
        = -100;
        return kIOReturnSuccess;
    }
    return kIOReturnError;
}

const uint8_t fake_bssid[] = { 0x64, 0x7C, 0x34, 0x5C, 0x1C, 0x40 };

IOReturn VirtualMac80211::getBSSID(IO80211Interface *interface,
                         struct apple80211_bssid_data *bd)
{
    if (curState == APPLE80211_S_RUN) {
        memset(bd, 0, sizeof(*bd));
        bd->version = APPLE80211_VERSION;
        bd->bssid.octet[0] = fake_bssid[0];
        bd->bssid.octet[1] = fake_bssid[1];
        bd->bssid.octet[2] = fake_bssid[2];
        bd->bssid.octet[3] = fake_bssid[3];
        bd->bssid.octet[4] = fake_bssid[4];
        bd->bssid.octet[5] = fake_bssid[5];
        return kIOReturnSuccess;
    }
    return kIOReturnError;
}

IOReturn VirtualMac80211::getRATE(IO80211Interface *interface, struct apple80211_rate_data *rd)
{
    if (curState == APPLE80211_S_RUN) {
        memset(rd, 0, sizeof(*rd));
        rd->version = APPLE80211_VERSION;
        rd->num_radios = 1;
        rd->rate[0] = 20;
        return kIOReturnSuccess;
    }
    return kIOReturnError;
}

IOReturn VirtualMac80211::getAUTH_TYPE(
                             IO80211Interface *interface, struct apple80211_authtype_data *ad)
{
    if (isAssociating) {
        ad->version = APPLE80211_VERSION;
        ad->authtype_lower = APPLE80211_AUTHTYPE_OPEN;
        ad->authtype_upper = 8;
        return kIOReturnSuccess;
    }
    return kIOReturnError;
}

const char *fake_ssid = "UPC5424297";

IOReturn VirtualMac80211::getSSID(IO80211Interface *interface,
                        struct apple80211_ssid_data *sd)
{
    if (curState == APPLE80211_S_RUN) {
        memset(sd, 0, sizeof(*sd));
        sd->version = APPLE80211_VERSION;
        memcpy(sd->ssid_bytes, fake_ssid, strlen(fake_ssid));
        sd->ssid_len = strlen(fake_ssid);
        return kIOReturnSuccess;
    }
    return kIOReturnError;
}

IOReturn VirtualMac80211::getSTATE(IO80211Interface *interface,
                         struct apple80211_state_data *sd)
{
    sd->version = APPLE80211_VERSION;
    sd->state = curState;
    return kIOReturnSuccess;
}

IOReturn VirtualMac80211::getCOUNTRY_CODE(IO80211Interface *interface,
                                struct apple80211_country_code_data *cd)
{
    cd->version = APPLE80211_VERSION;
    strncpy((char*)cd->cc, "CN", sizeof(cd->cc));
    return kIOReturnSuccess;
}

IOReturn VirtualMac80211::getMCS(IO80211Interface* interface, struct apple80211_mcs_data* md)
{
    if (curState == APPLE80211_S_RUN) {
        md->version = APPLE80211_VERSION;
        md->index = APPLE80211_MCS_INDEX_AUTO;
        return kIOReturnSuccess;
    }
    return kIOReturnError;
}

IOReturn VirtualMac80211::getTXPOWER(IO80211Interface *interface,
                           struct apple80211_txpower_data *txd)
{
    if (curState == APPLE80211_S_RUN) {
        memset(txd, 0, sizeof(*txd));
        txd->version = APPLE80211_VERSION;
        txd->txpower = 100;
        txd->txpower_unit = APPLE80211_UNIT_PERCENT;
        return kIOReturnSuccess;
    }
    return kIOReturnError;
}

IOReturn VirtualMac80211::getPROTMODE(
                            IO80211Interface *interface, struct apple80211_protmode_data *pd)
{
    if (curState == APPLE80211_S_RUN) {
        memset(pd, 0, sizeof(*pd));
        pd->version = APPLE80211_VERSION;
        pd->threshold = 0;
        pd->protmode = 0;
        return kIOReturnSuccess;
    }
    return kIOReturnError;
}

IOReturn VirtualMac80211::getPOWERSAVE(IO80211Interface *interface, struct apple80211_powersave_data *pd)
{
    pd->version = APPLE80211_VERSION;
    pd->powersave_level = APPLE80211_POWERSAVE_MODE_DISABLED;
    return kIOReturnSuccess;
}

IOReturn VirtualMac80211::getINT_MIT(
                           IO80211Interface *interface, struct apple80211_intmit_data *imd)
{
    imd->version = APPLE80211_VERSION;
    imd->int_mit = APPLE80211_INT_MIT_AUTO;
    return kIOReturnSuccess;
}

IOReturn VirtualMac80211::
getSUPPORTED_CHANNELS(IO80211Interface *interface, struct apple80211_sup_channel_data *ad)
{
    ad->version = APPLE80211_VERSION;
    ad->num_channels = 0;
    for (int i = 0; i < 14; i++) {
            ad->supported_channels[ad->num_channels++].channel = (i + 1);
            ad->supported_channels[ad->num_channels].flags = APPLE80211_C_FLAG_2GHZ;
    }
    return kIOReturnSuccess;
}

IOReturn VirtualMac80211::
getHW_SUPPORTED_CHANNELS(IO80211Interface *interface, struct apple80211_sup_channel_data *ad)
{
    ad->version = APPLE80211_VERSION;
    ad->num_channels = 0;
    for (int i = 0; i < 14; i++) {
            ad->supported_channels[ad->num_channels++].channel = (i + 1);
            ad->supported_channels[ad->num_channels].flags = APPLE80211_C_FLAG_2GHZ;
    }
    return kIOReturnSuccess;
}

IOReturn VirtualMac80211::
getDRIVER_VERSION(IO80211Interface *interface,
                                  struct apple80211_version_data *hv)
{
    hv->version = APPLE80211_VERSION;
    snprintf(hv->string, sizeof(hv->string), "RTL008: %s fw: %s", "1.0.0", "RTL");
    hv->string_len = strlen(hv->string);
    return kIOReturnSuccess;
}

IOReturn VirtualMac80211::
setSCANCACHE_CLEAR(IO80211Interface *interface)
{
    VMLog("%s\n", __FUNCTION__);
    if (scanResult) {
        if (scanResult->asr_ie_len && scanResult->asr_ie_data) {
            IOFree(scanResult->asr_ie_data, scanResult->asr_ie_len);
            scanResult->asr_ie_data = NULL;
            scanResult->asr_ie_len = 0;
        }
        IOFree(scanResult, sizeof(struct apple80211_scan_result));
        scanResult = NULL;
    }
    return kIOReturnSuccess;
}

IOReturn VirtualMac80211::
setSCAN_REQ(IO80211Interface *interface, struct apple80211_scan_data *sd)
{
    VMLog("%s Type: %u BSS Type: %u PHY Mode: %u Dwell time: %u Rest time: %u Num channels: %u SSID: %s\n",
          __FUNCTION__,
          sd->scan_type,
          sd->bss_type,
          sd->phy_mode,
          sd->dwell_time,
          sd->rest_time,
          sd->num_channels,
          sd->ssid);
    if (waitingForScanResult) {
        return kIOReturnError;
    }
    curState = APPLE80211_S_SCAN;
    if (interface && scanSource) {
        scanSource->setTimeoutMS(200);
        scanSource->enable();
    }
    waitingForScanResult = true;
    return kIOReturnSuccess;
}

IOReturn VirtualMac80211::
setSCAN_REQ_MULTIPLE(IO80211Interface *interface, struct apple80211_scan_multiple_data *sd)
{
    int i;
    VMLog("%s Type: %u SSID Count: %u BSSID Count: %u PHY Mode: %u Dwell time: %u Rest time: %u Num channels: %u Unk: %u\n",
          __FUNCTION__,
          sd->scan_type,
          sd->ssid_count,
          sd->bssid_count,
          sd->phy_mode,
          sd->dwell_time,
          sd->rest_time,
          sd->num_channels,
          sd->unk_2);
    for (i = 0; i < sd->ssid_count; i++) {
        VMLog("%s index=%d ssid=%s ssid_len=%d\n", __FUNCTION__, i, sd->ssids[i].ssid_bytes, sd->ssids[i].ssid_len);
    }
    if (waitingForScanResult) {
        return kIOReturnError;
    }
    if (interface && scanSource) {
        scanSource->setTimeoutMS(200);
        scanSource->enable();
    }
    waitingForScanResult = true;
    return kIOReturnSuccess;
}

const apple80211_channel fake_channel = {
    .version = APPLE80211_VERSION,
    .channel = 1,
    .flags = APPLE80211_C_FLAG_2GHZ
};

// This string contains information elements from beacon frame that I captured via Wireshark
const char beacon_ie[] = "\x00\x0a\x55" \
"\x50\x43\x35\x34\x32\x34\x32\x39\x37\x01\x08\x82\x84\x8b\x96\x0c" \
"\x12\x18\x24\x03\x01\x01\x05\x04\x00\x01\x00\x00\x07\x06\x43\x5a" \
"\x20\x01\x0d\x14\x2a\x01\x04\x32\x04\x30\x48\x60\x6c\x2d\x1a\xad" \
"\x01\x1b\xff\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" \
"\x00\x00\x00\x04\x06\xe6\xe7\x0d\x00\x3d\x16\x01\x00\x17\x00\x00" \
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" \
"\x00\x4a\x0e\x14\x00\x0a\x00\x2c\x01\xc8\x00\x14\x00\x05\x00\x19" \
"\x00\x7f\x01\x01\xdd\x18\x00\x50\xf2\x02\x01\x01\x80\x00\x03\xa4" \
"\x00\x00\x27\xa4\x00\x00\x42\x43\x5e\x00\x62\x32\x2f\x00\xdd\x09" \
"\x00\x03\x7f\x01\x01\x00\x00\xff\x7f\x30\x18\x01\x00\x00\x0f\xac" \
"\x02\x02\x00\x00\x0f\xac\x04\x00\x0f\xac\x02\x01\x00\x00\x0f\xac" \
"\x02\x00\x00\xdd\x1a\x00\x50\xf2\x01\x01\x00\x00\x50\xf2\x02\x02" \
"\x00\x00\x50\xf2\x04\x00\x50\xf2\x02\x01\x00\x00\x50\xf2\x02\xdd" \
"\x22\x00\x50\xf2\x04\x10\x4a\x00\x01\x10\x10\x44\x00\x01\x02\x10" \
"\x57\x00\x01\x01\x10\x3c\x00\x01\x01\x10\x49\x00\x06\x00\x37\x2a" \
"\x00\x01\x20";

IOReturn VirtualMac80211::
getSCAN_RESULT(IO80211Interface *interface,
                                           struct apple80211_scan_result **sr) {
    
    if (!waitingForScanResult) {
        return 0xe0820446;
    }
    
    struct apple80211_scan_result* result;
    
    if (scanResult) {
        result = scanResult;
    } else {
        result = (struct apple80211_scan_result*)IOMalloc(sizeof(struct apple80211_scan_result));
        bzero(result, sizeof(*result));
        result->version = APPLE80211_VERSION;
        
        result->asr_channel = fake_channel;
        
        result->asr_noise = -101;
        //    result->asr_snr = 60;
        result->asr_rssi = -73;
        result->asr_beacon_int = 100;
        
        result->asr_cap = 0x411;
        
        result->asr_age = 0;
        
        memcpy(result->asr_bssid, fake_bssid, sizeof(fake_bssid));
        
        result->asr_nrates = 1;
        result->asr_rates[0] = 54;
        
        strncpy((char*)result->asr_ssid, fake_ssid, sizeof(result->asr_ssid));
        result->asr_ssid_len = strlen(fake_ssid);
        
        result->asr_ie_len = 246;
        result->asr_ie_data = IOMalloc(result->asr_ie_len);
        memcpy(result->asr_ie_data, beacon_ie, result->asr_ie_len);
        scanResult = result;
    }

    *sr = result;
    
    waitingForScanResult = false;
    
    return kIOReturnSuccess;
}

IOReturn VirtualMac80211::
getHARDWARE_VERSION(IO80211Interface *interface,
                                    struct apple80211_version_data *hv)
{
    hv->version = APPLE80211_VERSION;
    strncpy(hv->string, "RTL", sizeof(hv->string));
    hv->string_len = strlen("RTL");
    return kIOReturnSuccess;
}

const OSString* VirtualMac80211::newVendorString() const {
    return OSString::withCString("Apple");
}

const OSString* VirtualMac80211::newModelString() const {
    return OSString::withCString("Intel Wiress Card");
}

const OSString* VirtualMac80211::newRevisionString() const {
    return OSString::withCString("1.0.0d");
}
