import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.util.*;

import org.apache.http.HttpEntity;
import org.apache.http.HttpException;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;


/**
 * sending data to the PHP server in form of HTTP Post request
 * @author utsavmehta
 *
 */



public class WanderingNetworkModule {

	public WanderingNetworkModule(String xmldata) {
		
	//public static void main(String[] args) throws HttpException {
		 	//Sending post request to the server
			String url = "http://127.0.0.1/~utsavmehta/OHC%20Database/test.php";
		 	try {
		 	HttpClient httpclient = new DefaultHttpClient();	
		 	HttpPost httppost = new HttpPost(url);

		 	// Request parameters and other properties.
		 	List<NameValuePair> params = new ArrayList<NameValuePair>(2);
		 	params.add(new BasicNameValuePair("Authenticate","OHCData"));//authentication of the client
		 	params.add(new BasicNameValuePair("SNo","8"));
		 	params.add(new BasicNameValuePair("Name","Utsav"));
		 	params.add(new BasicNameValuePair("XML",xmldata));
		    //adding all the information to be sent to the arraylist
		    /*params.add(new BasicNameValuePair("HandlerID","1"));
		 	params.add(new BasicNameValuePair("HandlerName","SpO2-handler"));
		 	params.add(new BasicNameValuePair("SNo","8"));
		 	params.add(new BasicNameValuePair("type","PaO2"));
		 	params.add(new BasicNameValuePair("Value","120"));
		 	params.add(new BasicNameValuePair("Ontology","NONE"));
		 	params.add(new BasicNameValuePair("encoding","INT32"));
		 	params.add(new BasicNameValuePair("rtSpec","Periodic"));
		 	params.add(new BasicNameValuePair("rtInterval","5000"));
		 	params.add(new BasicNameValuePair("rangeUpper","100"));
		 	params.add(new BasicNameValuePair("rangeLower","50"));
		 	params.add(new BasicNameValuePair("ohcTimestamp","4504"));
		 	params.add(new BasicNameValuePair("devTimestamp","5485"));
		 	*/
				httppost.setEntity(new UrlEncodedFormEntity(params, "UTF-8"));
				HttpResponse response = httpclient.execute(httppost);
			 	//Handling the response from the server
				HttpEntity entity = response.getEntity();
			 	String result="";
			 	if (entity != null) {
			 	    InputStream instream = entity.getContent();
			 	    try {
			 	    	    BufferedReader reader = new BufferedReader(new InputStreamReader(instream));
					        StringBuilder sb = new StringBuilder();
					        String line = null;

					       while ((line = reader.readLine()) != null) {
					           sb.append(line);
					       }
					       result=sb.toString();
					       System.out.println(result);
			 	    } finally {
			 	        instream.close();
			 	    }
			 	    
			 	}
			 	
			} catch (UnsupportedEncodingException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}  catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}		 	
 			
 			
	    
	    }
	/*public static void main(String[] args) throws HttpException {
		WanderingNetworkModule mod=new WanderingNetworkModule();
	}*/
}
	   

