//
// Created by Administrator on 2018/4/23/023.
//
#include "my_log.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <stdio.h>
#include "my_open_sl_test.h"
#include <thread>

using namespace std;
/**
 * soundtouch
 */
int SIZE = 1024;

/**
 * https://developer.android.com/ndk/guides/audio/android-extensions.html?hl=zh-cn#dynamic-interfaces 官方链接
 * https://blog.csdn.net/ywl5320/article/details/78503768
 * 利用opensles 播放pcm原始数据文件
 * 播放和暂停应该是要控制pcm的原始数据
 * seek等也是。跳转到10s出，就去解码10s出的音频。
 */
SLObjectItf engineSL = NULL;
const char *pcm_path;
SLPlayItf iplayer = NULL;
SLEngineItf eng = NULL;
SLObjectItf mix = NULL;
SLObjectItf player = NULL;
FILE *fp = NULL;
char *buf = NULL;


SLAndroidSimpleBufferQueueItf pcmQue = NULL;

//test
FILE *fileTest;

SLEngineItf createSL() {
//    LOGE(" SIZE OF SHORT %d " , sizeof(short));
    SLresult re = 0;
    SLEngineItf en = NULL;
    re = slCreateEngine(&engineSL, 0, 0, 0, 0, 0);
    if (re != SL_RESULT_SUCCESS) {
        LOGE("slCreateEngine FAILD ");
        return NULL;
    }
    re = (*engineSL)->Realize(engineSL, SL_BOOLEAN_FALSE);
    if (re != SL_RESULT_SUCCESS) {
        LOGE("Realize FAILD ");
        return NULL;
    }
    re = (*engineSL)->GetInterface(engineSL, SL_IID_ENGINE, &en);
    if (re != SL_RESULT_SUCCESS) {
        LOGE("GetInterface FAILD ");
        return NULL;
    }
    return en;
}

bool haveData = true;
int num2 = 0;

int readPcmData() {
    int len = 0;

//    while (!feof(fp)) {
//        if (haveData) {
//            haveData = false;
//            //立体声。
//            len = fread(buf, 1, SIZE * 2, fp);
//
//            if (len > 0) {
//                //第二个参数是放入多少个sample ， 16位一个。双声道，16位 , 加起来就/4
//
//                mySoundTouch->putSamples((SAMPLETYPE *) buf, len / 2);
//                LOGE(" len %d ", len);
//            } else {
//                num2 = 0;
//                mySoundTouch->clear();
//            }
//        }
//        num2 = mySoundTouch->receiveSamples(reciveBuf, len / 2);
//        LOGE(" num2 %d ", num2);
//        if (num2 == 0) {
//            haveData = true;
//            continue;
//        }
//        return num2;
//    }
    return len;
}

//声音回调
void pcmCall(SLAndroidSimpleBufferQueueItf bf, void *context) {

//    if (!buf) {
//        //一帧是1024个sample ，
//        buf = (char *) malloc(SIZE * 2 );
//    }
//    if (!reciveBuf) {
//        reciveBuf = (SAMPLETYPE *) malloc(SIZE * 2);
//    }
//    if (!fp) {
//        fp = fopen(pcm_path, "rb");
//    }
//    if (!fp) {
//        LOGE("file faild ! %s ?", pcm_path);
//        return;
//    }
//    int size = readPcmData();
//
//    if(size > 0){
//        //往缓冲区中丢数据，有数据他就播放。没有数据就进入回调函数 ， 第三个参数应该是字节数
//        LOGE(" ADD BUFFER ! %d " , size);
//        (*bf)->Enqueue(bf, reciveBuf, size * 2);
//
//    }else{
//        LOGE(" finish... !");
//    }


}

//暂停或者播放
int pause(bool flag) {
    if (iplayer != NULL) {

        SLresult re = (*iplayer)->SetPlayState(iplayer, flag == true ? SL_PLAYSTATE_PLAYING
                                                                     : SL_PLAYSTATE_PAUSED);

        if (re != SL_RESULT_SUCCESS) {
            LOGE("SetPlayState pause FAILD ");
            return -1;
        }
        LOGE("SetPlayState pause success ");
    }

    return 0;
}

void ThreadSleep2(int mis) {
    chrono::milliseconds du(mis);
    this_thread::sleep_for(du);
}


void playAudioDelay() {

}

/**
 * https://blog.csdn.net/ywl5320/article/details/79735943
 * @return
 */
