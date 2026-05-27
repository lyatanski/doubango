# tinyWRAP

Multi-language bindings for the [Doubango](https://www.doubango.org/) IMS/SIP framework, generated via [SWIG](https://www.swig.org/). tinyWRAP wraps the full C++ API of `tinySIP`, `tinyDAV`, `tinyXCAP`, `tinyMSRP`, and `tinySMS` into Python, Java, and Lua.

## Table of Contents

- [Prerequisites & Build](#prerequisites--build)
- [Architecture Overview](#architecture-overview)
- [Quick Start](#quick-start)
- [SIP Stack](#sip-stack)
  - [Creating and configuring the stack](#creating-and-configuring-the-stack)
  - [Event callbacks](#event-callbacks)
  - [Stack lifecycle event codes](#stack-lifecycle-event-codes)
- [Registration (SIP REGISTER)](#registration-sip-register)
- [Voice and Video Calls (SIP INVITE)](#voice-and-video-calls-sip-invite)
  - [Outgoing call](#outgoing-call)
  - [Incoming call](#incoming-call)
  - [Call control](#call-control)
  - [Media configuration](#media-configuration)
- [Instant Messaging (SIP MESSAGE)](#instant-messaging-sip-message)
- [MSRP Messaging and File Transfer](#msrp-messaging-and-file-transfer)
- [Presence (SIP SUBSCRIBE / PUBLISH)](#presence-sip-subscribe--publish)
- [SIP OPTIONS](#sip-options)
- [XCAP](#xcap)
- [SMS Encoding](#sms-encoding)
- [Debug Callbacks](#debug-callbacks)
- [Media Session Manager](#media-session-manager)
- [Proxy Audio/Video Plugins](#proxy-audiovideo-plugins)
- [Java Notes](#java-notes)
- [Lua Notes](#lua-notes)
- [API Reference Summary](#api-reference-summary)

---

## Prerequisites & Build

**Required:**
- CMake ≥ 3.16
- SWIG ≥ 4.0

**Optional per language:**

| Language | Package needed |
|----------|---------------|
| Python | `python3-dev` (or equivalent) |
| Java | JDK (provides `jni.h`) |
| Lua | `liblua5.x-dev` |

Build the whole project from the repository root; tinyWRAP is built automatically when the respective language SDK is found:

```bash
cmake -S . -B out
cmake --build out/ -j $(nproc)
```

The Python module (`tinyWRAP`) is installed via pip into the active environment. The Java JNI library and Lua module land in the `out/` tree.

---

## Architecture Overview

```
SipStack          — lifecycle, transport, codec config
  └── SipCallback — virtual event sink (subclass to handle events)

SipSession        — base for all dialog types
  ├── InviteSession       — common INVITE logic (accept/hangup/reject)
  │     ├── CallSession         — audio/video calls + DTMF, hold, transfer
  │     └── MsrpSession         — MSRP file/message transfer
  ├── RegistrationSession — SIP REGISTER
  ├── MessagingSession    — SIP MESSAGE (instant messaging)
  ├── SubscriptionSession — SIP SUBSCRIBE / NOTIFY
  ├── PublicationSession  — SIP PUBLISH (presence)
  ├── InfoSession         — SIP INFO
  └── OptionsSession      — SIP OPTIONS

SipEvent          — base for all events delivered to SipCallback
  ├── StackEvent, DialogEvent
  ├── InviteEvent, MessagingEvent, InfoEvent
  ├── OptionsEvent, PublicationEvent
  ├── RegistrationEvent, SubscriptionEvent

XcapStack / XcapCallback / XcapEvent / XcapMessage — XCAP over HTTP
MsrpCallback / MsrpEvent / MsrpMessage             — MSRP protocol events
SMSEncoder / RPMessage / SMSData                   — SMS PDU encode/decode
MediaSessionMgr / Codec / QoS                      — per-call media control
ProxyPluginMgr / ProxyAudio* / ProxyVideo*         — custom media I/O
```

The central pattern is always the same:
1. Subclass a `*Callback` and override the virtual methods you care about.
2. Pass the callback to the stack or session constructor.
3. Call operations on the stack/session; the callback fires asynchronously.

---

## Quick Start

```python
import tinyWRAP as t
import time

class MyCallback(t.SipCallback):
    def OnStackEvent(self, e):
        print(f"Stack event code={e.getCode()} phrase={e.getPhrase()}")
        return 0

    def OnRegistrationEvent(self, e):
        print(f"Registration event code={e.getCode()} phrase={e.getPhrase()}")
        return 0

cb = MyCallback()
stack = t.SipStack(cb,
    "sip:example.com",        # realm (home domain)
    "alice@example.com",      # IMPI (private identity / auth username)
    "sip:alice@example.com"   # IMPU (public identity)
)
stack.setPassword("secret")
stack.setProxyCSCF("pcscf.example.com", 5060, "UDP", "ipv4")
stack.start()

reg = t.RegistrationSession(stack)
reg.setExpires(3600)
reg.register_()

time.sleep(5)
stack.stop()
```

---

## SIP Stack

### Creating and configuring the stack

```python
stack = t.SipStack(callback, realm_uri, impi_uri, impu_uri)
```

| Parameter | Description |
|-----------|-------------|
| `realm_uri` | Home domain URI, e.g. `"sip:example.com"` |
| `impi_uri` | Private identity used for authentication, e.g. `"alice@example.com"` |
| `impu_uri` | Public SIP address, e.g. `"sip:alice@example.com"` |

**Common configuration calls (all return `bool`):**

```python
stack.setDisplayName("Alice")                          # Caller-ID display name
stack.setPassword("secret")                            # Auth password (hex=True by default)
stack.setProxyCSCF("pcscf.example.com", 5060,
                   "UDP",   # transport: "UDP", "TCP", or "TLS"
                   "ipv4")  # IP version: "ipv4" or "ipv6"

stack.setLocalIP("0.0.0.0")                            # bind address
stack.setLocalPort(5061)                               # local SIP port

stack.setSTUNEnabled(True)                             # enable STUN
stack.setTLSSecAgree(True)                             # TLS security agreement
stack.setSSLCertificates("privkey.pem",
                         "pubkey.pem",
                         "ca.pem",
                         verify=True)

stack.addHeader("X-MyApp", "v1.0")                     # custom SIP header on all requests
stack.setEarlyIMS(False)                               # strict IMS mode
stack.setMaxFDs(1024)                                  # file descriptor limit

# Codec selection (combine flags with |)
t.SipStack.setCodecs(t.tmedia_codec_id_opus | t.tmedia_codec_id_pcma)
t.SipStack.setCodecPriority(t.tmedia_codec_id_opus, 0) # highest priority
```

**DNS helpers:**

```python
ip, port = stack.dnsNaptrSrv("example.com", "_sip._udp", None)
ip        = stack.dnsENUM("E2U+sip", "+15551234567", "e164.arpa")
ip, port  = stack.dnsSrv("_sip._tcp.example.com", None)
```

**Local address introspection:**

```python
addr_port = stack.getLocalIPnPort("UDP")   # returns "ip:port"
identity  = stack.getPreferredIdentity()   # returns preferred IMPU string
```

### Event callbacks

Subclass `SipCallback` and override any events you need. All methods receive a typed event object and must return `0` on success.

```python
class AppCallback(t.SipCallback):

    def OnStackEvent(self, e: t.StackEvent):
        # e.getCode() / e.getPhrase()
        return 0

    def OnDialogEvent(self, e: t.DialogEvent):
        return 0

    def OnRegistrationEvent(self, e: t.RegistrationEvent):
        ev_type = e.getType()
        if ev_type == t.tsip_register_event_type_e.tsip_i_register:
            session = e.takeSessionOwnership()  # incoming REGISTER
        return 0

    def OnInviteEvent(self, e: t.InviteEvent):
        ev_type = e.getType()
        if ev_type == t.tsip_invite_event_type_e.tsip_i_newcall:
            call = e.takeCallSessionOwnership()
            call.accept()
        return 0

    def OnMessagingEvent(self, e: t.MessagingEvent):
        msg  = e.getSipMessage()
        print(f"MESSAGE: code={msg.getCode()}")
        return 0

    def OnSubscriptionEvent(self, e: t.SubscriptionEvent):
        return 0

    def OnPublicationEvent(self, e: t.PublicationEvent):
        return 0

    def OnInfoEvent(self, e: t.InfoEvent):
        return 0

    def OnOptionsEvent(self, e: t.OptionsEvent):
        return 0
```

### Stack lifecycle event codes

`StackEvent.getCode()` returns one of:

| Code | Meaning |
|------|---------|
| 950 | Stack starting |
| 951 | Stack started (ready to use) |
| 952 | Stack stopping |
| 953 | Stack stopped |
| 954 | Stack failed to start |
| 955 | Stack failed to stop |
| 956 | Stack disconnected |

Dialog-level codes (on `DialogEvent`):

| Code | Meaning |
|------|---------|
| 700 | Transport error |
| 702 | Global error |
| 703 | Message error |
| 800–809 | Request states (incoming, outgoing, cancelled…) |
| 900 | Connecting |
| 901 | Connected |
| 902 | Terminating |
| 903 | Terminated |

---

## Registration (SIP REGISTER)

```python
reg = t.RegistrationSession(stack)
reg.setExpires(3600)                       # registration expiry in seconds
reg.addHeader("P-Access-Network-Info", "3GPP-E-UTRAN-FDD")
reg.register_()                            # send REGISTER

# ... later ...
reg.unRegister()                           # de-register (Expires: 0)
```

Handle the result in `OnRegistrationEvent`:

```python
def OnRegistrationEvent(self, e):
    code     = e.getCode()       # SIP response code, e.g. 200
    ev_type  = e.getType()       # tsip_register_event_type_e
    session  = e.getSession()    # RegistrationSession (read-only view)

    if code == 200:
        print("Registered successfully")
    elif code == 401 or code == 407:
        print("Auth challenge — credentials incorrect?")
    return 0
```

---

## Voice and Video Calls (SIP INVITE)

### Outgoing call

```python
call = t.CallSession(stack)

# Optional: per-call media settings
call.setVideoFps(30)
call.setVideoPrefSize(t.tmedia_pref_video_size_t.tmedia_pref_video_size_720p)
call.setVideoBandwidthUploadMax(2000)    # kbps
call.setSRtpMode(t.tmedia_srtp_mode_e.tmedia_srtp_mode_optional)
call.setICE(True)
call.setSTUNServer("stun.example.com", 3478)
call.setSTUNCred("user", "pass")
call.set100rel(True)                    # require provisional reliability
call.setSessionTimer(1800, "uas")

# Initiate call: media_type is a bitmask
media = t.twrap_media_type_t.twrap_media_audio_video
call.call("sip:bob@example.com", media)
```

**Media type flags** (combine with `|`):

| Constant | Meaning |
|----------|---------|
| `twrap_media_audio` | Audio only |
| `twrap_media_video` | Video only |
| `twrap_media_audio_video` | Audio + video |
| `twrap_media_t140` | Real-time text (T.140) |
| `twrap_media_msrp` | MSRP messaging |

### Incoming call

`takeCallSessionOwnership()` transfers lifecycle management to your code — you are now responsible for calling `hangup()`.

```python
def OnInviteEvent(self, e):
    if e.getType() == t.tsip_invite_event_type_e.tsip_i_newcall:
        call = e.takeCallSessionOwnership()

        config = t.ActionConfig()
        config.setResponseLine(180, "Ringing")
        call.accept(config)                  # or call.reject()
    return 0
```

### Call control

```python
call.hold()                                 # put on hold
call.resume()                               # resume from hold
call.sendDTMF(5)                            # send DTMF digit 0-15
call.transfer("sip:charlie@example.com")    # blind transfer (REFER)
call.acceptTransfer()                       # accept incoming REFER
call.rejectTransfer()                       # reject incoming REFER
call.hangup()                               # terminate call (BYE)
```

**Real-time text (T.140):**

```python
class MyT140(t.T140Callback):
    def ondata(self, d):
        buf = bytearray(d.getSize())
        d.getData(buf, len(buf))
        print("T.140:", buf.decode("utf-8", errors="replace"))
        return 0

t140_cb = MyT140()
call.setT140Callback(t140_cb)

# Send text
text = b"Hello over T.140"
call.sendT140Data(t.tmedia_t140_data_type_e.tmedia_t140_data_type_utf8,
                  text, len(text))
```

### Media configuration

Per-call overrides live on `CallSession`; global defaults live on `MediaSessionMgr` (static methods):

```python
# Global defaults (apply before calls are made)
t.MediaSessionMgr.defaultsSetAudioGain(0, 0)         # mic gain, spk gain (dB)
t.MediaSessionMgr.defaultsSetEchoSuppEnabled(True)
t.MediaSessionMgr.defaultsSetAgcEnabled(True)
t.MediaSessionMgr.defaultsSetNoiseSupEnabled(True)
t.MediaSessionMgr.defaultsSetVadEnabled(True)
t.MediaSessionMgr.defaultsSetVideoFps(25)
t.MediaSessionMgr.defaultsSetVideoMotionRank(1)       # 1=low, 2=medium, 3=high
t.MediaSessionMgr.defaultsSetPrefVideoSize(
    t.tmedia_pref_video_size_t.tmedia_pref_video_size_720p)
t.MediaSessionMgr.defaultsSetJbType(
    t.tmedia_jb_type_e.tmedia_jb_type_adaptive)
```

Access per-call QoS metrics after the call connects:

```python
def OnInviteEvent(self, e):
    if e.getType() == t.tsip_invite_event_type_e.tsip_ao_request:
        mgr = e.getSession().getMediaMgr()
        if mgr:
            qos = mgr.sessionGetQoS(t.twrap_media_audio)
            print(f"Bandwidth down={qos.getBandwidthDownKbps()} kbps")
            print(f"Video out: {qos.getVideoOutWidth()}x{qos.getVideoOutHeight()}")
    return 0
```

---

## Instant Messaging (SIP MESSAGE)

### Sending a message

```python
session = t.MessagingSession(stack)
session.setToUri("sip:bob@example.com")
session.addHeader("Content-Type", "text/plain")

body = b"Hello, Bob!"
session.send(body, len(body))
```

### Receiving a message

```python
def OnMessagingEvent(self, e):
    ev_type = e.getType()
    msg     = e.getSipMessage()

    if ev_type == t.tsip_message_event_type_e.tsip_i_message:
        session = e.takeSessionOwnership()
        ct = msg.getSipHeaderValue("Content-Type")
        print(f"Received MESSAGE Content-Type={ct}")
        session.accept()
    return 0
```

---

## MSRP Messaging and File Transfer

MSRP runs over a dedicated INVITE session. Create an `MsrpSession` with a callback, then call `callMsrp()`.

```python
class MyMsrp(t.MsrpCallback):
    def OnEvent(self, e):
        msg = e.getMessage()
        if msg and not msg.isRequest():
            print(f"MSRP response: {msg.getCode()} {msg.getPhrase()}")
        elif msg and msg.isRequest():
            length = msg.getMsrpContentLength()
            buf    = bytearray(length)
            msg.getMsrpContent(buf, length)
            start, end, total = msg.getByteRange(None, None, None)
            print(f"MSRP chunk [{start}-{end}/{total}]: {buf.decode()}")
        return 0

msrp_cb = MyMsrp()
msrp    = t.MsrpSession(stack, msrp_cb)
msrp.callMsrp("sip:bob@example.com")

# After the session is established (OnInviteEvent → tsip_i_msrp_newcall / connected):
data = b"Hello via MSRP"
msrp.sendMessage(data, len(data))

# File transfer (server must have Content-Disposition configured in ActionConfig)
cfg = t.ActionConfig()
cfg.setMediaString(t.twrap_media_msrp, "file-path", "/tmp/report.pdf")
cfg.setMediaString(t.twrap_media_msrp, "file-type", "application/pdf")
msrp.sendFile(cfg)
```

---

## Presence (SIP SUBSCRIBE / PUBLISH)

### Publishing presence (SIP PUBLISH)

```python
pub = t.PublicationSession(stack)
pub.setToUri("sip:alice@example.com")   # own IMPU
pub.setExpires(3600)

pidf = b"""<?xml version="1.0"?>
<presence xmlns="urn:ietf:params:xml:ns:pidf" entity="sip:alice@example.com">
  <tuple id="t1"><status><basic>open</basic></status></tuple>
</presence>"""
pub.publish(pidf, len(pidf))

# ... later ...
pub.unPublish()
```

### Subscribing to a resource (SIP SUBSCRIBE)

```python
sub = t.SubscriptionSession(stack)
sub.setToUri("sip:bob@example.com")
sub.addHeader("Event", "presence")
sub.addHeader("Accept", "application/pidf+xml")
sub.setExpires(3600)
sub.subscribe()

def OnSubscriptionEvent(self, e):
    msg = e.getSipMessage()
    if e.getType() == t.tsip_subscribe_event_type_e.tsip_i_notify:
        print("NOTIFY body:", msg.getSdpHeaderValue("o"))   # raw message access
    return 0

# Unsubscribe
sub.unSubscribe()
```

---

## SIP OPTIONS

```python
opts = t.OptionsSession(stack)
opts.setToUri("sip:bob@example.com")
opts.send()

def OnOptionsEvent(self, e):
    if e.getCode() == 200:
        msg = e.getSipMessage()
        allow = msg.getSipHeaderValue("Allow")
        print(f"Remote capabilities: {allow}")
    return 0
```

---

## XCAP

XCAP lets you read and write XML documents stored on an XCAP server (typically a presence/contact-list server).

```python
class MyXcap(t.XcapCallback):
    def onEvent(self, e):
        xmsg = e.getXcapMessage()
        print(f"XCAP {e.getType()}: {xmsg.getCode()} {xmsg.getPhrase()}")
        if xmsg.getCode() == 200:
            length = xmsg.getXcapContentLength()
            buf    = bytearray(length)
            xmsg.getXcapContent(buf, length)
            print("Body:", buf.decode())
        return 0

xcap_cb = MyXcap()
xcap = t.XcapStack(xcap_cb,
    "alice@example.com",              # XUI
    "secret",                         # password
    "https://xcap.example.com/xcap"   # XCAP root
)
xcap.start()

# Fetch the resource-lists document
xcap.getDocument("https://xcap.example.com/xcap/resource-lists/users/alice@example.com/index")

# Build a selector for a specific element
sel = t.XcapSelector(xcap)
sel.setAUID("resource-lists")
sel.setName("list")
sel.setAttribute("list", "name", "friends")
url = sel.getString()
xcap.getElement(url)

# Upload a new document
doc = b'<resource-lists xmlns="urn:ietf:params:xml:ns:resource-lists"/>'
xcap.putDocument(
    "https://xcap.example.com/xcap/resource-lists/users/alice@example.com/index",
    doc, len(doc), "application/resource-lists+xml"
)

xcap.stop()
```

**Register a custom AUID before fetching it:**

```python
xcap.registerAUID(
    "my-app-config",                      # AUID
    "application/my-app+xml",             # MIME type
    "urn:example:myapp",                  # namespace
    "config",                             # default document name
    False                                 # per-user (not global)
)
```

---

## SMS Encoding

`SMSEncoder` provides static methods to encode and decode GSM/3GPP RP-DATA SMS PDUs for transport over IMS.

```python
# Encode a SUBMIT PDU
rp_data = t.SMSEncoder.encodeSubmit(
    0,                  # message reference
    "+15551234567",     # destination address (DA)
    "Hello SMS!"        # text body
)

# Encode a DELIVER PDU (mobile-terminated)
rp_data = t.SMSEncoder.encodeDeliver(
    "+15559876543",     # originating address (OA)
    "Hi there"
)

# Encode ACK / Error
ack   = t.SMSEncoder.encodeACK(0, t.tmsrp_request_type_e.tmsrp_SEND)
error = t.SMSEncoder.encodeError(0, t.tmsrp_request_type_e.tmsrp_SEND, 127)

# Decode an incoming RP-DATA binary payload
buf     = b"\x01\x06\x91..."    # raw RP bytes
rp_msg  = t.SMSEncoder.decode(buf, len(buf), False)

sms_data = rp_msg.getPayload()
print(f"From: {sms_data.getOA()}, To: {sms_data.getDA()}")
length = sms_data.getPayload()   # binary SMS payload
```

---

## Debug Callbacks

Redirect the framework's internal log output to your own handler:

```python
class AppDebug(t.DDebugCallback):
    def OnDebugInfo(self, msg):
        print(f"[INFO]  {msg}")
        return 0
    def OnDebugWarn(self, msg):
        print(f"[WARN]  {msg}")
        return 0
    def OnDebugError(self, msg):
        print(f"[ERROR] {msg}")
        return 0
    def OnDebugFatal(self, msg):
        print(f"[FATAL] {msg}")
        return 0

debug_cb = AppDebug()
t.DDebug.setCallback(debug_cb)
```

---

## Media Session Manager

`MediaSessionMgr` exposes per-call media control as well as static defaults that apply to all new calls.

**Per-call access:**

```python
def OnInviteEvent(self, e):
    if e.getType() == t.tsip_invite_event_type_e.tsip_ao_request:
        mgr = e.getSession().getMediaMgr()
        if mgr:
            # Read the negotiated codec
            codec = mgr.producerGetCodec(t.twrap_media_audio)
            print(f"Audio codec: {codec.getName()} @ {codec.getAudioSamplingRate()} Hz")

            # Adjust mic gain at runtime
            mgr.producerSetInt32(t.twrap_media_audio, "gain", 3)
    return 0
```

**Global defaults (call before `stack.start()`):**

```python
t.MediaSessionMgr.defaultsSetProfile(t.tmedia_profile_e.tmedia_profile_rtcweb)
t.MediaSessionMgr.defaultsSetBandwidthLevel(
    t.tmedia_bandwidth_level_e.tmedia_bandwidth_level_high)

# Audio
t.MediaSessionMgr.defaultsSetAudioChannels(1, 1)      # record, play
t.MediaSessionMgr.defaultsSetAudioPtime(20)
t.MediaSessionMgr.defaultsSetEchoSuppEnabled(True)
t.MediaSessionMgr.defaultsSetAgcEnabled(True)
t.MediaSessionMgr.defaultsSetNoiseSupEnabled(True)
t.MediaSessionMgr.defaultsSetVadEnabled(True)
t.MediaSessionMgr.defaultsSetAudioGain(0, 0)          # dB mic, dB spk

# Video
t.MediaSessionMgr.defaultsSetVideoFps(30)
t.MediaSessionMgr.defaultsSetVideoMotionRank(2)
t.MediaSessionMgr.defaultsSetPrefVideoSize(
    t.tmedia_pref_video_size_t.tmedia_pref_video_size_1080p)
t.MediaSessionMgr.defaultsSetVideoMaxBandwidthUp(4000)   # kbps
t.MediaSessionMgr.defaultsSetVideoMaxBandwidthDown(4000)

# SRTP / security
t.MediaSessionMgr.defaultsSetSRtpMode(
    t.tmedia_srtp_mode_e.tmedia_srtp_mode_mandatory)
t.MediaSessionMgr.defaultsSetSRtpType(
    t.tmedia_srtp_type_e.tmedia_srtp_type_sdes_dtls)

# ICE / STUN / TURN
t.MediaSessionMgr.defaultsSetIceEnabled(True)
t.MediaSessionMgr.defaultsSetStunEnabled(True)
t.MediaSessionMgr.defaultsSetStunServer("stun.example.com", 3478)
t.MediaSessionMgr.defaultsSetTurnServer("turn.example.com", 3478)
t.MediaSessionMgr.defaultsSetStunCred("user", "pass")

# RTCP
t.MediaSessionMgr.defaultsSetRtcpEnabled(True)
t.MediaSessionMgr.defaultsSetRtcpMuxEnabled(True)
t.MediaSessionMgr.defaultsSetAvpfMode(t.tmedia_mode_e.tmedia_mode_optional)
```

---

## Proxy Audio/Video Plugins

The proxy plugin system lets you inject custom audio/video capture and rendering code written in Python (or Java/Lua) without touching any native code.

```python
class MyAudioConsumer(t.ProxyAudioConsumerCallback):
    """Receives decoded PCM audio from the framework for playback."""
    def prepare(self, ptime, rate, channels):
        print(f"Audio consumer: ptime={ptime}ms, {rate}Hz, ch={channels}")
        return 0
    def start(self):  return 0
    def pause(self):  return 0
    def stop(self):   return 0
    def consume(self, buf, len_bytes, type_):
        # write buf to audio output device
        return 0

class MyAudioProducer(t.ProxyAudioProducerCallback):
    """Supplies PCM audio to the framework for encoding and sending."""
    def prepare(self, ptime, rate, channels):
        return 0
    def start(self):  return 0
    def pause(self):  return 0
    def stop(self):   return 0
    def fillPushBuffer(self):
        # called when the framework needs more audio data
        data = mic_device.read(frame_size)
        self.push(data, len(data))
        return 0

class MyPluginMgrCb(t.ProxyPluginMgrCallback):
    def OnPluginCreated(self, id_, type_):
        mgr = t.ProxyPluginMgr.getInstance()
        if type_ == t.twrap_proxy_plugin_audio_consumer:
            consumer = mgr.findAudioConsumer(id_)
            consumer.setCallback(MyAudioConsumer())
        elif type_ == t.twrap_proxy_plugin_audio_producer:
            producer = mgr.findAudioProducer(id_)
            producer.setCallback(MyAudioProducer())
        return 0
    def OnPluginDestroyed(self, id_, type_):
        return 0

plugin_cb = MyPluginMgrCb()
t.ProxyPluginMgr.createInstance(plugin_cb)
```

> Proxy plugins must be registered **before** `stack.start()`. The framework creates plugin instances as needed for each call.

---

## Java Notes

Add the generated JNI library to `java.library.path`, then import the module:

```java
import org.doubango.tinyWRAP.*;

public class App {
    static { System.loadLibrary("tinyWRAP"); }

    static class MyCallback extends SipCallback {
        @Override public int OnRegistrationEvent(RegistrationEvent e) {
            System.out.println("Reg code=" + e.getCode());
            return 0;
        }
        @Override public int OnInviteEvent(InviteEvent e) {
            if (e.getType() == tsip_invite_event_type_e.tsip_i_newcall) {
                CallSession call = e.takeCallSessionOwnership();
                call.accept(null);
            }
            return 0;
        }
    }

    public static void main(String[] args) throws Exception {
        MyCallback cb = new MyCallback();
        SipStack stack = new SipStack(cb,
            "sip:example.com", "alice@example.com", "sip:alice@example.com");
        stack.setPassword("secret");
        stack.setProxyCSCF("pcscf.example.com", (short)5060, "UDP", "ipv4");
        stack.start();

        RegistrationSession reg = new RegistrationSession(stack);
        reg.setExpires(3600);
        reg.register_();

        Thread.sleep(5000);
        stack.stop();
    }
}
```

---

## Lua Notes

```lua
local t = require("tinyWRAP")

local MyCallback = t.SipCallback:new_local()
function MyCallback:OnRegistrationEvent(e)
    print("Reg code=" .. e:getCode())
    return 0
end
function MyCallback:OnInviteEvent(e)
    if e:getType() == t.tsip_i_newcall then
        local call = e:takeCallSessionOwnership()
        call:accept(nil)
    end
    return 0
end

local cb    = MyCallback()
local stack = t.SipStack(cb,
    "sip:example.com", "alice@example.com", "sip:alice@example.com")
stack:setPassword("secret")
stack:setProxyCSCF("pcscf.example.com", 5060, "UDP", "ipv4")
stack:start()

local reg = t.RegistrationSession(stack)
reg:setExpires(3600)
reg:register_()

os.execute("sleep 5")
stack:stop()
```

---

## API Reference Summary

### SipStack

| Method | Description |
|--------|-------------|
| `SipStack(cb, realm, impi, impu)` | Create stack |
| `start() / stop()` | Lifecycle |
| `setPassword(pwd)` | Auth password |
| `setProxyCSCF(host, port, transport, ipver)` | Outbound proxy |
| `setDisplayName(name)` | Caller-ID display name |
| `setLocalIP(ip) / setLocalPort(port)` | Bind address |
| `setSTUNEnabled(bool)` | STUN |
| `setTLSSecAgree(bool) / setSSLCertificates(...)` | TLS |
| `addHeader(name, value)` | Global SIP header |
| `setCodecs(flags)` *(static)* | Allowed codecs |
| `isValid()` | Check stack is alive |

### SipSession (base for all sessions)

| Method | Description |
|--------|-------------|
| `setToUri(uri) / setFromUri(uri)` | Target / source identity |
| `setExpires(seconds)` | Expires header |
| `addHeader(name, value)` | Per-session SIP header |
| `addCaps(name, value)` | Contact capabilities |
| `setSilentHangup(bool)` | Terminate without BYE |
| `getId()` | Internal session ID |

### CallSession

| Method | Description |
|--------|-------------|
| `call(uri, media_type)` | Initiate call |
| `accept() / reject() / hangup()` | Answer / decline / terminate |
| `hold() / resume()` | Call hold |
| `sendDTMF(digit)` | DTMF 0–15 |
| `transfer(uri)` | Blind transfer |
| `setICE(bool) / setSTUNServer(host, port)` | ICE/STUN |
| `setSRtpMode(mode)` | SRTP mode |
| `setVideoFps(fps)` | Per-call video FPS |
| `getMediaMgr()` | Access MediaSessionMgr |

### RegistrationSession

| Method | Description |
|--------|-------------|
| `register_()` | Send REGISTER |
| `unRegister()` | Deregister |
| `accept() / reject()` | Server-side registration handling |

### MessagingSession

| Method | Description |
|--------|-------------|
| `send(payload, len)` | Send SIP MESSAGE |
| `accept() / reject()` | Respond to incoming MESSAGE |

### MsrpSession

| Method | Description |
|--------|-------------|
| `callMsrp(uri)` | Establish MSRP session |
| `sendMessage(payload, len)` | Send MSRP message chunk |
| `sendFile(config)` | Initiate file transfer |
| `setCallback(cb)` | Attach MsrpCallback |

### XcapStack

| Method | Description |
|--------|-------------|
| `XcapStack(cb, xui, password, xcap_root)` | Create XCAP stack |
| `getDocument(url)` | HTTP GET document |
| `putDocument(url, data, len, ct)` | HTTP PUT document |
| `deleteDocument(url)` | HTTP DELETE document |
| `getElement(url) / putElement(...) / deleteElement(url)` | Element-level operations |
| `getAttribute(url) / putAttribute(...) / deleteAttribute(url)` | Attribute-level operations |
| `registerAUID(id, mime, ns, docname, global)` | Register custom AUID |

### SMSEncoder *(static methods)*

| Method | Description |
|--------|-------------|
| `encodeSubmit(mr, da, text)` | Encode MO-SMS (Submit) |
| `encodeDeliver(oa, text)` | Encode MT-SMS (Deliver) |
| `encodeACK(mr, type)` | Encode RP-ACK |
| `encodeError(mr, type, cause)` | Encode RP-ERROR |
| `decode(buf, len, MO)` | Decode RP-DATA bytes → RPMessage |
