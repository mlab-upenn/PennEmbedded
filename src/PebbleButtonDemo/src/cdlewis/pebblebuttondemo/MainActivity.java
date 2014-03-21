package cdlewis.pebblebuttondemo;

import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.UUID;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.getpebble.android.kit.PebbleKit;
import com.getpebble.android.kit.util.PebbleDictionary;

public class MainActivity extends Activity {
	
	Button button; 
    String direction;
	//Keys - identical to watch app keys
	private static final int DATA_KEY = 1;
	private static final int SELECT_BUTTON_KEY = 0;
	private static final int UP_BUTTON_KEY = 1;
	private static final int DOWN_BUTTON_KEY = 2;
	
	private static final int BUFFER_LENGTH = 32;
	
	//Use the same UUID as on the watch
	//private static final UUID APP_UUID = UUID.fromString("f1db3db2-0b0e-47c1-86af-e28d5ac7767c");
	private static final UUID APP_UUID = UUID.fromString("f0734b0e-189e-4399-b2a0-f1616adde554");
	private PebbleKit.PebbleDataReceiver dataHandler;
	private TextView statusLabel;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		//Allocate the TextView used to show which button press was received
		statusLabel = (TextView) findViewById(R.id.buttonLabel);
		
	}
	
	@Override
	public void onResume() {
		statusLabel.setText("In Resume");
		super.onResume();
		//Start the companion app on the watch
		PebbleKit.startAppOnPebble(getApplicationContext(), APP_UUID);
		statusLabel.setText("In Resume2");
		//Create a DataReciever
		//sendStringToPebble("Connected");
		dataHandler = new PebbleKit.PebbleDataReceiver(APP_UUID) {
			@Override
			public void receiveData(final Context context, final int transactionId, final PebbleDictionary data) {         
				//ACK to prevent timeouts
				PebbleKit.sendAckToPebble(context, transactionId);
				//Get which button was pressed
				boolean selectionkeyExists = data.contains(SELECT_BUTTON_KEY); 
				boolean upkeyExists = data.contains(UP_BUTTON_KEY);
				boolean downkeyExists = data.contains(DOWN_BUTTON_KEY);
				int buttonPressed ;
				
				if(selectionkeyExists)
					buttonPressed= 0;//data.size();//getUnsignedInteger(DATA_KEY).intValue();//getUnsignedInteger(1).intValue();
				else if(upkeyExists)
					buttonPressed = 1;
				else if(downkeyExists)
					buttonPressed =2;
				else 
					buttonPressed = 3;
				//statusLabel.setText(buttonPressed);
				//buttonPressed = data.getUnsignedInteger(DATA_KEY).intValue();
				//Update UI
				switch(buttonPressed) {
					case SELECT_BUTTON_KEY: {
						direction = "1";
						statusLabel.setText("Select button pressed");
                        sendtoPi(1);
						//sendStringToPebble("Phone says 'Select pressed!'");
						break;
					}
					case UP_BUTTON_KEY: {
						direction = "2";
						statusLabel.setText("Up button pressed");
						sendtoPi(2);
						//sendStringToPebble("Phone says 'Up pressed!'");
						break;
					}
					case DOWN_BUTTON_KEY: {
						direction = "3";
						statusLabel.setText("Down button pressed");
						sendtoPi(3);
						//sendStringToPebble("Phone says 'Down pressed!'");
						break;
					}
					default: {
						direction = "0";
						statusLabel.setText("Unknown button!");
						break;
					}
				}
			}
		};
		
		//Register the DataHandler with Android to receive any messages from the watch
		PebbleKit.registerReceivedDataHandler(getApplicationContext(), dataHandler);
	}

	@Override
	public void onPause() {
		super.onPause();
		
		// Always deregister any Activity-scoped BroadcastReceivers when the Activity is paused
		if (dataHandler != null) {
			unregisterReceiver(dataHandler);
			dataHandler = null;
		}
	}
	
	/**
	 * Send a string to the companion Pebble Watch App
	 * @param message	The string to send
	 */
	private void sendStringToPebble(String message) {
		if(message.length() < BUFFER_LENGTH) {

			//Create a PebbleDictionary
			PebbleDictionary dictionary = new PebbleDictionary();	

			//Store a string in the dictionary using the correct key
			dictionary.addString(DATA_KEY, message);	
			
			//Send the Dictionary
			PebbleKit.sendDataToPebble(getApplicationContext(), APP_UUID, dictionary);	
			
		} else {
			Log.i("sendStringToPebble()", "String too long!");
		}
	}
	
	public void sendtoPi(int cmd) {
		Thread cThread = new Thread(new ClientThread());
        cThread.start();
        
        try {
			cThread.join();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
      }
	
	public class ClientThread implements Runnable {
		
		public void run(){
			statusLabel.setText("Opening Socket");
			OutputStream dos;
	        Socket clnt;
			try {
				
				clnt = new Socket("158.130.37.106",5000);
				dos = clnt.getOutputStream();
	             dos.write(direction.getBytes());
	             /*BufferedReader in;
	             in = new BufferedReader(new InputStreamReader(clnt.getInputStream()));
	             int fromServer;
	             fromServer = Integer.parseInt(in.readLine());
	             if (fromServer==1){
	            	startActivity(i);
	             	USERNAME=username;
	             }
	             else{
	            	 startActivity(j);
	             }*/
	             
	        	 clnt.close(); 
			} catch (UnknownHostException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

}
