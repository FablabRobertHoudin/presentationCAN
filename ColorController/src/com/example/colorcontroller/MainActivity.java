package com.example.colorcontroller;

import java.io.IOException;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

import android.os.Bundle;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Color;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;

public class MainActivity extends Activity {

	private static final String TAG = "ColorControl";

	private SeekBar barR = null, barG = null, barB = null, barX = null;
	private BluetoothAdapter btAdapter = null;
	private BluetoothSocket btSocket = null;
	private OutputStream outStream = null;
	// Well known SPP UUID
	private static final UUID MY_UUID = UUID
			.fromString("00001101-0000-1000-8000-00805F9B34FB");
	
	private String BTadr = null;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		barR = (SeekBar) findViewById(R.id.barRed);
		barR. setBackgroundColor(Color.RED);
		barG = (SeekBar) findViewById(R.id.barGreen);
		barG. setBackgroundColor(Color.GREEN);
		barB = (SeekBar) findViewById(R.id.barBlue);
		barB. setBackgroundColor(Color.BLUE);

		btAdapter = BluetoothAdapter.getDefaultAdapter();
		Log.d(TAG, "btAdapter: " + btAdapter);

		OnSeekBarChangeListener listener = new OnSeekBarChangeListener() {
			@Override
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				Log.d(TAG, "seekBar:" + seekBar);
				String txt = "=";
				txt += barR.getProgress() + ",";
				txt += barG.getProgress() + ",";
				txt += barB.getProgress() + ",";
				//txt += skX.getProgress() + "\r\n";
				// text1.setText(seekBar.getTag()+": "+progress);
				Log.d(TAG, "msg: '" + txt+"'");
				try {
					outStream.write(txt.getBytes());
					outStream.flush();
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}

			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
				// TODO Auto-generated method stub

			}

			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
				// TODO Auto-generated method stub

			}
		};

		barR.setOnSeekBarChangeListener(listener);
		barG.setOnSeekBarChangeListener(listener);
		barB.setOnSeekBarChangeListener(listener);
}

	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		Log.d(TAG, "...In onPause()...");
		if (outStream != null) {
			try {
				outStream.close();
			} catch (IOException e) {
				Log.d(TAG, "In onPause() and failed to flush output stream: "
						+ e.getMessage());
			}
		}
		try {
			btSocket.close();
		} catch (IOException e2) {
			Log.d(TAG,
					"In onPause() and failed to close socket."
							+ e2.getMessage());
		}

	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		Log.d(TAG, "...In onResume - Attempting client connect...");
		try {
			barR.setEnabled(false);
			barG.setEnabled(false);
			barB.setEnabled(false);
			BluetoothDevice device = btAdapter.getRemoteDevice(BTadr);
					//.getRemoteDevice("00:12:F3:08:EE:DC"); // Arduino
			Log.d(TAG, "BT device: " + device + ", " + device.getBondState());
			btSocket = device.createRfcommSocketToServiceRecord(MY_UUID);
			Log.d(TAG, "btSocket: " + btSocket);
			btSocket.connect();
			outStream = btSocket.getOutputStream();
			Log.d(TAG, "outStream: " + outStream);
			barR.setEnabled(true);
			barG.setEnabled(true);
			barB.setEnabled(true);
		} catch (Exception e) {
			Log.d(TAG, "In onResume(): " + e.getMessage() + ", btSocket="
					+ btSocket + ", outStream=" + outStream);
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		//getMenuInflater().inflate(R.menu.activity_main, menu);

		Log.d(TAG, "onCreateOptionsMenu");
		Set<BluetoothDevice> pairedDevices = btAdapter.getBondedDevices();
		if (pairedDevices.size() > 0) {
			for (BluetoothDevice device : pairedDevices) {
				if (device.getBluetoothClass().getMajorDeviceClass() == 7936) {// UNCATEGORIZED
					Log.d(TAG, "device: " + device.getName()+", "+device.getBluetoothClass()+", "+device.getAddress());
					menu.add(device.getName());
				}
			}
		}
/*		
		this.registerReceiver(new BroadcastReceiver() {
			@Override
			public void onReceive(Context context, Intent intent) {
				 if (BluetoothDevice.ACTION_FOUND.equals(intent.getAction())) {
					 BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
						Log.d(TAG, "device OK: " + device.getName()+", "+device.getBluetoothClass()+", "+device.getAddress());
						//menu.add(device.getName());
				 }
			}}, new IntentFilter(BluetoothDevice.ACTION_FOUND));
		btAdapter.startDiscovery();
*/
		return true;
	}
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		Set<BluetoothDevice> pairedDevices = btAdapter.getBondedDevices();
		for (BluetoothDevice device : pairedDevices) {
			if (device.getName().equals(item.toString())) {
				Log.d(TAG, "menu item: " + item+" -> "+device.getAddress());
				BTadr = device.getAddress();
				onResume();
			}
		}
		return false;
	}

}
