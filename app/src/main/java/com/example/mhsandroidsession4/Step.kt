package com.example.mhsandroidsession4

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.View
import android.widget.Button

class Step(context: Context, attrs: AttributeSet) : View(context, attrs) {

    interface OnStateChangeListener {
        fun onStateChange(isActive : Boolean)
    }

    private var listener: OnStateChangeListener? = null
    private var isActive = false
    private var radius : Float = 0F

    private val circlePaint = Paint(Paint.ANTI_ALIAS_FLAG).apply {
        style = Paint.Style.FILL
        color = Color.GRAY
    }

    fun setOnStateChangeListener(l : OnStateChangeListener) {
        listener = l
    }

    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)
        canvas.drawCircle(radius, radius, radius, circlePaint)
    }

    override fun onSizeChanged(w: Int, h: Int, oldw: Int, oldh: Int) {
        super.onSizeChanged(w, h, oldw, oldh)

        val width = w.toFloat()
        radius = width/2F
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        when(event.action) {
            MotionEvent.ACTION_UP -> {

                val new_value = !isActive
                updateSate(new_value)
                listener?.onStateChange(new_value)
            }
        }

        return true
    }

    fun updateSate(active: Boolean) {
        if(active) {
            circlePaint.color = Color.MAGENTA
        }
        else {
            circlePaint.color = Color.GRAY
        }

        isActive = active
        invalidate()
    }

    fun highlightStep(highlighted : Boolean) {
        if(highlighted) {
            circlePaint.color = Color.CYAN
            invalidate()
        }
        else {
            updateSate(isActive)
        }
    }

}