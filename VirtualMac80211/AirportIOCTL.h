//
//  AirportIOCTL.h
//  VirtualMac80211
//
//  Created by qcwap on 2020/6/28.
//  Copyright © 2020 zxystd. All rights reserved.
//

#ifndef AirportIOCTL_h
#define AirportIOCTL_h

#include "apple/IONetworkInterface.h"

IOReturn sGetSSID(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sGetBSSID(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sGetPOWER(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sGetINT_MIT(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sGetAuthType(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sGetCardCapa(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sGetOpMode(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sGetPhyMode(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sGetRSSI(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sGetNoise(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sGetChannel(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sGetAuthType(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sGetRate(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sGetState(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sGetCountryCode(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sGetMCS(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sGetTxPower(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sGetPowerSave(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sGetProtMode(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sGetIntMit(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sGetSupportedChannels(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sGetHWSupportedChannels(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sGetVersion(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sGetScanResult(IONetworkInterface *inf, struct apple80211req *data);


IOReturn sSetPOWER(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sSetScanRequest(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sSetScanRequestMultiple(IONetworkInterface *inf, struct apple80211req *data);

IOReturn sSetScanCacheClear(IONetworkInterface *inf, struct apple80211req *data);

#endif /* AirportIOCTL_h */
