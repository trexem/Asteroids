#if defined(__ANDROID__)

#include "AdsBridge.h"
#include "Log.h"

#include <SDL3/SDL_system.h>
#include <SDL3/SDL.h>
#include <android/log.h>

#define LOG_TAG "AdsBridge"

void showRewardedAd() {
    JNIEnv* env = (JNIEnv *)SDL_GetAndroidJNIEnv();
    jobject activity = (jobject)SDL_GetAndroidActivity();
    jclass cls = env->GetObjectClass(activity);
    jmethodID mid = env->GetMethodID(cls, "showRewardedAd", "()V");

    if (mid) {
        env->CallVoidMethod(activity, mid);
        DEBUG_LOG("Called showRewardedAd() from native");
    } else {
        DEBUG_LOG("Failed to find method showRewardedAd()");
    }
}

void showInterstitialAd() {
    JNIEnv* env = (JNIEnv *)SDL_GetAndroidJNIEnv();
    jobject activity = (jobject)SDL_GetAndroidActivity();
    jclass cls = env->GetObjectClass(activity);
    jmethodID mid = env->GetMethodID(cls, "showInterstitialAd", "()V");

    if (mid) {
        env->CallVoidMethod(activity, mid);
        DEBUG_LOG("Called showRewardedAd() from native");
    } else {
        DEBUG_LOG("Failed to find method showRewardedAd()");
    }
}

extern "C" JNIEXPORT void JNICALL
Java_org_trexemgames_MainActivity_nativeRewardGranted(JNIEnv* env, jobject obj) {
    DEBUG_LOG("Reward granted from Kotlin");
}

#endif
