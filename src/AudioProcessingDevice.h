//
// Created by Josh Shiells on 31/01/2024.
//

#ifndef VOICETRANSCRIPTIONAPP_AUDIOPROCESSINGDEVICE_H
#define VOICETRANSCRIPTIONAPP_AUDIOPROCESSINGDEVICE_H

struct AudioDeviceSettings {
    int deviceId;
};

class AudioProcessingDevice {
private:
    AudioDeviceSettings m_audioDeviceSettings;
public:
    AudioProcessingDevice() = default;
    ~AudioProcessingDevice() = default;

    void setDeviceId(int deviceId) {
        m_audioDeviceSettings.deviceId = deviceId;
    }

    AudioDeviceSettings* getAudioDeviceSettings() {
        return &m_audioDeviceSettings;
    }
};


#endif //VOICETRANSCRIPTIONAPP_AUDIOPROCESSINGDEVICE_H
