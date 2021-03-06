//
// Created by dugang on 2018/6/29.
//

#ifndef MYFFMPEG_MYSOUNDTOUCH_H
#define MYFFMPEG_MYSOUNDTOUCH_H

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <my_data.h>
#include "MyThread.h"
#include "FFmpegReadFrame.h"
#include "DecodeAudioThread.h"
#include "sonic.h"
#include <queue>
#include <SonicRead.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
}

using namespace std;

class mySoundTouch : public MyThread{

public :
    void run();
    //ffmepg
    AVFrame *aframe;
    AVFormatContext *afc;
    int audioindex ;
    AVCodec *audioCode;
    AVCodecContext *ac;
    SwrContext *swc;
    int64_t duration;
    //audio_sl
    SLObjectItf engineOpenSL ;
    SLPlayItf iplayer ;
    SLEngineItf eng ;
    SLObjectItf mix ;
    SLObjectItf player ;
    SLAndroidSimpleBufferQueueItf pcmQue ;


    queue<AVPacket *> audioPktQue;
    queue<MyData> audioFrameQue;
//    SAMPLETYPE *buf_play_gpu = NULL;
    void init(const char *soucetouch);
    short *playAudioBuffer;
//    SoundTouchDeal *soundTouchDeal;
    ~mySoundTouch();
    SonicRead *sonicRead;
    short *getBuf;
    FILE *after ;
    FILE *fReadPcm ;
    int bufferSize;
private :
    int initFFmpeg(const char *inputPath);
    int initOpenSl();
    SLEngineItf createOpenSL();
    void audioPlayDelay();
    ReadFrame *readFrameThread;
    DecodeAudioThread *decodeAudioThread;
    pthread_mutex_t mutex_pthread ;
    int sampleRate ;


};

#endif //MYFFMPEG_MYSOUNDTOUCH_H
