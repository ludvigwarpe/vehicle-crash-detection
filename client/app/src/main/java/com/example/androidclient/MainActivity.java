package com.example.androidclient;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.DialogInterface;
import android.graphics.Color;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.util.Log;
import android.widget.Button;
import android.widget.TextView;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import java.util.Locale;
import java.util.concurrent.TimeUnit;

public class MainActivity extends AppCompatActivity {

    private static final String SERVER_URI = "tcp://test.mosquitto.org:1883";
    private static final String TAG = "MqttController";
    private static final String SENSORS_URI = "luwa9626/vehicle-crash-detection/sensors/";
    private static final String SENSOR_IMPACT = "impact";
    private static final String GPS_LATITUDE = "gps/latitude";
    private static final String GPS_LONGITUDE = "gps/longitude";
    private static final String GPS_SPEED = "gps/speed";

    private MqttAndroidClient client;

    private TextView txv_connection;
    private TextView txv_latitude;
    private TextView txv_longitude;
    private TextView txv_speed;

    private String currentLatitude = "";
    private String currentLongitude = "";
    private String currentSpeed = "";
    private String collisionType = "";
    private boolean hasCollided = false;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        txv_connection = (TextView) findViewById(R.id.txv_connectionValue);
        txv_latitude = (TextView) findViewById(R.id.txv_latitudeValue);
        txv_longitude = (TextView) findViewById(R.id.txv_longitudeValue);
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
                    subscribe(SENSORS_URI + GPS_LATITUDE);
                    subscribe(SENSORS_URI + GPS_LONGITUDE);
                    subscribe(SENSORS_URI + GPS_SPEED);
                } else {
                    System.out.println("Connected to: " + serverURI);
                    txv_connection.setText("OK");
                    txv_connection.setTextColor(Color.GREEN);
                    subscribe(SENSORS_URI + SENSOR_IMPACT);
                    subscribe(SENSORS_URI + GPS_LATITUDE);
                    subscribe(SENSORS_URI + GPS_LONGITUDE);
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
                    collisionType = newMessage;
                    buildAlertDialog();

                }
                if (topic.equals((SENSORS_URI + GPS_LATITUDE))) {
                    String newMessage = new String(message.getPayload());
                    currentLatitude = newMessage;
                    txv_latitude.setText(newMessage);
                }
                if (topic.equals((SENSORS_URI + GPS_LONGITUDE))) {
                    String newMessage = new String(message.getPayload());
                    currentLongitude = newMessage;
                    txv_longitude.setText(newMessage);
                }

                if (topic.equals((SENSORS_URI + GPS_SPEED))) {
                    String newMessage = new String(message.getPayload());
                    currentSpeed = newMessage;
                    txv_speed.setText(newMessage);
                }
            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken token) {
            }
        });
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

    private void buildAlertDialog(){
        AlertDialog alertDialog = new AlertDialog.Builder(this)
                .setTitle("VEHICLE COLLISION")
                .setMessage(getAlertString())
                .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.dismiss();
                    }
                })
                .setNegativeButton("CANCEL", null)
                .create();
        addTimerToDialog(alertDialog);
        alertDialog.show();
    }

    private void addTimerToDialog(AlertDialog alertDialog){
        alertDialog.setOnShowListener(new DialogInterface.OnShowListener() {
            private static final int AUTO_DISMISS_MILLIS = 10000;

            @Override
            public void onShow(final DialogInterface dialog) {
                final Button button = ((AlertDialog) dialog).getButton(AlertDialog.BUTTON_NEGATIVE);
                final CharSequence negativeButtonText = button.getText();

                new CountDownTimer(10000, 100) {
                    @Override
                    public void onTick(long millisUntilFinished) {
                        button.setText(String.format(
                                Locale.getDefault(), "%s (%d)",
                                negativeButtonText,
                                TimeUnit.MILLISECONDS.toSeconds(millisUntilFinished) + 1 //add one so it never displays zero
                        ));
                    }
                    @Override
                    public void onFinish() {
                        if (((AlertDialog) dialog).isShowing()) {
                            dialog.dismiss();
                        }
                    }
                }.start();
            }
        });
    }

    @NonNull
    private String getAlertString() {
        boolean flipped = false;
        if (collisionType.equalsIgnoreCase("flipped")){
            flipped = true;
            collisionType = "";
        }

        String msg = "\nLATITUDE:" + currentLatitude
                + "\nLONGITUDE:" + currentLongitude
                +"\nSPEED:" + currentSpeed + " km/h"
                + "\nCAR ROLL: " + flipped
                + "\n\nEMS will be notified.";
        return msg;
    }
}