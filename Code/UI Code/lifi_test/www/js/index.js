/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
var app = {
    // Application Constructor
    initialize: function() {
        document.addEventListener('deviceready', this.onDeviceReady.bind(this), false);
    },

    // deviceready Event Handler
    //
    // Bind any cordova events here. Common events are:
    // 'pause', 'resume', etc.
    onDeviceReady: function() {
        this.receivedEvent('deviceready');
        window.plugins.flashlight.available(function(isAvailable) {
          if (isAvailable) {

            // switch on
            // window.plugins.flashlight.toggle(
            //   function() {}, // optional success callback
            //   function() {}, // optional error callback
            //   {intensity: 0.3} // optional as well
            // );

            // switch off after 3 seconds
            
            document.getElementById("photx-lock").addEventListener("click", lock);
            document.getElementById("photx-unlock").addEventListener("click", unlock);

          } else {
            alert("Flashlight not available on this device");
          }
        });
    },

    // Update DOM on a Received Event
    receivedEvent: function(id) {
        var parentElement = document.getElementById(id);
        var listeningElement = parentElement.querySelector('.listening');
        var receivedElement = parentElement.querySelector('.received');

        listeningElement.setAttribute('style', 'display:none;');
        receivedElement.setAttribute('style', 'display:block;');

        console.log('Received Event: ' + id);
    }
};

app.initialize();

function photx(binaryString){
    // binaryString= 0x99;

    var phoTx = setInterval(function() {
        if (binaryString ==0){
            clearInterval(phoTx);
            window.plugins.flashlight.switchOff(
              function() {}, // optional success callback
              function() {}, // optional error callback
            );
        }
        bit = binaryString & 0x1;
        console.log(bit);
        if(bit){
            window.plugins.flashlight.switchOn(
              function() {}, // optional success callback
              function() {}, // optional error callback
            );
        } 
        else{
            window.plugins.flashlight.switchOff(
              function() {}, // optional success callback
              function() {}, // optional error callback
            );
        }  
        binaryString = binaryString >> 1;
        
    }, 50);
}
function calibrate(callback,args){
    window.plugins.flashlight.switchOn(
      function() {}, // optional success callback
      function() {}, // optional error callback
    );

    setTimeout(function(){
        window.plugins.flashlight.switchOff(
          function() {}, // optional success callback
          function() {}, // optional error callback
        );
        setTimeout(function(){
            window.plugins.flashlight.switchOn(
              function() {}, // optional success callback
              function() {}, // optional error callback
            );
            setTimeout(function(){
                window.plugins.flashlight.switchOff(
                  function() {}, // optional success callback
                  function() {}, // optional error callback
                );
                setTimeout(function(){
                    callback(args);
                },50)
            },50)
        },1000)
    },1000);
    
}
function lock(){
    // calibrate(photx,0xAA);
    photx(0xAA);

}
function unlock(){
    // calibrate(photx, 0x99);
    photx(0x99);
}