/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2016 cocos2d-x.org
Copyright (c) 2013-2016 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.lua;

import java.io.File;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.R;
import android.R.string;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.util.Patterns;
import android.view.WindowManager;
import android.view.animation.AlphaAnimation;
import android.widget.ImageView;
import android.widget.TextView;

import com.karics.library.zxing.android.CaptureActivity;
import com.moon.mahjong.wxapi.*;
import com.tencent.mm.opensdk.modelmsg.SendAuth;
import com.tencent.mm.opensdk.modelmsg.SendMessageToWX;
import com.tencent.mm.opensdk.modelmsg.WXMediaMessage;
import com.tencent.mm.opensdk.modelmsg.WXTextObject;
import com.tencent.mm.opensdk.openapi.WXAPIFactory;
import com.tencent.mm.opensdk.openapi.IWXAPI;
import com.moon.mahjong.wxapi.*;
import com.zxing.zxingscan;
public class AppActivity extends Cocos2dxActivity{
	private static final int REQUEST_CODE_SCAN = 0x0000;
	
	public static final int REQ_ZXING = 1;		//二维码扫描
	
	private static AppActivity c = null;
	private static final String TAG ="APPACTIVITY";
	
	TextView qrCoded;
	ImageView qrCodeImage;
	
	public Handler mHandle = new Handler()
	{          
        public void handleMessage(Message msg) {  
			switch(msg.what){
				case REQ_ZXING:
					onHandleZXingScan();
					break;
			}
        };
	};
	
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		c = this;
		Log.d(TAG,   "==================hcb");
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,
				WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);		
	}	
	
	public static boolean InstallAPK(final String fileName){
		Log.d(TAG, "install apk path:" + fileName);
		Intent intent = new Intent(Intent.ACTION_VIEW); 
		//检查文件是否存在
		File dir  = new File(fileName);
		if(dir.exists())
			Log.d(TAG,   "apk is exist");
		else
			Log.d(TAG, "apk is not exist");
		
		intent.setDataAndType(Uri.fromFile(new File(fileName)), "application/vnd.android.package-archive"); 
		c.startActivity(intent);
		return true;
	}
	
	public static void WeChatInfo(final String appID, final String appSecret, final String accessTokenURL){
		Log.d(TAG, " appID=" + appID+";appSecret="+appSecret+ ";accessTokenURL="+accessTokenURL);
		WXEntryActivity.WX_APP_ID = appID;
		WXEntryActivity.WX_APP_SECRET = appSecret;
		WXEntryActivity.URL_FORMAT = accessTokenURL;
		WeChatAuthorize(appID);
	}
	
	public static void WeChatAuthorize(String WX_APP_ID){		
		IWXAPI WXapi;
		//api注册
		WXapi = WXAPIFactory.createWXAPI(c, WX_APP_ID, false);
        WXapi.registerApp(WX_APP_ID);
        SendAuth.Req req = new SendAuth.Req();
        //授权读取用户信息
        req.scope = "snsapi_userinfo";
        //自定义信息
        req.state = "WechatLogin";
        //向微信发送请求
        WXapi.sendReq(req);
	}
	
	public static void WeChatShard(String WX_APP_ID,String context){
		IWXAPI WXapi;
		WXapi = WXAPIFactory.createWXAPI(c, WX_APP_ID, false);
        WXapi.registerApp(WX_APP_ID);
		WXTextObject textObj = new WXTextObject();
		textObj.text = context;
		WXMediaMessage msg = new WXMediaMessage();
		msg.mediaObject = textObj;
		msg.description = context;
		
		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = String.valueOf(System.currentTimeMillis());
		//req.scene = SendMessageToWX.Req.WXSceneSession;		//文字分享
		req.scene = SendMessageToWX.Req.WXSceneTimeline;		//分享到朋友圈
		req.message = msg;
		WXapi.sendReq(req);
	}
	
	
	public static void WeChatShardBbp(String WX_APP_ID)
	{
		
		
	}
	
	//二维码扫描
	public static void ZXingScan()
	{
		Log.d("扫描二维码","=================hcb1");		
		Bundle data = new Bundle();
		//data.putString(key, value);
		Message msg = new Message();
		msg.what = REQ_ZXING;
		msg.setData(data);
		c.mHandle.sendMessage(msg);
	}
	
	
	public void onHandleZXingScan()
	{
		Log.d("扫描二维码","=================hcb4");
		/*zxingscan scan = new zxingscan();
		scan.onCreate();*/
//		Intent intent = new Intent(c,zxingscan.class);
//		startActivityForResult(intent, 0);
		Intent intent=new Intent(c,CaptureActivity.class);
		startActivityForResult(intent, REQUEST_CODE_SCAN);
	}
	
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        // TODO Auto-generated method stub
		Log.d("扫描二维码","=================9");
     /* //先判断是哪个页面返回过来的
        switch (requestCode) {
        case REQUEST_A:
            
            //再判断返回过来的情况，是成功还是失败还是其它的什么……
            switch (resultCode) {
            case ActivityA.RESULT_SUCCESS:
                //成功了
                break;
            case ActivityA.RESULT_FAILED:
                //失败了
                break;
            }
            break;
        case REQUEST_B:
            //同上……
            break;
        }*/
		super.onActivityResult(requestCode, resultCode, data);		
		// 扫描二维码/条码回传
		if (requestCode == REQUEST_CODE_SCAN && resultCode == RESULT_OK) {
			if (data != null) {

				String content = data.getStringExtra("codedContent");
				Bitmap bitmap = data.getParcelableExtra("codedBitmap");
				//qrCoded.setText("解码结果： \n" + content);
				//qrCodeImage.setImageBitmap(bitmap);
				Log.d("扫描的结果:", "================"+content);	
				openURL(content);
			}
		}		
    }
	
	public void openURL(String url)
	{		
		//if(Patterns.WEB_URL.matcher(url).matches()) //验证http://r.m.baidu.com/3ii99ns这种地址有误
			Uri uri = Uri.parse(url);
			Intent intent = new Intent(Intent.ACTION_VIEW,uri);
	//		 Intent intent = new Intent();
	//		intent.setData(Uri.parse(url));//Url 就是你要打开的网址
	//	    intent.setAction(Intent.ACTION_VIEW);
			//intent.setClassName("com.UCMobile","com.uc.browser.InnerUCMobile");//打开UC浏览器
			//intent.setClassName("com.tencent.mtt","com.tencent.mtt.MainActivity");//打开QQ浏览器		
			startActivity(intent);
		
	}
}