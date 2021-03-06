package module.video.jnc.myffmpeg;

import android.app.Activity;
import android.content.Intent;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;

/**
 * https://blog.csdn.net/leixiaohua1020/article/details/39803457
 * rtmp 网络流
 */
public class NetStreamActivity extends Activity implements FFmpegUtils.Lis {

    private Handler handler = new Handler(new Handler.Callback() {
        @Override
        public boolean handleMessage(Message msg) {
            switch (msg.what){
                case 1:
                    Toast.makeText(NetStreamActivity.this, (String)msg.obj , Toast.LENGTH_LONG).show();
                    break;
            }
            return false;
        }
    });

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_net_stream);
        FFmpegUtils.addNativeNotify(this);

        findViewById(R.id.bt_camera_stream).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(new Intent(NetStreamActivity.this , CameraStreamActivity.class));
            }
        });

        findViewById(R.id.bt_net_work).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //注意 后面的live 是name
                FFmpegUtils.rtmpInit("rtmp://192.168.2.109/live/live"/*"sdcard/FFmpeg/aaaaa.mp4"*/ ,
                        "sdcard/FFmpeg/video/test.flv");
            }
        });
    }


    @Override
    public void nativeNotify(String str) {
        Message msg = new Message();
        msg.what = 1;
        msg.obj = str;
        handler.sendMessage(msg);
    }

    @Override
    protected void onPause() {
        super.onPause();
        FFmpegUtils.rtmpClose();
    }
}




