package com.example.mhsandroidsession4

import android.app.Activity
import android.content.Context
import android.media.AudioManager
import android.os.Build
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.widget.SeekBar
import android.widget.Switch
import android.widget.TextView
import com.example.mhsandroidsession4.databinding.ActivityMainBinding

const val TAG = "Mytag"

class MainActivity : Activity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        val view = binding.root
        setContentView(view)

        // Retrieve default sample rate and burst size from device -----

        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {
            val myAudioMgr = getSystemService(Context.AUDIO_SERVICE) as AudioManager
            val sampleRateStr = myAudioMgr.getProperty(AudioManager.PROPERTY_OUTPUT_SAMPLE_RATE)
            val defaultSampleRate = sampleRateStr.toInt()
            val framesPerBurstStr = myAudioMgr.getProperty(AudioManager.PROPERTY_OUTPUT_FRAMES_PER_BUFFER)
            val defaultFramesPerBurst = framesPerBurstStr.toInt()
            setDefaultStreamValues(defaultSampleRate, defaultFramesPerBurst)
        }

        // Switch
        binding.holdSwitch.setOnCheckedChangeListener { switch, state ->
            setHoldValue(state)
            Log.i(TAG, "State is $state")
        }

        // Set the maximum frequency value to 1 kHz and init value to 440 Hz
        val seekbar = binding.seekBar
        seekbar.max = 1000
        //seekbar.progress = 440
        setFrequencyValue(440)

        // Seekbar
        seekbar.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {

            override fun onProgressChanged(seekBar: SeekBar, i: Int, b: Boolean) {
                val frequency = i + 20
                setFrequencyValue(frequency)
                Log.i(TAG, "Slider value is $i")
            }

            override fun onStartTrackingTouch(p0: SeekBar?) { }

            override fun onStopTrackingTouch(p0: SeekBar?) { }
        })

        // Pad
        binding.pad.setOnPressedListener(object : Pad.OnPressedListener() {

            override fun onPadPressed(pressed : Boolean) {
                setHoldValue(pressed)
            }

        })

    }

    override fun onResume() {
        super.onResume()
        startAudio()
    }

    override fun onPause() {
        stopAudio()
        super.onPause()
    }

//    override fun onTouchEvent(event: MotionEvent): Boolean {
//        when(event.action) {
//            MotionEvent.ACTION_DOWN -> {
//                setHoldValue(true)
//            }
//            MotionEvent.ACTION_UP -> {
//                setHoldValue(false)
//            }
//        }
//        return super.onTouchEvent(event)
//    }

    fun setHoldValue(hold : Boolean) {
        binding.holdSwitch.isChecked = hold // update switch
        binding.pad.updateView(hold) // update pad
        hold(hold) // notify engine
    }

    fun setFrequencyValue(freq : Int) {
        binding.seekBar.progress = freq - 20 // update seekbar
        binding.frequencyValue.text = "$freq Hz"// update text value
        setFrequency(freq) // notify engine
    }

    // Native methods
    external fun startAudio()
    external fun stopAudio()
    external fun setDefaultStreamValues(sampleRate : Int , framesPerBurst : Int)

    external fun hold(hold: Boolean)
    external fun setFrequency(freq : Int)

    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }
}