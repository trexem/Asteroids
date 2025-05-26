package com.trexem.weneedmoreasteroids;

import java.util.Locale;

public class LocaleHelper {
    public static String getLanguageCode() {
        return Locale.getDefault().getLanguage();
    }

    public static String getFullLocale() {
        return Locale.getDefautl().toString();
    }
}
