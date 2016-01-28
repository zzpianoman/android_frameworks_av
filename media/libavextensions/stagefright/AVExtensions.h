/*
 * Copyright (c) 2013 - 2015, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *      contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _AV_EXTENSIONS_H_
#define _AV_EXTENSIONS_H_

#include <media/stagefright/DataSource.h>
#include <common/AVExtensionsCommon.h>
#include <system/audio.h>
#include <camera/ICamera.h>
#include <media/mediarecorder.h>
#include <media/IOMX.h>

namespace android {

struct ACodec;
struct ALooper;
class MediaExtractor;
struct MediaCodec;
class AudioParameter;
struct NuCachedSource2;
struct MediaHTTP;
struct IMediaHTTPConnection;
class CameraParameters;
class MediaBuffer;
struct AudioSource;
class CameraSource;
class CameraSourceTimeLapse;
class ICamera;
class ICameraRecordingProxy;
class String16;
class IGraphicBufferProducer;
struct Size;

/*
 * Factory to create objects of base-classes in libstagefright
 */
struct AVFactory {
    virtual sp<ACodec> createACodec();
    virtual MediaExtractor* createExtendedExtractor(
            const sp<DataSource> &source, const char *mime,
            const sp<AMessage> &meta);
    virtual sp<MediaExtractor> updateExtractor(
            sp<MediaExtractor> ext, const sp<DataSource> &source,
            const char *mime, const sp<AMessage> &meta);
    virtual sp<NuCachedSource2> createCachedSource(
            const sp<DataSource> &source,
            const char *cacheConfig = NULL,
            bool disconnectAtHighwatermark = false);
    virtual MediaHTTP* createMediaHTTP(
            const sp<IMediaHTTPConnection> &conn);

    virtual AudioSource* createAudioSource(
            audio_source_t inputSource,
            const String16 &opPackageName,
            uint32_t sampleRate,
            uint32_t channels,
            uint32_t outSampleRate = 0);

    virtual CameraSource *CreateCameraSourceFromCamera(
            const sp<ICamera> &camera,
            const sp<ICameraRecordingProxy> &proxy,
            int32_t cameraId,
            const String16& clientName,
            uid_t clientUid,
            Size videoSize,
            int32_t frameRate,
            const sp<IGraphicBufferProducer>& surface,
            bool storeMetaDataInVideoBuffers = true);

    virtual CameraSourceTimeLapse *CreateCameraSourceTimeLapseFromCamera(
            const sp<ICamera> &camera,
            const sp<ICameraRecordingProxy> &proxy,
            int32_t cameraId,
            const String16& clientName,
            uid_t clientUid,
            Size videoSize,
            int32_t videoFrameRate,
            const sp<IGraphicBufferProducer>& surface,
            int64_t timeBetweenFrameCaptureUs,
            bool storeMetaDataInVideoBuffers = true);
    // ----- NO TRESSPASSING BEYOND THIS LINE ------
    DECLARE_LOADABLE_SINGLETON(AVFactory);
};

/*
 * Common delegate to the classes in libstagefright
 */
struct AVUtils {

    virtual status_t convertMetaDataToMessage(
            const sp<MetaData> &meta, sp<AMessage> *format);
    virtual DataSource::SnifferFunc getExtendedSniffer();
    virtual status_t mapMimeToAudioFormat( audio_format_t& format, const char* mime);
    virtual status_t sendMetaDataToHal(const sp<MetaData>& meta, AudioParameter *param);

    virtual sp<MediaCodec> createCustomComponentByName(const sp<ALooper> &looper,
                const char* mime, bool encoder, const sp<AMessage> &format);

    virtual void extractCustomCameraKeys(
            const CameraParameters& /*params*/, sp<MetaData> &/*meta*/) {}
    virtual void printFileName(int /*fd*/) {}
    virtual void addDecodingTimesFromBatch(MediaBuffer * /*buf*/,
            List<int64_t> &/*decodeTimeQueue*/) {}

    virtual bool canDeferRelease(const sp<MetaData> &/*meta*/) { return false; }
    virtual void setDeferRelease(sp<MetaData> &/*meta*/) {}
    virtual bool isAudioMuxFormatSupported(const char *mime);
    virtual void cacheCaptureBuffers(sp<ICamera> camera, video_encoder encoder);
    virtual const char *getCustomCodecsLocation();

    virtual void setIntraPeriod(
                int nPFrames, int nBFrames, const sp<IOMX> OMXhandle,
                IOMX::node_id nodeID);

    // ----- NO TRESSPASSING BEYOND THIS LINE ------
    DECLARE_LOADABLE_SINGLETON(AVUtils);
};

}

#endif // _AV_EXTENSIONS__H_
