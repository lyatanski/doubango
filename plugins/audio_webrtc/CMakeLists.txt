add_library(audio_webrtc SHARED
    audio_webrtc.cxx
    audio_webrtc_consumer.cxx
    audio_webrtc_producer.cxx
    audio_webrtc_transport.cxx
)
set_target_properties(audio_webrtc PROPERTIES PREFIX "")
target_link_libraries(audio_webrtc tinyDAV)

