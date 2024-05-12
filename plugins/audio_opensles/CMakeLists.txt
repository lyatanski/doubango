add_library(audio_opensles SHARED
    audio_opensles.cxx
    audio_opensles_consumer.cxx
    audio_opensles_device_impl.cxx
    audio_opensles_producer.cxx
    audio_opensles_device.cxx
)
set_target_properties(audio_opensles PROPERTIES PREFIX "")
target_link_libraries(audio_opensles tinyMEDIA tinyDAV)

