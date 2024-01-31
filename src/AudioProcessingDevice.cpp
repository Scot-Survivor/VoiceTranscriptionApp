//
// Created by Josh Shiells on 31/01/2024.
//

#include "AudioProcessingDevice.h"

unsigned int AudioProcessingDevice::find_device_idx_by_id(const std::vector<unsigned int> &deviceIds, unsigned int deviceId) {
    for (unsigned int i = 0; i < deviceIds.size(); i++) {
        if (deviceIds[i] == deviceId) {
            return i;
        }
    }
    return -1;
}

AudioProcessingDevice::AudioProcessingDevice(const std::vector<unsigned int> &deviceIds, unsigned int deviceId) {
    m_audioDeviceSettings = {};
    m_audioDeviceSettings.deviceId = find_device_idx_by_id(deviceIds, deviceId);
}
