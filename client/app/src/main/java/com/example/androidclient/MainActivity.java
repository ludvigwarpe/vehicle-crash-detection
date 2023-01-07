package com.example.androidclient;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

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

    //MQTT URI and topics
    private static final String SERVER_URI = "tcp://test.mosquitto.org:1883";
    private static final String TAG = "MqttController";
    private static final String SENSORS_URI = "luwa9626/vehicle-crash-detection/sensors/";
    private static final String SENSOR_IMPACT = "impact";
    private static final String GPS_LATITUDE = "gps/latitude";
    private static final String GPS_LONGITUDE = "gps/longitude";
    private static final String GPS_SPEED = "gps/speed";
    public static final String ERROR = "ERROR";
    public static final String OK = "OK";

    //Eclipse Paho MQTT Client used for connecting to server
    private MqttAndroidClient client;

    //TextViews displaying data from MQTT messages
    private TextView txv_connection;
    private TextView txv_latitude;
    private TextView txv_longitude;
    private TextView txv_speed;

    //Variables for storing MQTT messages received
    private String currentLatitude = "";
    private String currentLongitude = "";
    private String currentSpeed = "";
    private String collisionType = "";


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        txv_connection = findViewById(R.id.txv_connectionValue);
        txv_latitude = findViewById(R.id.txv_latitudeValue);
        txv_longitude = findViewById(R.id.txv_longitudeValue);
        txv_speed = findViewById(R.id.txv_speedValue);

        connect(); //Connects to MQTT-server

        client.setCallback(new MqttCallbackExtended() {
            @Override
            public void connectComplete(boolean reconnect, String serverURI) {
                if (reconnect) {
                    System.out.println("Reconnected to : " + serverURI);
                    txv_connection.setText(OK);
                    txv_connection.setTextColor(Color.GREEN);
                    // Re-subscribe to topics, due to new session being made.
                    subscribe(SENSORS_URI + SENSOR_IMPACT);
                    subscribe(SENSORS_URI + GPS_LATITUDE);
                    subscribe(SENSORS_URI + GPS_LONGITUDE);
                    subscribe(SENSORS_URI + GPS_SPEED);
                } else {
                    System.out.println("Connected to: " + serverURI);
                    txv_connection.setText(OK);
                    txv_connection.setTextColor(Color.GREEN);
                    subscribe(SENSORS_URI + SENSOR_IMPACT);
                    subscribe(SENSORS_URI + GPS_LATITUDE);
                    subscribe(SENSORS_URI + GPS_LONGITUDE);
                    subscribe(SENSORS_URI + GPS_SPEED);
                }
            }

            @Override
            public void connectionLost(Throwable cause) {
                txv_connection.setText(ERROR);
                txv_connection.setTextColor(Color.RED);
                System.out.println("The Connection was lost.");
            }

            //If messages arrive from subscribed topics, they will be handled and displayed to the user.
            @Override
            public void messageArrived(String topic, MqttMessage message) {
                if (topic.equals((SENSORS_URI + SENSOR_IMPACT))) {
                    collisionType = new String(message.getPayload());
                    buildAlertDialog(); //Displays alertDialog if a collision has been detected on the Arduino

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

    //Creates mqtt client and connects to specified server URI.
    private void connect() {
        String clientId = MqttClient.generateClientId();
        System.out.println("CLIENT ID " + clientId);
        client = new MqttAndroidClient(this.getApplicationContext(), SERVER_URI, clientId);

        try {
            IMqttToken token = client.connect();
            token.setActionCallback(new IMqttActionListener() {
                //Connection was successful!
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    Log.d(TAG, "onSuccess");
                    System.out.println(TAG + " Success. Connected to " + SERVER_URI);
                    txv_connection.setText(OK);
                    txv_connection.setTextColor(Color.GREEN);
                }

                //Connection failed, potential cause could be timeout or firewall.
                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    Log.d(TAG, "onFailure");
                    System.out.println(TAG + " Oh no! Failed to connect to " +
                            SERVER_URI);
                    txv_connection.setText(ERROR);
                    txv_connection.setTextColor(Color.RED);
                }
            });
        } catch (MqttException e) {
            System.out.println("ERROR: " + e.getMessage());
            e.printStackTrace();
        }
    }

    //Subscribes to given topic on MQTT-server
    private void subscribe(String topicToSubscribe) {
        final String topic = topicToSubscribe;
        int qos = 1; //Quality of service level 1 - sent at least once
        try {
            IMqttToken subToken = client.subscribe(topic, qos);
            subToken.setActionCallback(new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    System.out.println("Subscription successful to topic: " + topic);
                }

                // Subscription failed, could be caused by the user not being
                // authorized to subscribe on the specified topic.
                @Override
                public void onFailure(IMqttToken asyncActionToken,
                                      Throwable exception) {
                    System.out.println("Failed to subscribe to topic: " + topic);
                }
            });
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    //Builds AlertDialog displaying GPS + collision data
    private void buildAlertDialog() {
        AlertDialog alertDialog = new AlertDialog.Builder(this)
                .setTitle("VEHICLE COLLISION")
                .setMessage(getAlertString())
                .setPositiveButton(OK, (dialog, which) -> dialog.dismiss())
                .setNegativeButton("CANCEL", null)
                .create();
        addTimerToDialog(alertDialog);
        alertDialog.show();
    }

    //buildAlertDialog() helper-method, adds countdown to negativeButton in alertDialog
    private void addTimerToDialog(AlertDialog alertDialog) {
        alertDialog.setOnShowListener(dialog -> {
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
            });
        }


    //buildAlertDialog() helper-method, generates string of collision info to be shown in alertdialog.
    @NonNull
    private String getAlertString() {
        boolean flipped = false;
        if (collisionType.equalsIgnoreCase("flipped")) {
            flipped = true;
            collisionType = "";
        }

        return "\nLATITUDE:" + currentLatitude
                + "\nLONGITUDE:" + currentLongitude
                + "\nSPEED:" + currentSpeed + " km/h"
                + "\nCAR ROLL: " + flipped
                + "\n\nEMS will be notified.";

    }
}