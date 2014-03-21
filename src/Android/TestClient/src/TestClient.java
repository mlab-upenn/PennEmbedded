

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



public class TestClient {

	public TestClient() {
		// TODO Auto-generated constructor stub
	//}

	/**
	 * @param args
	 * @throws HttpException 
	 */
	//public static void main(String[] args) throws HttpException {
		 	//Sending post request to the server
			String url = "http://127.0.0.1/~utsavmehta/OHC%20Database/index.php";
		 	try {
		 	HttpClient httpclient = new DefaultHttpClient();
		 	HttpPost httppost = new HttpPost(url);

		 	// Request parameters and other properties.
		 	List<NameValuePair> params = new ArrayList<NameValuePair>(2);
		 	params.add(new BasicNameValuePair("Authenticate","OHCData"));
		 	params.add(new BasicNameValuePair("SNo", "22"));
		 	params.add(new BasicNameValuePair("Field", "Air Pressure"));
		 	
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
	public static void main(String[] args) throws HttpException {
		TestClient cl=new TestClient();
		//WanderingNetworkModule mod=new WanderingNetworkModule();
	}
}
	   

