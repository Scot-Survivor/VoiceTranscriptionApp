//
// Created by Josh Shiells on 31/01/2024.
//

#include "AudioProcessingDevice.h"

int record( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
            double streamTime, RtAudioStreamStatus status, void *userData )
{
    if ( status ) {
        std::cout << "Stream overflow detected!" << std::endl;
    }
    else {
        auto audioFrameQueue = (SafeQueue<AudioFrame>*)userData;

        // Do something with the data in the "inputBuffer" buffer.
        if ( inputBuffer ) {
            std::cout << "Stream time: " << streamTime << std::endl;
            // memcpy( userData, inputBuffer, 256 * sizeof( short ) );
            // auto temp = (float*)inputBuffer;
            AudioFrame audioFrame = {};
            //std::copy(temp, temp + nBufferFrames, audioFrame.data);
            audioFrame.data = (int16_t *)inputBuffer;
            audioFrame.size = nBufferFrames;
            audioFrame.streamTime = streamTime;
            audioFrameQueue->push(audioFrame);
        }
    }
    return 0;
}

unsigned int AudioProcessingDevice::find_device_idx_by_id(const std::vector<unsigned int>& deviceIds, unsigned int deviceId) {
    for (unsigned int i = 0; i < deviceIds.size(); i++) {
        if (deviceIds[i] == deviceId) {
            return i;
        }
    }
    return -1;
}

AudioProcessingDevice::AudioProcessingDevice(RtAudio* dac, unsigned int deviceId) {
    m_dac = dac;
    m_deviceIds = m_dac->getDeviceIds();
    m_audioDeviceSettings = {};
    m_audioDeviceSettings.deviceId =  deviceId;
}

AudioProcessingDevice::AudioProcessingDevice(unsigned int deviceId) {
    m_dac = new RtAudio();
    m_deviceIds = m_dac->getDeviceIds();
    m_audioDeviceSettings = {};
    m_audioDeviceSettings.deviceId = deviceId;
}

std::vector<std::string> AudioProcessingDevice::get_device_names() {
    std::vector<std::string> deviceNames;
    for (unsigned int deviceId : m_deviceIds) {
        RtAudio::DeviceInfo info = m_dac->getDeviceInfo(deviceId);
        deviceNames.push_back(info.name);
    }
    return deviceNames;
}

std::vector<unsigned int> AudioProcessingDevice::get_device_ids() {
    return m_deviceIds;
}

unsigned int AudioProcessingDevice::find_device_idx_by_name(const std::string &deviceName) {
    for (unsigned int i = 0; i < m_deviceIds.size(); i++) {
        RtAudio::DeviceInfo info = m_dac->getDeviceInfo(m_deviceIds[i]);
        if (info.name == deviceName) {
            return i;
        }
    }
    return -1;
}

void AudioProcessingDevice::start_stream() {
    if ( m_dac->isStreamOpen() ) return;

    RtAudio::DeviceInfo info = m_dac->getDeviceInfo(m_audioDeviceSettings.deviceId);
    RtAudio::StreamParameters parameters;
    parameters.deviceId = m_audioDeviceSettings.deviceId;
    parameters.nChannels = std::max(info.inputChannels, (unsigned int) 1);
    parameters.firstChannel = 0;
    unsigned int sampleRate = info.preferredSampleRate;
    unsigned int bufferFrames = 256; // 256 sample frames

    if ( m_dac->openStream( nullptr, &parameters, RTAUDIO_SINT16,
                         sampleRate, &bufferFrames, &record, &m_audioFrameQueue ) ) {
        std::cout << '\n' <<  m_dac->getErrorText() << '\n' << std::endl;
        exit( 0 ); // problem with device settings
    }

    // Stream is open ... now start it.
    if ( m_dac->startStream() ) {
        std::cout << m_dac->getErrorText() << std::endl;
        if ( m_dac->isStreamOpen() ) m_dac->closeStream();
    }
}

unsigned int AudioProcessingDevice::find_device_id_by_name(const std::string &deviceName) {
    for (unsigned int m_deviceId : m_deviceIds) {
        RtAudio::DeviceInfo info = m_dac->getDeviceInfo(m_deviceId);
        if (info.name == deviceName) {
            return m_deviceId;
        }
    }
    return -1;
}

std::vector<float *> AudioProcessingDevice::pop_audio_frames() {
    std::vector<float *> audioFrames;
    std::optional<AudioFrame> frame = m_audioFrameQueue.pop();
    while (frame.has_value()) {
        audioFrames.push_back(frame.value().data);
        frame = m_audioFrameQueue.pop();
    }
    return audioFrames;
}

std::vector<float *> AudioProcessingDevice::pop_audio_frames(double min, double max) {
    std::vector<float *> audioFrames;
    std::optional<AudioFrame> frame = m_audioFrameQueue.pop();
    while (frame.has_value()) {
        if (frame.value().streamTime >= min && frame.value().streamTime <= max) {
            audioFrames.push_back(frame.value().data);
        }
        frame = m_audioFrameQueue.pop();
    }
    return audioFrames;
}
