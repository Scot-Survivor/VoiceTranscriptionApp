//
// Created by Josh Shiells on 31/01/2024.
//

#ifndef VOICETRANSCRIPTIONAPP_AUDIOPROCESSINGDEVICE_H
#define VOICETRANSCRIPTIONAPP_AUDIOPROCESSINGDEVICE_H
#include <vector>
struct AudioDeviceSettings {
    unsigned int deviceId;
};

class AudioProcessingDevice {
private:
    AudioDeviceSettings m_audioDeviceSettings{};
public:
    AudioProcessingDevice(const std::vector<unsigned int>& deviceIds, unsigned int deviceId);
    ~AudioProcessingDevice() = default;

    void setDeviceId(int deviceId) {
        m_audioDeviceSettings.deviceId = deviceId;
    }

    AudioDeviceSettings* getAudioDeviceSettings() {
        return &m_audioDeviceSettings;
    }

    static unsigned int find_device_idx_by_id(const std::vector<unsigned int>& deviceIds, unsigned int deviceId);
};


#endif //VOICETRANSCRIPTIONAPP_AUDIOPROCESSINGDEVICE_H
