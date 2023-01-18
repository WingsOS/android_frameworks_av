/*
 * Copyright (C) 2022 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <aidl/android/hardware/audio/effect/IEffect.h>
#include <media/audiohal/EffectHalInterface.h>
#include <system/audio_effect.h>

#include "EffectConversionHelperAidl.h"

namespace android {
namespace effect {

class EffectHalAidl : public EffectHalInterface, public EffectConversionHelperAidl {
  public:
    // Set the input buffer.
    status_t setInBuffer(const sp<EffectBufferHalInterface>& buffer) override;

    // Set the output buffer.
    status_t setOutBuffer(const sp<EffectBufferHalInterface>& buffer) override;

    // Effect process function.
    status_t process() override;

    // Process reverse stream function. This function is used to pass
    // a reference stream to the effect engine.
    status_t processReverse() override;

    // Send a command and receive a response to/from effect engine.
    status_t command(uint32_t cmdCode, uint32_t cmdSize, void* pCmdData, uint32_t* replySize,
                     void* pReplyData) override;

    // Returns the effect descriptor.
    status_t getDescriptor(effect_descriptor_t *pDescriptor) override;

    // Free resources on the remote side.
    status_t close() override;

    // Whether it's a local implementation.
    bool isLocal() const override { return false; }

    status_t dump(int fd) override;

    uint64_t effectId() const override { return mEffectId; }

    const std::shared_ptr<::aidl::android::hardware::audio::effect::IEffect> getIEffect() const {
        return mEffect;
    }

  private:
    friend class sp<EffectHalAidl>;

    const uint64_t mEffectId;
    const int32_t mSessionId;
    const int32_t mIoId;
    const std::shared_ptr<::aidl::android::hardware::audio::effect::IEffect> mEffect;
    const ::aidl::android::hardware::audio::effect::Descriptor mDesc;

    sp<EffectBufferHalInterface> mInBuffer, mOutBuffer;
    effect_config_t mConfig;

    // Can not be constructed directly by clients.
    EffectHalAidl(const std::shared_ptr<::aidl::android::hardware::audio::effect::IEffect>& effect,
                  uint64_t effectId, int32_t sessionId, int32_t ioId,
                  const ::aidl::android::hardware::audio::effect::Descriptor& desc);
    bool setEffectReverse(bool reverse);

    // The destructor automatically releases the effect.
    virtual ~EffectHalAidl();
};

} // namespace effect
} // namespace android
