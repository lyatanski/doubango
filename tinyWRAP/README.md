## tynyWRAP
this project provides binding to different languages generated using [SWIG](https://www.swig.org/).
If Cmake detect a particular programming languange supported bu SWIG to be installed on the system, bindings for it will be generated

Example (Python):

    import tinyWRAP as t
    sip_cb = t.SipCallback()
    sip = t.SipStack(sip_cb, "123", "user@123", "impu@123")
    sip.start()

