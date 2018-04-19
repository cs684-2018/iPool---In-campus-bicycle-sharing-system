var express = require('express');
var router = express.Router();
var bcrypt = require('bcrypt');
var models = require('../models/sequelize_models');
var config = require('../config');
var jwt_mid = require('express-jwt');
var secret = require('../config').secret;
var jwt = require('jsonwebtoken');
function dec2bin(dec){
    return (dec >>> 0).toString(2);
}

function iPoolHash(plaintext){
	var hash = config.key ^ plaintext; //simple XOR with secret. CHANGE THIS ASAP!!
	console.log(dec2bin(hash));
	var token = (hash | 0x8000) << 16 | plaintext; // shift hash by 2 bytes and append plaintext. Also make sure first bit is 1
	console.log(dec2bin(token));
	return dec2bin(token);
}
/* POST request for new ride. Returns 4 byte token. */
router.post('/', jwt_mid({secret: secret}), function(req, res, next) {
	models.Ride.create({user_id:req.user.id}).then(ride => {
		console.log(ride.get({
			plain: true
		})) // => { username: 'barfooz', isAdmin: false }
		res.json({"token":iPoolHash(ride.id)});
	})
});

module.exports = router;
