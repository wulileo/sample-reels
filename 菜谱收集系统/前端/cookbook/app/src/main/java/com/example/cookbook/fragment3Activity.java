package com.example.cookbook;

import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.MainThread;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;


public class fragment3Activity extends Fragment  {
    EditText edit1,edit2;
    private Button button;
    private TextView textView1;
    Socket Socket = MainActivity.Socket;//Socket
    OutputStream OutputStream = MainActivity.OutputStream;//定义数据输出流，用于发送数据
    InputStream InputStream = MainActivity.InputStream;//定义数据输入流，用于接收数据


    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view=inflater.inflate(R.layout.activity_fragment3,container,false);
        return view;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        edit1=(EditText)getActivity().findViewById(R.id.editText133);
        edit2=(EditText)getActivity().findViewById(R.id.editText233);
        button=(Button)getActivity().findViewById(R.id.Button6);
        textView1=(TextView)getActivity().findViewById(R.id.TextView33);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
               // Toast.makeText(getActivity(),"Fragment3",Toast.LENGTH_SHORT).show();
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
                String name1 =edit1.getText().toString().trim();
                String name2 =edit2.getText().toString().trim();
                //String result;
                //result=String.format("%0"+(16-name1.length())+"c",Integer.parseInt(name2) + 1);
                String type="4";
                byte[]name1_s = name1.getBytes("GBK");
                byte[]name2_s = name2.getBytes("GBK");
                byte[]type_s = type.getBytes("GBK");
                byte[]final_s = new byte[1024];
                System.arraycopy(type_s,0,final_s,0,type_s.length);
                System.arraycopy(name1_s,0,final_s,1,name1_s.length);
                System.arraycopy(name2_s,0,final_s,17,name2_s.length);
                MainActivity.OutputStream.write(final_s);
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
                    textView1.setText(new String(textdate.getBytes(),"UTF-8"));
                }
            }catch (Exception e){
                e.printStackTrace();
            }
        }
    }


}

