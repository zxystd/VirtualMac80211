
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
    return kIOReturnSuccess;
}

IOReturn VirtualMac80211::enable(IONetworkInterface *netif)
{
    super::enable(netif);
    scanSource->enable();
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
    cd->capabilities[0] = 0xeb;
    cd->capabilities[1] = 0x7e;
    cd->capabilities[2] |= 0xc0;
    cd->capabilities[2] = 3;
    cd->capabilities[2] |= 0x13;
    cd->capabilities[2] |= 0x20;
    cd->capabilities[2] |= 0x28;
    cd->capabilities[2] |= 4;
    cd->capabilities[5] |= 8;
    cd->capabilities[3] |= 2;
    cd->capabilities[4] |= 1;
    cd->capabilities[6] |= 8;

    cd->capabilities[3] |= 0x23;
    cd->capabilities[2] |= 0x80;
    cd->capabilities[5] |= 4;
    cd->capabilities[2] |= 0xC0;
    cd->capabilities[6] |= 0x84;
    cd->capabilities[3] |= 8;
    cd->capabilities[6] |= 1;
    cd->capabilities[5] |= 0x80;
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

const char *fake_ssid = "大脸猫来啦";

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
    strncpy((char*)cd->cc, "ZZ", sizeof(cd->cc));
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
    return kIOReturnSuccess;
}

IOReturn VirtualMac80211::
setSCAN_REQ(IO80211Interface *interface, struct apple80211_scan_data *sd)
{
    if (interface && scanSource) {
        scanSource->setTimeoutMS(200);
        scanSource->enable();
    }
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
