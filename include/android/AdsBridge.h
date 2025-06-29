#pragma once

#if defined(__ANDROID__)
#include "jni.h"

void showRewardedAd();
void showInterstitialAd();

extern "C" JNIEXPORT void JNICALL
Java_org_trexemgames_MainActivity_nativeRewardGranted(JNIEnv* env, jobject obj);

#else
#include "Log.h"

inline void showRewardedAd() {
	DEBUG_LOG("showRewardedAd() called");
}

inline void showInterstitialAd() {}

#endif
