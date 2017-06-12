// Core client manager
var mqttClient;

function startConnnection() {
  //client = new Paho.MQTT.Client("192.168.0.94", 1884, "clientId");
  client = new Paho.MQTT.Client("ws://iot.eclipse.org/ws", "Fun" + new Date().getTime());

  // set callback handlers
  client.onConnectionLost = onConnectionLost;
  client.onMessageArrived = onMessageArrived;

  // connect the client
  client.connect({ onSuccess: onConnect });
}

// called when the client connects
function onConnect() {
  // Once a connection has been made, make a subscription and send a message.
  console.log("onConnect");
  client.subscribe("uniajc/fluxo-metter/#");
  //message = new Paho.MQTT.Message("Hello");
  //message.destinationName = "World";
  //client.send(message);
}

// called when the client loses its connection
function onConnectionLost(responseObject) {
  if (responseObject.errorCode !== 0) {
    console.log("onConnectionLost:" + responseObject.errorMessage);
  }
}

// called when a message arrives
function onMessageArrived(message) {
  signal = message.payloadString;
  console.log("onMessageArrived:" + signal);

  $('#current').text(signal);

/*
  if (signal == "ON") {
    $('#btn-control').removeClass('btn-warning').addClass('btn-success');

  } else {
    $('#btn-control').removeClass('btn-success').addClass('btn-warning');
  }
*/
}

function changeStatus() {
  console.log(estado);

  signal = "OFF"
  if (estado) {
    signal = "ON";
    estado = false;
  } else {
    estado = true;
    //message = new Paho.MQTT.Message("OFF");
  }
  message = new Paho.MQTT.Message(signal);
  message.destinationName = "BRAVE/feeds/onoff";
  client.send(message);
}

// READY: El documento esta cargado
// Asocia click del botón
// Se invoca la conexión

$().ready(function () {
/*
  $('#btn-control').bind("click", function () {
    changeStatus();
  });*/
  //startConnnection();
});
