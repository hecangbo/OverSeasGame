package com.moon.mahjong.wxapi;
import com.loopj.android.http.HttpGet;
import com.tencent.mm.opensdk.modelmsg.SendAuth;
import com.tencent.mm.opensdk.constants.ConstantsAPI;
import com.tencent.mm.opensdk.modelbase.BaseReq;
import com.tencent.mm.opensdk.modelbase.BaseResp;
import com.tencent.mm.opensdk.modelmsg.ShowMessageFromWX;
import com.tencent.mm.opensdk.modelmsg.WXAppExtendObject;
import com.tencent.mm.opensdk.modelmsg.WXMediaMessage;
import com.tencent.mm.opensdk.openapi.IWXAPI;
import com.tencent.mm.opensdk.openapi.IWXAPIEventHandler;
import com.tencent.mm.opensdk.openapi.WXAPIFactory;

import cz.msebera.android.httpclient.HttpEntity;
import cz.msebera.android.httpclient.HttpResponse;
import cz.msebera.android.httpclient.client.ClientProtocolException;
import cz.msebera.android.httpclient.client.HttpClient;
import cz.msebera.android.httpclient.impl.client.DefaultHttpClient;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URI;
import java.net.URLEncoder;

import org.json.JSONException;
import org.json.JSONObject;
import org.cocos2dx.lib.Cocos2dxLuaJavaBridge;

public class WXEntryActivity extends Activity implements IWXAPIEventHandler {
	public static final String TAG = "com.hcb.game.WeChat";
    private IWXAPI api;
    private BaseResp resp = null;
    
    public static String URL_FORMAT;
    public static String WX_APP_ID;
    public static String WX_APP_SECRET;
       

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        //注册
        api = WXAPIFactory.createWXAPI(this, WX_APP_ID, false);
        api.handleIntent(getIntent(), this);
    }

    // 微信发送请求到第三方应用时，会回调到该方法
    @Override
    public void onReq(BaseReq req) {
        finish();
    }

    // 第三方应用发送到微信的请求处理后的响应结果，会回调到该方法
    @Override
    public void onResp(BaseResp resp) {
        String result = "";
        Log.v(TAG,"=====WXEntryActivity onResp");
        Log.i(TAG, "=====onResp errCode " + resp.errCode);
		Log.i(TAG, "=====onResp openId " + resp.openId);
        switch (resp.errCode) {
        case BaseResp.ErrCode.ERR_OK:
            result = "发送成功";
            Toast.makeText(this, result, Toast.LENGTH_LONG).show();
            
//          SendAuth.Resp sendAuthResp = (SendAuth.Resp)resp;// 转换一下好取得code
//			String code = sendAuthResp.code;
            //获取微信传回的code
            String code = ((SendAuth.Resp) resp).code;            
            getAccessToken(code);
            
            finish();
            break;
        case BaseResp.ErrCode.ERR_USER_CANCEL:
            result = "发送取消";
            Toast.makeText(this, result, Toast.LENGTH_LONG).show();
            finish();
            break;
        case BaseResp.ErrCode.ERR_AUTH_DENIED:
            result = "发送被拒绝";
            Toast.makeText(this, result, Toast.LENGTH_LONG).show();
            finish();
            break;
        default:
            result = "发送返回";
            Toast.makeText(this, result, Toast.LENGTH_LONG).show();
            finish();
            break;
        }
    }
  

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        setIntent(intent);
        api.handleIntent(intent, this);
        finish();
    }

    /**
     * 获取access_token的URL（微信）
     * 
     * @param code
     *            授权时，微信回调给的
     * @return URL
     */
    private void getAccessToken(String code) {
    	String sUrl = String.format(URL_FORMAT,WX_APP_ID,WX_APP_SECRET,code);
		Log.v(TAG,".....hcb:sUrl:" + sUrl);
		 URI uri = URI.create(sUrl);
	        HttpClient client = new DefaultHttpClient();
	        HttpGet get = new HttpGet(uri);

	        HttpResponse response;
	        try {
	            response = client.execute(get);
	            if (response.getStatusLine().getStatusCode() == 200) {
	                HttpEntity entity = response.getEntity();
	                BufferedReader reader = new BufferedReader(new InputStreamReader(entity.getContent(), "UTF-8"));
	                StringBuilder sb = new StringBuilder();	
	                for (String temp = reader.readLine(); temp != null; temp = reader.readLine()) {
	                    sb.append(temp);
	                }
	                //Log.v(TAG,"sb:" + sb);
	                JSONObject object = new JSONObject(sb.toString().trim());
	                String accessToken = object.getString("access_token");
	                String openID = object.getString("openid");
	                String refreshToken = object.getString("refresh_token");
	                String unionID = object.getString("unionid");
	                Log.v(TAG,"===accessToken:" + accessToken);
	                Log.v(TAG,"===openId:" + openID);
	                Log.v(TAG,"===refreshToken:" +refreshToken);
	                Log.v(TAG,"===unionID:" +unionID);
					JSONObject msg = new JSONObject();  
					msg.put("accessToken", accessToken);
					msg.put("openID", openID);  
					msg.put("refreshToken", refreshToken);
					msg.put("unionID", unionID);
					Log.v(".....hcb:url==",msg.toString());
					Cocos2dxLuaJavaBridge.callLuaGlobalFunctionWithString("getWeChatLoginInfo",msg.toString());
	                finish();
	            }
	        } catch (ClientProtocolException e) {
	            // TODO Auto-generated catch block 
	            e.printStackTrace();
	        } catch (IOException e) {
	            // TODO Auto-generated catch block
	            e.printStackTrace();
	        } catch (IllegalStateException e) {
	            // TODO Auto-generated catch block
	            e.printStackTrace();
	        } catch (JSONException e) {
	            // TODO Auto-generated catch block
	            e.printStackTrace();
	        }		
    }
}