int init_sound_touch() {
    fileTest = fopen("sdcard/FFmpeg/sttest.pcm", "wb+");
//    mySoundTouch = new SoundTouch();
//    //采样率
//    mySoundTouch->setSampleRate(48000);
//    //声道数
//    mySoundTouch->setChannels(1);
//    //速度
//    mySoundTouch->setTempo(1.0);
//    mySoundTouch->setPitch(1);
    return RESULT_SUCCESS;
}

int play_audio(const char *path) {
    pcm_path = (char *) malloc(1024);
//    memcpy(pcm_path , path , 1024);
    pcm_path = path;
    //创建soundtouch
    init_sound_touch();
    //创建引擎
    eng = createSL();
    if (!eng) {
        LOGE("createSL FAILD ");
    }

    //2.创建混音器
    mix = NULL;
    SLresult re = 0;
    re = (*eng)->CreateOutputMix(eng, &mix, 0, 0, 0);
    if (re != SL_RESULT_SUCCESS) {
        LOGE("CreateOutputMix FAILD ");
        return -1;
    }
    re = (*mix)->Realize(mix, SL_BOOLEAN_FALSE);
    if (re != SL_RESULT_SUCCESS) {
        LOGE("Realize FAILD ");
        return -1;
    }
    SLDataLocator_OutputMix outmix = {SL_DATALOCATOR_OUTPUTMIX, mix};
    SLDataSink audioSink = {&outmix, 0};

    //配置音频信息
    SLDataLocator_AndroidSimpleBufferQueue que = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 10};



    //音频格式
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,
            1,//    声道数

            SL_SAMPLINGRATE_48,

            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT ,
            SL_BYTEORDER_LITTLEENDIAN //字节序，小端
    };
    SLDataSource ds = {&que, &pcm};

    //创建播放器
    const SLInterfaceID ids[] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[] = {SL_BOOLEAN_TRUE};
    re = (*eng)->CreateAudioPlayer(eng, &player, &ds, &audioSink,
                                   sizeof(ids) / sizeof(SLInterfaceID), ids, req);
    if (re != SL_RESULT_SUCCESS) {
        LOGE("CreateAudioPlayer FAILD ");
        return -1;
    }
    (*player)->Realize(player, SL_BOOLEAN_FALSE);
    re = (*player)->GetInterface(player, SL_IID_PLAY, &iplayer);
    if (re != SL_RESULT_SUCCESS) {
        LOGE("GetInterface SL_IID_PLAY FAILD ");
        return -1;
    }
    re = (*player)->GetInterface(player, SL_IID_BUFFERQUEUE, &pcmQue);
    if (re != SL_RESULT_SUCCESS) {
        LOGE("GetInterface SL_IID_BUFFERQUEUE FAILD ");
        return -1;
    }

    (*pcmQue)->RegisterCallback(pcmQue, pcmCall, 0);
    LOGE("THREAD START %s ", pcm_path);
    (*iplayer)->SetPlayState(iplayer, SL_PLAYSTATE_PLAYING);
    (*pcmQue)->Enqueue(pcmQue, "", 1);
    LOGE(" play_audio SUCCESS ");
    return 1;
}


// 对象应按照与创建时相反的顺序销毁 ,  例如，请按照此顺序销毁：音频播放器和录制器、输出混合，最后是引擎。
int openslDestroy() {
    if (iplayer && (*iplayer)) {
        (*iplayer)->SetPlayState(iplayer, SL_PLAYSTATE_STOPPED);
    }
    if (pcmQue != NULL) {
        (*pcmQue)->Clear(pcmQue);
    }
    if (player != NULL) {
        (*player)->Destroy(player);
        player = NULL;
        iplayer = NULL;
        pcmQue = NULL;
    }
    if (mix != NULL) {
        (*mix)->Destroy(mix);
        mix = NULL;
    }
    if (engineSL != NULL) {
        (*engineSL)->Destroy(engineSL);
        engineSL = NULL;
        eng = NULL;
    }
    if (fp != NULL) {
        fclose(fp);
        fp = NULL;
    }
//    if (mySoundTouch != NULL) {
//        delete mySoundTouch;
//        mySoundTouch = NULL;
//    }
    if (fileTest != NULL) {
        fclose(fileTest);
    }
    if(pcm_path != NULL){
//        free(pcm_path);
//        delete pcm_path;
    }
    return 1;
}


