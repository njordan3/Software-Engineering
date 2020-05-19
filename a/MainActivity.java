package com.example.student.lab10;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.os.Handler;
import android.view.MotionEvent;
import android.support.constraint.ConstraintLayout;
import java.lang.Runnable;
import android.widget.Toast;

public class Lab10 extends AppCompatActivity implements View.OnTouchListener {
    Handler handler = new Handler();
    TextView default_text, name_text;
    Button btn;
    float initial_x, initial_y, final_x, final_y;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_lab10);
        ConstraintLayout c = (ConstraintLayout) findViewById(R.id.layout);
        default_text = (TextView) findViewById(R.id.default_textView);
        name_text = (TextView) findViewById(R.id.name);
        btn = (Button) findViewById(R.id.button);
        c.setOnTouchListener(this);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                default_text.setVisibility(View.INVISIBLE);
                name_text.setVisibility(View.VISIBLE);
                handler.postDelayed(new Runnable() {
                    public void run() {
                        default_text.setVisibility(View.VISIBLE);
                        name_text.setVisibility(View.INVISIBLE);
                    }
                }, 1000);
                Toast.makeText(getApplicationContext(),"Great job! You pushed the button!",Toast.LENGTH_SHORT).show();
            }
        });
    }
    @Override
    public boolean onTouch(View v, MotionEvent event) {
        switch (event.getActionMasked()) {
            case MotionEvent.ACTION_DOWN:
                initial_x = event.getX();
                initial_y = event.getY();
                break;
            case MotionEvent.ACTION_MOVE:
                final_x = event.getX();
                final_y = event.getY();
                if (initial_y - final_y < -750 && initial_x - final_x > -100 && initial_x - final_x < 100) {
                    finish();
                    System.exit(0);
                }
                break;
            case MotionEvent.ACTION_UP:
                break;
        }
        return true;
    }
}