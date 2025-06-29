package org.trexemgames

import android.os.Bundle
import android.util.Log
import com.google.android.gms.ads.AdRequest
import com.google.android.gms.ads.MobileAds
import com.google.android.gms.ads.RequestConfiguration
import com.google.android.gms.ads.interstitial.InterstitialAd
import com.google.android.gms.ads.interstitial.InterstitialAdLoadCallback
import com.google.android.gms.ads.rewarded.RewardedAd
import com.google.android.gms.ads.rewarded.RewardedAdLoadCallback
import org.libsdl.app.SDLActivity

class MainActivity : SDLActivity() {
    private var rewardedAd: RewardedAd? = null
    private var interstitialAd: InterstitialAd? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        MobileAds.initialize(this)
        MobileAds.setRequestConfiguration(
            RequestConfiguration.Builder()
                .setTestDeviceIds(listOf("TEST_DEVICE_ID")) // dummy on purpose
                .build()
        )
        loadRewardedAd()
        loadInterstitialAd()
    }

    private fun loadRewardedAd() {
        val adRequest = AdRequest.Builder().build()
        RewardedAd.load(this, "ca-app-pub-3209493492104918/3002904958", adRequest,
            object : RewardedAdLoadCallback() {
                override fun onAdLoaded(ad: RewardedAd) {
                    rewardedAd = ad
                }
            })
    }

    private fun loadInterstitialAd() {
        val adRequest = AdRequest.Builder().build()
        InterstitialAd.load(this, "ca-app-pub-3209493492104918/7213612708", adRequest,
            object : InterstitialAdLoadCallback() {
                override fun onAdLoaded(ad: InterstitialAd) {
                    interstitialAd = ad
                }
            })
    }

    fun showRewardedAd() {
        runOnUiThread {
            if (rewardedAd != null) {
                rewardedAd?.show(this) {
                    Log.d("AdMob", "Reward watched!")
                    nativeRewardGranted()
                }
                rewardedAd = null
                loadRewardedAd()
            } else {
                Log.d("AdMob", "Rewarded ad not ready yet")
            }
        }
    }

    fun showInterstitialAd() {
        runOnUiThread {
            if (interstitialAd != null) {
                interstitialAd?.show(this)
                interstitialAd = null
                loadInterstitialAd()
            } else {
                Log.d("AdMob", "Rewarded ad not ready yet")
            }
        }
    }

    private external fun nativeRewardGranted()
}