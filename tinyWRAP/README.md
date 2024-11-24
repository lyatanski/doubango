## tynyWRAP
this project provides binding to different languages generated using [SWIG](https://www.swig.org/).
If supported language is detected by cmake, bindings for it will be generated

```{.py}
import tinyWRAP as wrap
```

callback for particular event could be defined
```{.py}
class SipCallback(wrap.SipCallback):
    def __init__(self):
        super().__init__()
        self.__disown__()

    def OnDialogEvent(self, event):
        return 0

    def OnStackEvent(self, event):
        return 0

    def OnInviteEvent(self, event):
        return 0

    def OnMessagingEvent(self, event):
        return 0

    def OnInfoEvent(self, event):
        return 0

    def OnOptionsEvent(self, event):
        return 0

    def OnPublicationEvent(self, event):
        return 0

    def OnRegistrationEvent(self, event):
        return 0

    def OnSubscriptionEvent(self, event):
        return 0
```

create instance:
```{.py}
mcc = "001"
mnc = "001"
domain = f"ims.mnc{mnc}.mcc{mcc}.3gppnetwork.org"
sip = wrap.SipStack(SipCallback(), domain, f"impi@{domain}", f"sip:impu@{domain}")
sip.setProxyCSCF(pcscf, 5060, "tcp", "ipv4")
```

set security parameters:
```{.py}
sip.setAMF("8000")
sip.setPassword(Ki)
sip.setOperatorIdConcealed(opc)
```

start the stack:
```{.py}
sip.start()
```
