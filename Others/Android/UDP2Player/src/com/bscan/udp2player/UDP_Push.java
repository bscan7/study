package com.bscan.udp2player;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;

import android.os.Handler;
import android.os.Message;
import android.os.Bundle;
import android.webkit.MimeTypeMap;
import android.widget.Button;
import android.widget.TextView;
 
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.text.SimpleDateFormat;
import java.util.Date;

public class UDP_Push  extends Activity {
    /*发送广播端的socket*/
    private MulticastSocket ms;
    /*发送广播的按钮*/
    private Button sendUDPBrocast;
    private String sPrefix = "";
    private String sUrl = "";
    private TextView TextView00;
    private TextView TextView01;
    
    public static void pushLog(final String sLog)
    {
    	Log.i("TAGG", android.os.Process.myTid() + " " + sLog);
//        new Thread(new Runnable() {
//            @Override
//            public void run() {
//            	//sendUDP(sLog, 8009);
//        		//发送的数据包，局网内的所有地址都可以收到该数据包
//        		DatagramPacket dataPacket = null;
//        		MulticastSocket ms;
//        		/*创建socket实例*/
//        		try {
//        		    ms = new MulticastSocket();
//
//        		    ms.setTimeToLive(4);
//        		    //将本机的IP（这里可以写动态获取的IP）地址放到数据包里，其实server端接收到数据包后也能获取到发包方的IP的
//        		    Date date = new Date();
//        		    SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd  HH:mm:ss.SSS");
//        		    String formatedDate = format.format(date);
//         
//         
////                                InetAddress localhost = InetAddress.getByName("localhost");
////                                String hostAddress = localhost.getHostAddress();
////                                String sendData = hostAddress +formatedDate;
//         
//         
//        		    //获取wifi服务
////        		    WifiManager wifiManager = (WifiManager) getSystemService(Context.WIFI_SERVICE);
////        		    //判断wifi是否开启
////        		    if (!wifiManager.isWifiEnabled()) {
////        		        wifiManager.setWifiEnabled(true);
////        		    }
////        		    WifiInfo wifiInfo = wifiManager.getConnectionInfo();
////        		    int ipAddress = wifiInfo.getIpAddress();
////        		    String ip = intToIp(ipAddress);
////         
//
//        		    //String sendData=ip+formatedDate;
////         		    String sendData = sPrefix + sUrl;
////         		    if(sPrefix.length() > 0)
////         		    	sendData += "?etag=12345";
////                	sPrefix = "";
//
//
////                                byte[] data = "192.168.1.109".getBytes();
//        		    byte[] data = ("\r\n"+sLog).getBytes();
//         
//        		    //224.0.0.1为广播地址
//        		    //InetAddress address = InetAddress.getByName("224.0.0.1");
//        		    InetAddress address = InetAddress.getByName("255.255.255.255");
//        		    //InetAddress address = InetAddress.getByName("192.168.1.255");
//        		    //这个地方可以输出判断该地址是不是广播类型的地址
//        		    System.out.println(address.isMulticastAddress());
//        		    dataPacket = new DatagramPacket(data, data.length, address,
//        		    		8009);
//        		    ms.send(dataPacket);
//        		    ms.close();
//        		} catch (Exception e) {
//        		    e.printStackTrace();
//        		}
//            }
//        }).start();
    }
 
    public String memUsing() {
    	ActivityManager activityManager = (ActivityManager) getSystemService(ACTIVITY_SERVICE);
        //最大分配内存
        int memory = activityManager.getMemoryClass();
        System.out.println("memory: "+memory);
        //最大分配内存获取方法2
       float maxMemory = (float) (Runtime.getRuntime().maxMemory() * 1.0/ (1024 * 1024));
        //当前分配的总内存
        float totalMemory = (float) (Runtime.getRuntime().totalMemory() * 1.0/ (1024 * 1024));
        //剩余内存
        float freeMemory = (float) (Runtime.getRuntime().freeMemory() * 1.0/ (1024 * 1024));
        return "mem: "+maxMemory
        		+" / "+totalMemory
        		+" / "+freeMemory;
 	}

	@Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        init();

        Intent intent = getIntent();
        String message = intent.getStringExtra(MainActivity.EXTRA_MESSAGE);
        
