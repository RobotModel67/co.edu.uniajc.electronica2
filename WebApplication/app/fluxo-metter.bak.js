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




    


    self.setCommand = function (newCommand) {
        console.log('setCommand' + newCommand);
        self.deviceCommand(newCommand);
    };

}

var fluxometter = new fluxometterViewModel(); 
ko.applyBindings(fluxometter);
fluxometter.startConnnection();