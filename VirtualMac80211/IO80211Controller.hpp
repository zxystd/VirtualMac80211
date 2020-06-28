

#ifndef IO80211Controller_hpp
#define IO80211Controller_hpp

#include <IOKit/IOService.h>
#include "IOEthernetController.h"

class IO80211Controller : public IOEthernetController {
    OSDeclareAbstractStructors(IO80211Controller)
};

#endif /* IO80211Controller_hpp */
