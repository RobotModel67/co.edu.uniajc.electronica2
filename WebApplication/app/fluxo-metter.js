/**
 * jorgeluisVALENCIAPLAZA
 * Fluxo-metter
 * Arquitectura de computadores, Ingenieria de Sistemas, UNIAJC 2017-01
 * 2017-06-06
 */

function fluxometterViewModel() {
    self = this;
    self.mqttServerName = "ws://iot.eclipse.org/ws";
    self.deviceId = "fluxo-metter-" + new Date().getTime();
    self.namespace = "uniajc/fluxo-metter/";

    self.valueOfUnit = 5.625;
    self.equivalente = 400;

    self.mqttClient = new Paho.MQTT.Client(
        self.mqttServerName, 1884, self.deviceId);
    /*
    self.mqttClient = new Paho.MQTT.Client(
        self.mqttServerName, 
        self.deviceId);
    */
    self.appModes = { ERROR: 0, IDLE: 1, AUTO: 2, MANUAL: 3, HISTORY: 4 };
    self.deviceCommands = {MANUAL: 1, AUTO: 2, START: 3, STOP: 4};
    self.deviceModes = { UNKNOW: -1, ERROR: 0, IDLE: 1, AUTO: 2, MANUAL: 3 };
    self.OnOff = {OFF: 0, ON: 1}

    self.appMode = ko.observable();
    self.deviceCommand = ko.observable();
    self.deviceMode = ko.observable();
    self.deviceOnOff = ko.observable();    

    self.volume = ko.observable(0);
    self.value = ko.observable(self.valueOfUnit);

    self.total = ko.computed(function() {
        return self.volume() * self.value();
    }, self);
    
    self.startConnnection = function () {
        console.log("startConnection: " + self.mqttServerName)
        // set callback handlers
        self.mqttClient.onConnectionLost = self.onConnectionLost;
        self.mqttClient.onMessageArrived = self.onMessageArrived;
        // connect the client
        self.mqttClient.connect({ onSuccess: self.onConnect });
    };

    self.onConnect = function() {
        console.log("Connected");
        self.mqttClient.subscribe(self.namespace + "device/#");
    };

    self.onConnectionLost = function (responseObject) {
        if (responseObject.errorCode !== 0) {
            console.log("onConnectionLost:" + responseObject.errorMessage);
        }
    };

    self.sendMQTTMessage = function(topic, message) {
        msg = new Paho.MQTT.Message(message);
        msg.destinationName = self.namespace + "app/" + topic;
        self.mqttClient.send(msg);
    };

    self.onMessageArrived = function (message) {
        signal = message.payloadString;
        path = message.destinationName.split('/');
        topic = path.pop();
        console.log("onMessageArrived:" + topic + ': ' + signal);

        switch (topic) {
            case 'onOff':
                    self.deviceOnOff(self.OnOff[signal]);
                break;
            case 'mode':
                    self.appMode(self.appModes[signal]);
                break;
            case 'volume':
                    self.volume(signal);            
                    //self.total(signal * self.valueOfUnit);
                break;
            case 'value':
                    self.value(signal);            
                break
            default:
                break;
        }

    };


    self.setAppMode = function (mode) {
        console.log('setAppMode: ' + mode);
        self.appMode(mode);
    };

}

var fmtter = new fluxometterViewModel(); 
ko.applyBindings(fmtter);
fmtter.startConnnection();