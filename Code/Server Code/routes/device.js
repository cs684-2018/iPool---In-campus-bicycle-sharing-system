var express = require('express');
var router = express.Router();
var awsIot = require('aws-iot-device-sdk');
var models = require('../models/sequelize_models');
var config = require('../config');
var jwt_mid = require('express-jwt');
var secret = require('../config').secret;
var jwt = require('jsonwebtoken');

var device = awsIot.device({
    keyPath: "./config/private.key.pem", //fill the path for private key
    certPath: "./config/certificate.crt.pem", //fill the path for certificate
    caPath: "./config/rootCA.pem",  //fill the path for ca certificat


    clientId: "MyDevice",
    host: "a221a6r4ojicsi.iot.ap-southeast-1.amazonaws.com",
    region: "ap-southeast-1",
    port: 8883,
    debug: true
});

device.on('connect', function() {
    console.log('connected');
    // device.subscribe('$aws/things/thing39/shadow/update');
    device.publish('$aws/things/thing40/shadow/update', JSON.stringify({
   "state": {
       "reported": {
         "device63.206": 11.1234,
         "device63.207": 19.1234,
     }
 }
}));
});

device.on('close', function() {
    console.log('disconnected', arguments);
});

device.on('error', function() {
    console.log('error', arguments);
});

device.on('reconnect', function() {
    console.log('reconnecting', arguments);
});

device.on('message', function(topic, payload) {
    console.log('message', topic, payload.toString());
});

device.on('timeout', function(thingName, clientToken) {
    console.log('received timeout');
});

/* POST device location. */
router.get('/:id', function(req, res, next) {
	console.log("publishing to AWS")
	device.publish('$aws/things/thing40/shadow/update', JSON.stringify({
	   "state": {
	       "reported": {
	         "device63.206": req.query.latitude,
	         "device63.207": req.query.longitude,
	     }
	 }
	}));
});


module.exports = router;
