# Welcome to [doubango](https://lyatanski.github.io/doubango/) project!

[3GPP IMS](http://en.wikipedia.org/wiki/IP_Multimedia_Subsystem) (IP Multimedia Subsystem) is the next generation network for delivering
IP multimedia services. IMS is standardized by the 3rd Generation Partnership Project (3GPP).
IMS services could be used over any type of network, such as [3GPP LTE](http://en.wikipedia.org/wiki/3GPP_Long_Term_Evolution), GPRS, Wireless LAN, CDMA2000 or fixed line.

[doubango](https://lyatanski.github.io/doubango/) is a mature, open source, 3GPP IMS/LTE framework for both embedded and desktop systems.
The framework is written in ANSI-C to ease portability and has been carefully designed to efficiently work on embedded systems with limited memory and low computing power and to be extremely portable. [doubango](https://lyatanski.github.io/doubango/) supports both Voice and SMS over LTE as defined by the **One Voice initiative**.


## Features

*   SIP (RFC 3261, 3GPP TS 24.229 Rel-9)
*   IMS-AKA (RFC 3310, 3GPP TS 33.203, 35.205, 35.206, 35.207, 35.208 and 35.909)
*   Private extension headers for 3GPP
*   IPv4/IPv6 dual stack
*   UDP, TCP and TLS
*   Service-Route Discovery (RFC 3608)
*   Proxy-CSCF discovery using DHCPv4/v6 or/and DNS NAPTR
*   SigComp (RFC 3320, 3485, 4077, 4464, 4465, 4896, 5049, 5112 and 1951)
*   IPSec
*   Security Agreement (RFC 3329)
*   NAT Traversal (STUN, TURN and ICE)
*   Preconditions (RFC 3312, 4032 and 5027)
*   SMS over IP (3GPP TS 23.038, 24.040, 24.011, 24.341 and 24.451)
*   ENUM (RFC 3761)
*   The tel URI for Telephone Numbers (RFC 3966)
*   SIP SIMPLE (Presence Subscription/Publication and Pager Mode IM)
*   MMTel (UNI)
*   SDP (RFC 3264)
*   (S)RTP and (S)RTCP
*   (S)AVP, (S)AVP(F) and UDP/TLS/RTP/(S)AVP(F) profiles
*   DTLS-SRTP(RFC 5763 and 5764)
*   XCAP (RFC 4825)
*   MSRP (RFC 4975 and 5547)
*   Audio/Video (AMR, Speex, G.729, G.722, Opus, G.711, GSM, iLBC, VP8, H.264, H.263, Theora, MP4V-ES...)
*   Full HD (1080p) video
*   T.140: Text Over IP ([http://en.wikipedia.org/wiki/Text\_over\_IP](http://en.wikipedia.org/wiki/Text_over_IP))
*   ...and many other features


## Multimedia Telephony (MMTel)

*   MMTel Supplementary Services (3GPP TS 24.173, 24607, 24.608, 24.607, 24.604, 24.611, 24.610, 24.606, 24.615 and 24.605)
*   Supplementary Service Configuration(3GPP TS 24.623)
*   Ad-Hoc Multi Party Conference (3GPP TS 24.605)
*   Communication Waiting(3GPP TS 24.615)
*   Message Waiting Indication(3GPP TS 24.606 and RFC 3842)
*   Originating/Terminating Identification Restriction(3GPP TS 24.607/608)
*   Communication Diversion(3GPP TS 24.604)
*   Communication Barring(3GPP TS 24.611)


## Projects

| Name | Prefix | Functionalities | Dependencies |
| ---- | ------ | --------------- | ------------ |
| tinySAK (tiny Swiss Army Knife) | tsk | ANSI-C Object Programing<br>Linked lists<br>String utility functions<br>Memory management<br>Dynamic buffers<br>Threading<br>Runnable<br>Mutexes<br>Semaphores<br>Conditional Variables<br>Timers<br>Time<br>Final State Machine (FSM) manager<br>Base64 encoder/decoder<br>UUID generator<br>CRC32 and CRC16<br>URL encoder/decoder<br>SHA-1, MD5, HMAC-MD5, HMAC-SHA-1 | None |
| tinyNET (Networking) | tnet | IPv4/IPv6 Sockets (UDP, TCP, TLS and SCTP)<br>DHCPv4/v6<br>DNS (NAPTR, PTR, SRV, MX, A, AAAA, OPT, CNAME ...)<br>ENUM<br>NAT Traversal (STUN, TURN and ICE)| tinySAK |
| tinyHTTP (HTTP/HTTPS stack) | thttp | Digest/Basic Authentication<br>Pipelining<br>CONNECT, DELETE, GET, HEAD, OPTIONS, PATCH, POST, PUT and TRACE | tinySAK<br>tinyNET |
| tinyXCAP (XML Configuration Access Protocol) | txcap | 3GPP 24.623<br>AUID manager<br>URL generator | tinySAK<br>tinyNET<br>tinyHTTP |
| tinySMS (Binary SMS over IMS/LTE networks) | tsms | SM-TL (3GPP TS 23.040)<br>SM-RL (3GPP TS 24.011)<br>SMS over 3GPP IMS/LTE networks (3GPP TS 24.341)| tinySAK |
| tinySigComp (Signalling Compression) | tcomp | Stream and Dgram compression/decompression<br>SIP/SDP and Presence dictionaries<br>Deflate Compressor<br>UDVM | tinySAK |
| tinyIPSec (IPSec) | tipsec | Mode: Transport and Tunnel<br>Protocol: AH, ESP or both<br>IP Protocol: UDP and TCP<br>Algorithm: HMAC-MD5-96 and HMAC-SHA-1-96<br>Encryption Algorithm: NULL, DES-EDE3-CBC and AES | tinySAK |
| tinySDP (Session Description Protocol) | tsdp | SDP Offer Answer (RFC 3262) | tinySAK |
| tinyMSRP (Message Session Relay Protocol) | tmsrp | Large Message IM (RFC 4975 and OMA SIMPLE IM)<br>File Transfer (RFC 5547)<br>Image Sharing (GSMA IR.79) | tinySDP |
| tinyMEDIA (Audio, Video, File Transfer ...) | tmedia | Plugins<br>Codecs | tinySAK<br>tinyNET<br>tinySDP |
| tinySIP (Session Initiation Protocol) | tsip | SIP (RFC 3261, 3GPP TS 24.229 Rel-9)<br>IMS-AKA (RFC 3310, 3GPP TS 33.203)<br>IPv4/IPv6 dual stack<br>UDP, TCP, TLS and SCTP transports<br>Service-Route Discovery (RFC 3608)<br>Proxy-CSCF discovery using DHCPv4/v6 or/and DNS<br>NAPTR+SRV<br>SigComp (RFC 3320, 3485, 4077, 4464, 4465, 4896, 5049, 5112 and 1951)<br>IPSec<br>Security Agreement (RFC 3329)<br>NAT Traversal (STUN, TURN and ICE)<br>Preconditions (RFC 3312, 4032 and 5027)<br>SMS over IP (3GPP TS 23.038, 24.040, 24.011, 24.341 and 24.451)<br>ENUM (RFC 3761)<br>The tel URI for Telephone Numbers (RFC 3966)<br>SIP SIMPLE (Presence subsciption/publication, Pager Mode IM, ...)<br>MMTel (UNI)<br>SDP Offer-Answer (SOA)<br>Session Timers<br>File transfer (RFC 5547) and Image Sharing(GSMA IR.79)<br>Large Message IM (OMA SIMPLE IM) | tinySAK<br>tinyNET<br>tinySDP<br>tinyMEDIA<br>tinyHTTP<br>tinyIPSec |


## Proof Of Concept

**Client-side components**

* [Boghe](https://github.com/DoubangoTelecom/boghe/)
    * SIP Video Client for Windows Phone 8 and Surface Pro
    * IMS/RCS Client for Windows XP, Vista, 7 and 8
* [IMSDroid](https://github.com/DoubangoTelecom/imsdroid/)
    * SIP/IMS Client for Android
* [iDoubs](https://github.com/DoubangoTelecom/idoubs/)
    * SIP/IMS VideoPhone for iOS (iPhone, iPad and iPod Touch) and MAC OS X


**Server-side components**

* [webrtc2sip](https://github.com/DoubangoTelecom/webrtc2sip)
    * Smart SIP and Media Gateway to connect WebRTC endpoints to any SIP-legacy network
* [telepresence](https://github.com/DoubangoTelecom/telepresence/)
    * the open source SIP TelePresence system with a porwerfull [MCU](http://en.wikipedia.org/wiki/Multipoint_Control_Unit)
* [Flash2IMS](https://github.com/DoubangoTelecom/flash2ims/)
    * Adobe® Flash® to SIP/IMS Gateway.

See also:
* [webrtc4all](https://github.com/DoubangoTelecom/webrtc4all/)
* [sipML5](https://github.com/DoubangoTelecom/sipml5/)
* [click2dial](https://github.com/DoubangoTelecom/click-2-dial/)


## Building

```
cmake -S . -B out --graphviz=out/dep.dot dot -Tpng -o out/dep.png out/dep.dot
cmake --build out/ -j 16
cd out/
ctest -V
```

**© 2010-2015 Doubango Telecom**
_Inspiring the future_
