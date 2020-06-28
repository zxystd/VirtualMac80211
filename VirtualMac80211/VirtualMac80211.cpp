
#include "VirtualMac80211.hpp"
#include <sys/kpi_mbuf.h>

#define super IO80211Controller
OSDefineMetaClassAndStructors(VirtualMac80211, IO80211Controller)

bool VirtualMac80211::init(OSDictionary *properties) {
    IOLog("Driver init()");
    return super::init(properties);
}

void VirtualMac80211::free() {
    IOLog("Driver free()");
    super::free();
}

IOService* VirtualMac80211::probe(IOService* provider, SInt32 *score) {
    IOLog("Driver probe");
    super::probe(provider, score);
    return this;
}

bool VirtualMac80211::start(IOService *provider) {
    IOLog("Driver start");
    if (!super::start(provider)) {
        IOLog("Super start call failed!");
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
        IOLog("attachInterface failed!");
        releaseAll();
        return false;
    }
    registerService();
    return true;
}

void VirtualMac80211::stop(IOService *provider) {
    IOLog("Driver stop");
    super::stop(provider);
    detachInterface(fNetIf);
    fNetIf->release();
}

bool VirtualMac80211::createMediumTables(const IONetworkMedium **primary)
{
    IONetworkMedium    *medium;
    
    OSDictionary *mediumDict = OSDictionary::withCapacity(1);
    if (mediumDict == NULL) {
        IOLog("Cannot allocate OSDictionary\n");
        return false;
    }
    
    medium = IONetworkMedium::medium(kIOMediumEthernet1000BaseT | kIOMediumOptionFullDuplex | kIOMediumOptionFlowControl, 1000 * 1000000);
    IONetworkMedium::addMedium(mediumDict, medium);
    medium->release();
    if (primary) {
        *primary = medium;
    }
    
    bool result = publishMediumDictionary(mediumDict);
    if (!result) {
        IOLog("Cannot publish medium dictionary!\n");
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
    IOLog("%s\n", __FUNCTION__);
}

bool VirtualMac80211::configureInterface(IONetworkInterface *netif) {
    IONetworkData *nd;
    
    IOLog("%s\n", __FUNCTION__);
    if (super::configureInterface(netif) == false) {
        IOLog("super failed\n");
        return false;
    }
    
    nd = netif->getNetworkData(kIONetworkStatsKey);
    if (!nd || !(fpNetStats = (IONetworkStats *)nd->getBuffer())) {
        IOLog("network statistics buffer unavailable?\n");
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
    return kIOReturnSuccess;
}

IOReturn VirtualMac80211::enable(IONetworkInterface *netif)
{
    super::enable(netif);
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
    IOLog("%s len=%d\n", __FUNCTION__, mbuf_pkthdr_len(m));
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