        Uri data = getIntent().getData();
        if(data != null)
        {
	        Log.i("TAG", "host = " + data.getHost() + " path = " + data.getPath() + " query = " + data.getQuery());
	        sUrl = data.toString();
	        
	     // Android 4.0 之后不能在主线程中请求网络
            new Thread(new Runnable(){
                @Override
                public void run() {
        	        setTxts();
        	        
         		    String sendData = sPrefix + sUrl;
         		    if(sPrefix.length() > 0)
         		    	sendData += "?etag=12345";
                	sPrefix = "";
                	sendUDP(sendData, 8003);
                }
            }).start();

	        
	        //result.setText(param+"]");
	     }
    }
 
    /*初始化参数*/
    public void init() {
        setContentView(R.layout.activity_main);
        
        TextView00 = (TextView) findViewById(R.id.TextView00);
        TextView01 = (TextView) findViewById(R.id.TextView01);
        
        TextView00.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
               // Toast.makeText(MainActivity.this, "实现点击TextView事件", Toast.LENGTH_SHORT).show();
               
                Thread thread = new Thread(new Runnable() {
                    @Override
                    public void run() {
                    	
            	        sUrl = TextView00.getText().toString();

             		    String sendData = sPrefix + sUrl;
             		    if(sPrefix.length() > 0)
             		    	sendData += "?etag=12345";
                    	sPrefix = "";
                    	sendUDP(sendData, 8003);
                    }
                });
     
                thread.start();
            }
        });

        TextView01.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
               // Toast.makeText(MainActivity.this, "实现点击TextView事件", Toast.LENGTH_SHORT).show();
               
                Thread thread = new Thread(new Runnable() {
                    @Override
                    public void run() {
            	        sUrl = TextView01.getText().toString();
             		    String sendData = sPrefix + sUrl;
             		    if(sPrefix.length() > 0)
             		    	sendData += "?etag=12345";
                    	sPrefix = "";
                    	sendUDP(sendData, 8003);
                    }
                });
     
                thread.start();
            }
        });

        sendUDPBrocast = (Button) findViewById(R.id.send);
        sendUDPBrocast.setOnClickListener(new SendUDPBrocastListener());
        Button sendUDPBrocast2 = (Button) findViewById(R.id.senddd);
        sendUDPBrocast2.setOnClickListener(new SendUDPBrocastListener2());
       try {
            /*创建socket实例*/
            ms = new MulticastSocket();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    private String int2IP(int i) {
 
        return (i & 0xFF ) + "." +
                ((i >> 8 ) & 0xFF) + "." +
                ((i >> 16 ) & 0xFF) + "." +
                ( i >> 24 & 0xFF) ;
    }
 
    public void sendUDP(String sendData, int iPort) {
		//发送的数据包，局网内的所有地址都可以收到该数据包
		DatagramPacket dataPacket = null;
		/*创建socket实例*/
		try {
		    ms = new MulticastSocket();
		} catch (IOException e) {
		    e.printStackTrace();
		}
		try {
		    ms.setTimeToLive(4);
		    //将本机的IP（这里可以写动态获取的IP）地址放到数据包里，其实server端接收到数据包后也能获取到发包方的IP的
		    Date date = new Date();
		    SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd  HH:mm:ss.SSS");
		    String formatedDate = format.format(date);
 
 
//                        InetAddress localhost = InetAddress.getByName("localhost");
//                        String hostAddress = localhost.getHostAddress();
//                        String sendData = hostAddress +formatedDate;
 
 
		    //获取wifi服务
		    WifiManager wifiManager = (WifiManager) getSystemService(Context.WIFI_SERVICE);
		    //判断wifi是否开启
		    if (!wifiManager.isWifiEnabled()) {
		        wifiManager.setWifiEnabled(true);
		    }
		    WifiInfo wifiInfo = wifiManager.getConnectionInfo();
		    int ipAddress = wifiInfo.getIpAddress();
		    String ip = int2IP(ipAddress);
 

		    //String sendData=ip+formatedDate;
// 		    String sendData = sPrefix + sUrl;
// 		    if(sPrefix.length() > 0)
// 		    	sendData += "?etag=12345";
//        	sPrefix = "";


//                        byte[] data = "192.168.1.109".getBytes();
		    byte[] data = sendData.getBytes();
 
		    //224.0.0.1为广播地址
		    //InetAddress address = InetAddress.getByName("224.0.0.1");
		    InetAddress address = InetAddress.getByName("255.255.255.255");
		    //InetAddress address = InetAddress.getByName("192.168.1.255");
		    //这个地方可以输出判断该地址是不是广播类型的地址
		    System.out.println(address.isMulticastAddress());
		    dataPacket = new DatagramPacket(data, data.length, address,
		    		iPort);
		    ms.send(dataPacket);
		    ms.close();
		} catch (Exception e) {
		    e.printStackTrace();
		}
	}

	public void setTxts() {
		String ArrStr[] = sUrl.split("http");
		if(ArrStr.length>0)
		{
			TextView01.setText("http" + ArrStr[ArrStr.length-1]);
		}
		TextView00.setText(sUrl);
	}

	/**
     * 单击按钮时，发送局域网广播
     * */
    class SendUDPBrocastListener implements View.OnClickListener {
 
        @Override
        public void onClick(View v) {
            Thread thread = new Thread(new Runnable() {
                @Override
                public void run() {
                	sPrefix = "https://www.playm3u8.cn/jiexi.php?url=";
         		    String sendData = sPrefix + sUrl;
         		    if(sPrefix.length() > 0)
         		    	sendData += "?etag=12345";
                	sPrefix = "";
                	sendUDP(sendData, 8003);
                }
            });
 
            thread.start();
 
        }
    }
    
	/**
     * 单击按钮时，发送局域网广播
     * */
    class SendUDPBrocastListener2 implements View.OnClickListener {
 
        @Override
        public void onClick(View v) {
            Thread thread = new Thread(new Runnable() {
                @Override
                public void run() {
                	sPrefix = "http://vip.vip-dianying.com/8050/?url=";
         		    String sendData = sPrefix + sUrl;
         		    if(sPrefix.length() > 0)
         		    	sendData += "?etag=12345";
                	sPrefix = "";
                	sendUDP(sendData, 8003);
                }
            });
 
            thread.start();
 
        }
    }
    

}
