//
// Created by Josh Shiells on 31/01/2024.
//

#include "AudioProcessingDevice.h"

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
    m_audioDeviceSettings.deviceId = find_device_idx_by_id(m_deviceIds, deviceId);
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
