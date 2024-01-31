//
// Created by Josh Shiells on 31/01/2024.
//

#ifndef VOICETRANSCRIPTIONAPP_AUDIOPROCESSINGDEVICE_H
#define VOICETRANSCRIPTIONAPP_AUDIOPROCESSINGDEVICE_H
#include <vector>
#include <string>
#include "RtAudio.h"

struct AudioDeviceSettings {
    unsigned int deviceId;
};

class AudioProcessingDevice {
private:
    AudioDeviceSettings m_audioDeviceSettings{};
    RtAudio* m_dac;
    std::vector<unsigned int> m_deviceIds;
public:
    AudioProcessingDevice(RtAudio* dac, unsigned int deviceId);
    ~AudioProcessingDevice() = default;

    void setDeviceId(unsigned int deviceId) {
        m_audioDeviceSettings.deviceId = deviceId;
    }

    AudioDeviceSettings* getAudioDeviceSettings() {
        return &m_audioDeviceSettings;
    }

    static unsigned int find_device_idx_by_id(const std::vector<unsigned int>& deviceIds, unsigned int deviceId);
    unsigned int find_device_idx_by_name(const std::string& deviceName);
    std::vector<std::string> get_device_names();
    std::vector<unsigned int> get_device_ids();
};


#endif //VOICETRANSCRIPTIONAPP_AUDIOPROCESSINGDEVICE_H
