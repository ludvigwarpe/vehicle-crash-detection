package com.example.androidclient;

import android.content.Context;
import android.graphics.Color;
import android.provider.CalendarContract;
import android.util.Log;
import android.widget.TextView;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

public class MqttController {

    private static final String SERVER_URI = "tcp://test.mosquitto.org:1883";
    private static final String TAG = "MqttController";
    private static final String SENSORS_URI = "luwa9626/vehicle-crash-detection/sensors/";
    private static final String SENSOR_IMPACT = "impact";
    private static  final String GPS_LOCATION = "gps/location";
    private static  final String GPS_SPEED = "gps/speed";

    private MqttAndroidClient client;
    private Context context;


    public MqttController(Context context, TextView txv_connection) {
        this.context = context;

        connect();


        client.setCallback(new MqttCallbackExtended() {
            @Override
            public void connectComplete(boolean reconnect, String serverURI) {
                if (reconnect) {
                    System.out.println("Reconnected to : " + serverURI);
                    txv_connection.setText("OK");                   //TEST
                    // Re-subscribe as we lost it due to new session
                    subscribe(SENSORS_URI + SENSOR_IMPACT);
                    subscribe(SENSORS_URI + GPS_LOCATION);
                    subscribe(SENSORS_URI + GPS_SPEED);
                } else {
                    System.out.println("Connected to: " + serverURI);
                    txv_connection.setText("OK");           //TEST
                    subscribe(SENSORS_URI + SENSOR_IMPACT);
                    subscribe(SENSORS_URI + GPS_LOCATION);
                    subscribe(SENSORS_URI + GPS_SPEED);
                }
            }

            @Override
            public void connectionLost(Throwable cause) {
                txv_connection.setText("ERROR");//TEST
                System.out.println("The Connection was lost.");
            }

            @Override
            public void messageArrived(String topic, MqttMessage message) throws
                    Exception {
                //TODO: Behandla meddeleandet från kommande sensorer
                if (topic.equals((SENSORS_URI + SENSOR_IMPACT))) {
                    String newMessage = new String(message.getPayload());
                    System.out.println("Incoming message: " + newMessage);
                }else if (topic.equals((SENSORS_URI + GPS_LOCATION))){
                    String newMessage = new String(message.getPayload());
                    System.out.println("Incoming message: " + newMessage);
                }else if (topic.equals((SENSORS_URI + GPS_SPEED))){
                    String newMessage = new String(message.getPayload());
                    System.out.println("Incoming message: " + newMessage);
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
        client = new MqttAndroidClient(context, SERVER_URI, clientId);

        try {
            IMqttToken token = client.connect();
            token.setActionCallback(new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    // We are connected
                    Log.d(TAG, "onSuccess");
                    System.out.println(TAG + " Success. Connected to " + SERVER_URI);
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    // Something went wrong e.g. connection timeout or firewall problems

                    Log.d(TAG, "onFailure");
                    System.out.println(TAG + " Oh no! Failed to connect to " +
                            SERVER_URI);
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
}
