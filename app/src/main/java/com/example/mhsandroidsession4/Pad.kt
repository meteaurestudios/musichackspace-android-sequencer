package com.example.mhsandroidsession4

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.View

class Pad(context: Context, attrs: AttributeSet) : View(context, attrs) {

    abstract class OnPressedListener {
        abstract fun onPadPressed(pressed : Boolean)
    }

    private var listener: OnPressedListener? = null

    private var radius : Float = 0F

    private val circlePaint = Paint(Paint.ANTI_ALIAS_FLAG).apply {
        style = Paint.Style.FILL
        color = Color.GRAY
    }

    fun setOnPressedListener(l : OnPressedListener) {
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
            MotionEvent.ACTION_DOWN -> {
                updateView(true)
                listener?.onPadPressed(true)
            }
            MotionEvent.ACTION_UP -> {
                updateView(false)
                listener?.onPadPressed(false)
            }
        }

        return true
    }

    fun updateView(pressed : Boolean) {
        if(pressed) {
            circlePaint.color = Color.MAGENTA
        }
        else {
            circlePaint.color = Color.GRAY
        }
        invalidate()
    }

}