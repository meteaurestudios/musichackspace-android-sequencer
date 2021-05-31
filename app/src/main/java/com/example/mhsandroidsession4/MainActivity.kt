package com.example.mhsandroidsession4

import android.app.Activity
import android.content.Context
import android.media.AudioManager
import android.os.Build
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.AdapterView
import android.widget.ArrayAdapter
import android.widget.SeekBar
import android.widget.Spinner
import com.example.mhsandroidsession4.databinding.ActivityMainBinding

const val TAG = "Mytag"

class MainActivity : Activity() {

    private lateinit var binding: ActivityMainBinding
    private lateinit var stepsArray : Array<Step>
    private lateinit var stepsPitchArray : Array<Spinner>
    private lateinit var engineCommunicationThread : CommunicationThread

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        // Initialize view binding -----

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Retrieve default sample rate and burst size from device -----

        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {
            val myAudioMgr = getSystemService(Context.AUDIO_SERVICE) as AudioManager
            val sampleRateStr = myAudioMgr.getProperty(AudioManager.PROPERTY_OUTPUT_SAMPLE_RATE)
            val defaultSampleRate = sampleRateStr.toInt()
            val framesPerBurstStr = myAudioMgr.getProperty(AudioManager.PROPERTY_OUTPUT_FRAMES_PER_BUFFER)
            val defaultFramesPerBurst = framesPerBurstStr.toInt()
            setDefaultStreamValues(defaultSampleRate, defaultFramesPerBurst)
        }

        // Play switch -----

        binding.playSwitch.setOnCheckedChangeListener { switch, state ->
            setPlayValue(state)
            Log.i(TAG, "State is $state")
        }

        // Tempo slider -----

        binding.tempoSeekBar.max = 270 // set the range to be in [30; 300], i.e. 271 possible values

        binding.tempoSeekBar.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {

            override fun onProgressChanged(seekBar: SeekBar, i: Int, b: Boolean) {
                val tempo = i + 30
                setTempoValue(tempo)
                Log.i(TAG, "Slider value is $i")
            }

            override fun onStartTrackingTouch(p0: SeekBar?) { }

            override fun onStopTrackingTouch(p0: SeekBar?) { }
        })

        // Steps on/off -----

        stepsArray = arrayOf(binding.step1,
                             binding.step2,
                             binding.step3,
                             binding.step4,
                             binding.step5,
                             binding.step6,
                             binding.step7,
                             binding.step8)

        for(i in stepsArray.indices) {
            stepsArray[i].setOnStateChangeListener(object: Step.OnStateChangeListener {

                override fun onStateChange(isActive: Boolean) {
                    setStepIsActiveValue(i, isActive)
                    Log.i(TAG, "step $i is $isActive")
                }

            })
        }

        // Steps pitch -----

        stepsPitchArray = arrayOf(binding.step1Pitch,
                                  binding.step2Pitch,
                                  binding.step3Pitch,
                                  binding.step4Pitch,
                                  binding.step5Pitch,
                                  binding.step6Pitch,
                                  binding.step7Pitch,
                                  binding.step8Pitch)

        for(i in stepsPitchArray.indices) {
            // Create an ArrayAdapter using the string array and a default spinner layout
            ArrayAdapter.createFromResource(
                this,
                R.array.pitches_array,
                android.R.layout.simple_spinner_item
            ).also { adapter ->
                // Specify the layout to use when the list of choices appears
                adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
                // Apply the adapter to the spinner
                stepsPitchArray[i].adapter = adapter
            }

            stepsPitchArray[i].setOnItemSelectedListener(object : AdapterView.OnItemSelectedListener {

                override fun onItemSelected(parent: AdapterView<*>,  view: View?,  pos: Int,  id: Long ) {
                    setStepPitchValue(i, pos)
                    Log.i(TAG, "step $i value $pos")
                }

                override fun onNothingSelected(parent: AdapterView<*>) {
                    // Do nothing
                }

            })
        }

        // Initialize play state and tempo value
        setTempoValue(120)
        setPlayValue(true)

        // Engine communication thread init
        engineCommunicationThread = CommunicationThread(this)
        engineCommunicationThread.start()
    }

    override fun onResume() {
        super.onResume()
        startAudio()
    }

    override fun onPause() {
        stopAudio()
        super.onPause()
    }

    // Parameter centered methods -----

    fun setPlayValue(isPlaying : Boolean) {
        binding.playSwitch.isChecked = isPlaying // update switch
        setEngineIsPlaying(isPlaying) // notify engine
    }

    fun setTempoValue(tempo : Int) {
        binding.tempoSeekBar.progress = tempo - 30 // update seekbar
        binding.tempoValue.text = "$tempo bpm"// update text value
        setEngineTempo(tempo) // notify engine
    }

    /**
     * Sets the state on a given step
     * @param step the index of the targeted step, from 0 to 7
     * @param isActive if true the step is on, off otherwise
     */
    fun setStepIsActiveValue(step : Int, isActive : Boolean) {
        setEngineStepIsActive(step, isActive)
    }

    /**
     * Sets the pitch value on a given step
     * @param step the index of the targeted step, from 0 to 7
     * @param pitch the index of the pitch from 0 to 11 (pitch array values)
     */
    fun setStepPitchValue(step : Int, pitch : Int) {
        setEngineStepPitch(step, pitch)
    }

    fun updateCurrentStep(step : Int) {
        // Remove previous highlight on all steps
        for(i in stepsArray.indices) {
            stepsArray[i].highlight(false)
        }

        // Highlight new step
        stepsArray[step].highlight(true)
    }

    // Native methods -----

    external fun startAudio()
    external fun stopAudio()
    external fun setDefaultStreamValues(sampleRate : Int , framesPerBurst : Int)

    external fun setEngineIsPlaying(isPlaying: Boolean)
    external fun setEngineTempo(tempo : Int)
    external fun setEngineStepIsActive(step : Int, isActive: Boolean)
    external fun setEngineStepPitch(step : Int, pitch: Int)

    external fun getEngineCurrentStep() : Int

    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }

    private class CommunicationThread(a: MainActivity) : Thread() {

        private var activity : MainActivity = a
        private var currentStep = 0

        override fun run() {

            while(!isInterrupted) {
                try {
                    val step = activity.getEngineCurrentStep()

                    if(step != currentStep) {

                        activity.runOnUiThread {
                            activity.updateCurrentStep(step)
                        }

                        currentStep = step
                    }
                    sleep(10)
                }
                catch (ex: InterruptedException) {
                    interrupt()
                }
            }
        }
    }

}
