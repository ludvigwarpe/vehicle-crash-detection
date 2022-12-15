package com.example.androidclient;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.DialogInterface;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

public class MainActivity extends AppCompatActivity{

    private static final String SERVER_URI = "tcp://test.mosquitto.org:1883";
    private static final String TAG = "MqttController";
    private static final String SENSORS_URI = "luwa9626/vehicle-crash-detection/sensors/";
    private static final String SENSOR_IMPACT = "impact";
    private static  final String GPS_LOCATION = "gps/location";
    private static  final String GPS_SPEED = "gps/speed";

    private MqttAndroidClient client;

    private AlertDialog alert;
    private TextView txv_connection;
    private TextView txv_location;
    private TextView txv_speed;

    private String currentLocation = "";
    private String currentSpeed = "";
    private String carFlipped = "";


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        txv_connection = (TextView) findViewById(R.id.txv_connectionValue);
        txv_location = (TextView) findViewById(R.id.txv_locationValue);
        txv_speed = (TextView) findViewById(R.id.txv_speedValue);

        connect();

        client.setCallback(new MqttCallbackExtended() {
            @Override
            public void connectComplete(boolean reconnect, String serverURI) {
                if (reconnect) {
                    System.out.println("Reconnected to : " + serverURI);
                    txv_connection.setText("OK");
                    txv_connection.setTextColor(Color.GREEN);
                    // Re-subscribe as we lost it due to new session
                    subscribe(SENSORS_URI + SENSOR_IMPACT);
                    subscribe(SENSORS_URI + GPS_LOCATION);
                    subscribe(SENSORS_URI + GPS_SPEED);
                } else {
                    System.out.println("Connected to: " + serverURI);
                    txv_connection.setText("OK");
                    txv_connection.setTextColor(Color.GREEN);
                    subscribe(SENSORS_URI + SENSOR_IMPACT);
                    subscribe(SENSORS_URI + GPS_LOCATION);
                    subscribe(SENSORS_URI + GPS_SPEED);
                }
            }

            @Override
            public void connectionLost(Throwable cause) {
                txv_connection.setText("ERROR");
                txv_connection.setTextColor(Color.RED);
                System.out.println("The Connection was lost.");
            }

            @Override
            public void messageArrived(String topic, MqttMessage message) throws
                    Exception {
                if (topic.equals((SENSORS_URI + SENSOR_IMPACT))) {
                    String newMessage = new String(message.getPayload());
                    carFlipped = newMessage;
                    buildAlertDialog();
                    alert.show();
                    //Toast.makeText(getApplicationContext(),newMessage,Toast.LENGTH_SHORT).show(); //SHOWS QUICK POP-UP ON SCREEN IF IMPACT HAPPENS
                    System.out.println("Incoming message: " + newMessage);
                }else if (topic.equals((SENSORS_URI + GPS_LOCATION))){
                    String newMessage = new String(message.getPayload());
                    currentLocation = newMessage;
                    txv_location.setText(newMessage);
                    //System.out.println("Incoming message: " + newMessage);
                }else if (topic.equals((SENSORS_URI + GPS_SPEED))){
                    String newMessage = new String(message.getPayload());
                    currentSpeed = newMessage;
                    txv_speed.setText(newMessage);
                    //System.out.println("Incoming message: " + newMessage);
                }
            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken token) {
            }
        });



        //mqttController = new MqttController(this.getApplicationContext(), txv_connection);


    }

    private void connect() {
        String clientId = MqttClient.generateClientId();
        System.out.println("CLIENT ID " + clientId);
        client = new MqttAndroidClient(this.getApplicationContext(), SERVER_URI, clientId);

        try {
            IMqttToken token = client.connect();
            token.setActionCallback(new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    // We are connected
                    Log.d(TAG, "onSuccess");
                    System.out.println(TAG + " Success. Connected to " + SERVER_URI);
                    txv_connection.setText("OK");
                    txv_connection.setTextColor(Color.GREEN);
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    // Something went wrong e.g. connection timeout or firewall problems

                    Log.d(TAG, "onFailure");
                    System.out.println(TAG + " Oh no! Failed to connect to " +
                            SERVER_URI);
                    txv_connection.setText("ERROR");
                    txv_connection.setTextColor(Color.RED);
                }
            });
        } catch (MqttException e) {
            System.out.println("ERROR: " + e.getMessage());
            e.printStackTrace();
        }
    }

    private void subscribe(String topicToSubscribe) {
        final String topic = topicToSubscribe;
        int qos = 1;
        try {
            IMqttToken subToken = client.subscribe(topic, qos);
            subToken.setActionCallback(new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    System.out.println("Subscription successful to topic: " + topic);
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken,
                                      Throwable exception) {
                    System.out.println("Failed to subscribe to topic: " + topic);
                    // The subscription could not be performed, maybe the user was not
                    // authorized to subscribe on the specified topic e.g. using wildcards
                }
            });
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    private void  buildAlertDialog(){
        AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
        String msg = "Collision detected\n" + "\nLocation:" + currentLocation + "\nSpeed:" + currentSpeed + "\n" + carFlipped + "\nEMS will be notified.";
        builder.setMessage(msg);
        System.out.println(msg);
        builder.setTitle("VEHICLE COLLISION");

        builder.setPositiveButton("OK", (DialogInterface.OnClickListener) (dialog, which) -> {
            dialog.dismiss();
            });

        builder.setNegativeButton("CANCEL", (DialogInterface.OnClickListener) (dialog, which) -> {
            dialog.cancel();
            });

        alert = builder.create();


    }
}