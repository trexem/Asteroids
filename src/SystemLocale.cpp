#include "SystemLocale.h"

#include <SDL3/SDL_system.h>
#include <SDL3/SDL_log.h>

#if defined(_WIN32)
    #include <windows.h>
#elif defined(__ANDROID__)
    #include <jni.h>
#else
    #include <clocale>
    #include <cstdlib>
    #include <cstring>
#endif

std::string SystemLocale::get() {
#if defined(_WIN32)
    wchar_t localeName[LOCALE_NAME_MAX_LENGTH] = {0};
    if (GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH)) {
        char buffer[LOCALE_NAME_MAX_LENGTH];
        WideCharToMultiByte(CP_UTF8, 0, localeName, -1, buffer, LOCALE_NAME_MAX_LENGTH, NULL, NULL);
        return std::string(buffer).substr(0, 2); // "en-US" → "en"
    }
    return "en";
#elif defined(__ANDROID__)
    JNIEnv* env = (JNIEnv *)SDL_GetAndroidJNIEnv();
    jobject activity = (jobject) SDL_GetAndroidActivity();

    if (!env || !activity) return "en";

    jclass helperClass = env->FindClass("com/trexem/weneedmoreasteroids/LocaleHelper");
    if (!helperClass) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "LocaleHelper class not found");
        return "en";
    }

    jmethodID methodID = env->GetStaticMethodID(helperClass, "getLanguageCode", "()Ljava/lang/String;");
    if (!methodID) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "getLanguageCode() method not found");
        return "en";
    }

    jstring langString = (jstring)env->CallStaticObjectMethod(helperClass, methodID);
    const char* nativeStr = env->GetStringUTFChars(langString, 0);
    std::string result(nativeStr);
    env->ReleaseStringUTFChars(langString, nativeStr);
    return result;
#else
    const char* lang = std::getenv("LANG");
    if (lang && strlen(lang) >= 2) {
        if (std::string(lang).substr(0, 2) == "C.") {
            return "en"; 
        }
        return std::string(lang).substr(0, 2); // "en_US.UTF-8" → "en"
    }
    return "en";
#endif
}
