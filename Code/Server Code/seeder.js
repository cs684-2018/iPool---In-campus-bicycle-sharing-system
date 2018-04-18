var mongoose = require('mongoose');
var config = require('./config');

mongoose.connect(config.database.mongo.URI);
var db = mongoose.connection;
db.on('error', console.error.bind(console, 'connection error:'));
db.once('open', function() {
  	console.log("Connected to Mongo");
  	var User = require('./models/User');
	var u = User.create({username:"Test",password:"test"},function(err,user){
		if(err)
			console.log(err);
		else
			console.log(user);
	})
});
