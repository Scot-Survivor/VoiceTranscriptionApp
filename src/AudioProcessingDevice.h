//
// Created by Josh Shiells on 31/01/2024.
//

#ifndef VOICETRANSCRIPTIONAPP_AUDIOPROCESSINGDEVICE_H
#define VOICETRANSCRIPTIONAPP_AUDIOPROCESSINGDEVICE_H
#include <vector>
#include <string>
#include <thread>
#include <cmath>
#include "RtAudio.h"
#include "SafeQueue.h"

struct AudioDeviceSettings {
    unsigned int deviceId;
};

struct AudioFrame {
    float* data;
    unsigned int size;
    double streamTime;
};

class AudioProcessingDevice {
private:
    AudioDeviceSettings m_audioDeviceSettings{};
    RtAudio* m_dac;
    std::vector<unsigned int> m_deviceIds;
    SafeQueue<AudioFrame> m_audioFrameQueue;
public:
    AudioProcessingDevice(RtAudio* dac, unsigned int deviceId);
    AudioProcessingDevice(unsigned int deviceId);
    ~AudioProcessingDevice() = default;

    void setDeviceId(unsigned int deviceId) {
        m_audioDeviceSettings.deviceId = deviceId;
    }

    AudioDeviceSettings* getAudioDeviceSettings() {
        return &m_audioDeviceSettings;
    }

    static unsigned int find_device_idx_by_id(const std::vector<unsigned int>& deviceIds, unsigned int deviceId);
    unsigned int find_device_idx_by_name(const std::string& deviceName);
    unsigned int find_device_id_by_name(const std::string& deviceName);
    std::vector<std::string> get_device_names();
    std::vector<unsigned int> get_device_ids();

    void start_stream();

    std::vector<float *> pop_audio_frames();
    std::vector<float *> pop_audio_frames(double min, double max);
};


#endif //VOICETRANSCRIPTIONAPP_AUDIOPROCESSINGDEVICE_H
