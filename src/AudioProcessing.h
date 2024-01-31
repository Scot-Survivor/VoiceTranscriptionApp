//
// Created by Josh Shiells on 31/01/2024.
//

#ifndef VOICETRANSCRIPTIONAPP_AUDIOPROCESSING_H
#define VOICETRANSCRIPTIONAPP_AUDIOPROCESSING_H

struct AudioDeviceSettings {
    int deviceId;
};

class AudioProcessing {
private:
    AudioDeviceSettings m_audioDeviceSettings;
public:
    AudioProcessing() = default;
    ~AudioProcessing() = default;

    void setDeviceId(int deviceId) {
        m_audioDeviceSettings.deviceId = deviceId;
    }

    AudioDeviceSettings* getAudioDeviceSettings() {
        return &m_audioDeviceSettings;
    }
};


#endif //VOICETRANSCRIPTIONAPP_AUDIOPROCESSING_H
