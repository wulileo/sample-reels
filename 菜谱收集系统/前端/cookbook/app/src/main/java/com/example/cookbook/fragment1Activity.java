package com.example.cookbook;

import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;


public class fragment1Activity extends Fragment {
    private TextView textView;
    private Button button;
    Socket Socket = MainActivity.Socket;//Socket
    OutputStream OutputStream = MainActivity.OutputStream;//定义数据输出流，用于发送数据
    InputStream InputStream = MainActivity.InputStream;//定义数据输入流，用于接收数据
    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view=inflater.inflate(R.layout.activity_fragment1,container,false);
        return view;
    }
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        textView=(TextView)getActivity().findViewById(R.id.TextView11);
        button=(Button)getActivity().findViewById(R.id.Button111);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                /*Toast.makeText(getActivity(),"Fragment4",Toast.LENGTH_SHORT).show();*/
                ThreadSendData t1=new ThreadSendData();
                t1.start();
                ThreadRecvData t2=new ThreadRecvData();
                t2.start();
            }
        });


    }

    public class ThreadSendData  extends Thread{
        public void run(){
            try {
                //用输出流发送数据
                String str="2";
                byte[] sb = str.getBytes("GBK");
                MainActivity.OutputStream.write(sb);
            }catch (Exception e){
                e.printStackTrace();
            }
        }
    }
    public class ThreadRecvData extends Thread{
        String textdate;
        public void run(){
            final byte[] readbuff=new byte[1024];
            int res;
            try{
                res = InputStream.read(readbuff,0,1024);
                if(res<0){
                    Socket.close();
                }
                else
                {
                    textdate = new String(readbuff,0,res,"GBK");
                    textView.setText(new String(textdate.getBytes(),"UTF-8"));
                }
            }catch (Exception e){
                e.printStackTrace();
            }
        }
    }
}

