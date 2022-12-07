package com.example.androidclient;

import android.content.Context;
import android.util.Log;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

public class MqttController {

    //TODO: Fixa en egen broker? Kolla här: https://wildanmsyah.wordpress.com/2017/05/11/mqtt-android-client-tutorial/
    private static final String SERVER_URI = "tcp://test.mosquitto.org:1883";
    private static final String TAG = "MqttController";
    private static final String CLIENT_ID = "ExampleAndroidClient";
    private MqttAndroidClient client;

    public MqttController(Context context) {
        String clientId = MqttClient.generateClientId();
        System.out.println("CLIENT ID " + clientId);
        client = new MqttAndroidClient(context, SERVER_URI, clientId);

        client.setCallback(new MqttCallbackExtended() {
            @Override
            public void connectComplete(boolean reconnect, String serverURI) {
                if (reconnect) {
                    System.out.println("Reconnected to : " + serverURI);
                    //TODO: subscriba till våra kommande sensorer
                    
                    // Re-subscribe as we lost it due to new session
                    //subscribe(SENSORS_URI + SENSOR_LUX);
                    //subscribe(SENSORS_URI + SENSOR_PROXIMITY);
                } else {
                    System.out.println("Connected to: " + serverURI);
                    //subscribe(SENSORS_URI + SENSOR_LUX);
                    //subscribe(SENSORS_URI + SENSOR_PROXIMITY);
                }
            }

            @Override
            public void connectionLost(Throwable cause) {
                System.out.println("The Connection was lost.");
            }

            @Override
            public void messageArrived(String topic, MqttMessage message) throws
                    Exception {
                //TODO: Behandla meddeleandet från kommande sensorer
//                if (topic.equals((SENSORS_URI + SENSOR_LUX))) {
//                    String newMessage = new String(message.getPayload());
//                    System.out.println("Incoming message: " + newMessage);
//                    txv_light.setText(newMessage);
//                } else if (topic.equals((SENSORS_URI + SENSOR_PROXIMITY))) {
//                    String newMessage = new String(message.getPayload());
//                    System.out.println("Incoming message: " + newMessage);
//                    txv_proximity.setText(newMessage);
//                }

            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken token) {
            }
        });

        connect();
    }

    private void connect() {

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
