package com.example.cookbook;

import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.text.TextUtils;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

public class MainActivity extends AppCompatActivity {
    private Button btn_login,btn_register;//登录按钮
    private String userName,psw,spPsw;//获取的用户名，密码
    private EditText et_user_name,et_psw;//编辑框

    public static Socket Socket = null;//Socket
    public static OutputStream OutputStream = null;//定义数据输出流，用于发送数据
    public static InputStream InputStream = null;//定义数据输入流，用于接收数据

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        init();

    }

    public class Connect_Thread extends Thread{
        public void run(){
            try {
                //判断socket的状态，防止重复执行
                if (Socket == null) {
                    //新建一个socket
                    Socket = new Socket("118.89.179.187", 4000);
                    InputStream = Socket.getInputStream();
                    OutputStream = Socket.getOutputStream();
                }
            } catch (Exception e) {
                //如果有错误则在这里返回
                e.printStackTrace();
            }
        }
    }
    public class ThreadSendData  extends Thread{
        public void run(){
            try {
                //用输出流发送数据
                String  str = "2";
                byte[] sb = str.getBytes("GBK");
                OutputStream.write(sb);
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
                   /* textdate = new String(readbuff,0,res,"GBK");
                    et_user_name.setText(new String(textdate.getBytes(),"UTF-8"));*/
                }
            }catch (Exception e){
                e.printStackTrace();
            }
        }
    }



    private void init() {
        Connect_Thread Connect_thread = new Connect_Thread();
        Connect_thread.start();

        btn_register=findViewById(R.id.button2);
        btn_login=findViewById(R.id.button1);
        et_user_name=findViewById(R.id.text_userid);
        et_psw=findViewById(R.id.text_userpwd);
        //立即注册控件的点击事件
        btn_register.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //为了跳转到注册界面，并实现注册功能
                Intent intent=new Intent(MainActivity.this,RegisterActivity.class);
                startActivityForResult(intent, 1);
            }
        });
        //登录按钮的点击事件
        btn_login.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //ThreadRecvData t2 = new ThreadRecvData();
                //开始登录，获取用户名和密码 getText().toString().trim();
                userName=et_user_name.getText().toString().trim();
                psw=et_psw.getText().toString().trim();
                // 定义方法 readPsw为了读取用户名，得到密码
                spPsw=readPsw(userName);
                // TextUtils.isEmpty
                if(TextUtils.isEmpty(userName)){
                    Toast.makeText(MainActivity.this, "请输入用户名", Toast.LENGTH_SHORT).show();
                    return;
                }else if(TextUtils.isEmpty(psw)){
                    Toast.makeText(MainActivity.this, "请输入密码", Toast.LENGTH_SHORT).show();
                    return;
                    // psw.equals(); 判断，输入的密码，是否与保存在SharedPreferences中一致
                }else if(psw.equals(spPsw)){
                    //一致登录成功
                    Toast.makeText(MainActivity.this, "登录成功", Toast.LENGTH_SHORT).show();
                    //保存登录状态，在界面保存登录的用户名 定义个方法 saveLoginStatus boolean 状态 , userName 用户名;
                    saveLoginStatus(true, userName);
                    //登录成功后关闭此页面进入主页
                    ThreadSendData t1=new ThreadSendData();
                    //t1.start();
                    Intent data=new Intent();
                    //datad.putExtra( ); name , value ;
                    data.putExtra("isLogin",true);
                    //RESULT_OK为Activity系统常量，状态码为-1
                    setResult(RESULT_OK,data);
                    //销毁登录界面
                    MainActivity.this.finish();
                    //跳转到主界面，登录成功的状态传递到 HomepageActivity 中
                    startActivity(new Intent(MainActivity.this, HomepageActivity.class));
                    return;
                }else if((spPsw!=null&&!TextUtils.isEmpty(spPsw)&&!psw.equals(spPsw))){
                    Toast.makeText(MainActivity.this, "输入的用户名和密码不一致", Toast.LENGTH_SHORT).show();
                    return;
                }else{
                    Toast.makeText(MainActivity.this, "此用户名不存在", Toast.LENGTH_SHORT).show();
                }
            }
        });
    }
    /**
     *从SharedPreferences中根据用户名读取密码
     */
    private String readPsw(String userName){
        //getSharedPreferences("loginInfo",MODE_PRIVATE);
        //"loginInfo",mode_private; MODE_PRIVATE表示可以继续写入
        SharedPreferences sp=getSharedPreferences("loginInfo", MODE_PRIVATE);
        //sp.getString() userName, "";
        return sp.getString(userName , "");
    }
    /**
     *保存登录状态和登录用户名到SharedPreferences中
     */
    private void saveLoginStatus(boolean status,String userName){
        //saveLoginStatus(true, userName);
        //loginInfo表示文件名  SharedPreferences sp=getSharedPreferences("loginInfo", MODE_PRIVATE);
        SharedPreferences sp=getSharedPreferences("loginInfo", MODE_PRIVATE);
        //获取编辑器
        SharedPreferences.Editor editor=sp.edit();
        //存入boolean类型的登录状态
        editor.putBoolean("isLogin", status);
        //存入登录状态时的用户名
        editor.putString("loginUserName", userName);
        //提交修改
        editor.commit();
    }
    /**
     * 注册成功的数据返回至此
     * @param requestCode 请求码
     * @param resultCode 结果码
     * @param data 数据
     */
    @Override
    //显示数据， onActivityResult
    //startActivityForResult(intent, 1); 从注册界面中获取数据
    //int requestCode , int resultCode , Intent data
    // LoginActivity -> startActivityForResult -> onActivityResult();
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        //super.onActivityResult(requestCode, resultCode, data);
        super.onActivityResult(requestCode, resultCode, data);
        if(data!=null){
            //是获取注册界面回传过来的用户名
            // getExtra().getString("***");
            String userName=data.getStringExtra("userName");
            if(!TextUtils.isEmpty(userName)){
                //设置用户名到 et_user_name 控件
                et_user_name.setText(userName);
                //et_user_name控件的setSelection()方法来设置光标位置
                et_user_name.setSelection(userName.length());
            }
        }
    }
}
