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
	
	public static final int REQ_ZXING = 1;		//��ά��ɨ��
	
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
		//����ļ��Ƿ����
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
		//apiע��
		WXapi = WXAPIFactory.createWXAPI(c, WX_APP_ID, false);
        WXapi.registerApp(WX_APP_ID);
        SendAuth.Req req = new SendAuth.Req();
        //��Ȩ��ȡ�û���Ϣ
        req.scope = "snsapi_userinfo";
        //�Զ�����Ϣ
        req.state = "WechatLogin";
        //��΢�ŷ�������
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
		//req.scene = SendMessageToWX.Req.WXSceneSession;		//���ַ���
		req.scene = SendMessageToWX.Req.WXSceneTimeline;		//��������Ȧ
		req.message = msg;
		WXapi.sendReq(req);
	}
	
	
	public static void WeChatShardBbp(String WX_APP_ID)
	{
		
		
	}
	
	//��ά��ɨ��
	public static void ZXingScan()
	{
		Log.d("ɨ���ά��","=================hcb1");		
		Bundle data = new Bundle();
		//data.putString(key, value);
		Message msg = new Message();
		msg.what = REQ_ZXING;
		msg.setData(data);
		c.mHandle.sendMessage(msg);
	}
	
	
	public void onHandleZXingScan()
	{
		Log.d("ɨ���ά��","=================hcb4");
		/*zxingscan scan = new zxingscan();
		scan.onCreate();*/
//		Intent intent = new Intent(c,zxingscan.class);
//		startActivityForResult(intent, 0);
		Intent intent=new Intent(c,CaptureActivity.class);
		startActivityForResult(intent, REQUEST_CODE_SCAN);
	}
	
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        // TODO Auto-generated method stub
		Log.d("ɨ���ά��","=================9");
     /* //���ж����ĸ�ҳ�淵�ع�����
        switch (requestCode) {
        case REQUEST_A:
            
            //���жϷ��ع�����������ǳɹ�����ʧ�ܻ���������ʲô����
            switch (resultCode) {
            case ActivityA.RESULT_SUCCESS:
                //�ɹ���
                break;
            case ActivityA.RESULT_FAILED:
                //ʧ����
                break;
            }
            break;
        case REQUEST_B:
            //ͬ�ϡ���
            break;
        }*/
		super.onActivityResult(requestCode, resultCode, data);		
		// ɨ���ά��/����ش�
		if (requestCode == REQUEST_CODE_SCAN && resultCode == RESULT_OK) {
			if (data != null) {

				String content = data.getStringExtra("codedContent");
				Bitmap bitmap = data.getParcelableExtra("codedBitmap");
				//qrCoded.setText("�������� \n" + content);
				//qrCodeImage.setImageBitmap(bitmap);
				Log.d("ɨ��Ľ��:", "================"+content);	
				openURL(content);
			}
		}		
    }
	
	public void openURL(String url)
	{		
		//if(Patterns.WEB_URL.matcher(url).matches()) //��֤http://r.m.baidu.com/3ii99ns���ֵ�ַ����
			Uri uri = Uri.parse(url);
			Intent intent = new Intent(Intent.ACTION_VIEW,uri);
	//		 Intent intent = new Intent();
	//		intent.setData(Uri.parse(url));//Url ������Ҫ�򿪵���ַ
	//	    intent.setAction(Intent.ACTION_VIEW);
			//intent.setClassName("com.UCMobile","com.uc.browser.InnerUCMobile");//��UC�����
			//intent.setClassName("com.tencent.mtt","com.tencent.mtt.MainActivity");//��QQ�����		
			startActivity(intent);
		
	}
}