var express = require('express');
var router = express.Router();
var User = require('../models/sequelize_models').User;
var jwt_mid = require('express-jwt');
var secret = require('../config').secret;
var jwt = require('jsonwebtoken');
var bcrypt = require('bcrypt');
router.post('/authenticate', function(req, res) {

  // find the user
  User.findOne({ where: {username: req.body.username} }).then(user => {
    if( !user){
      res.status(401);
      res.json({ message: 'Authentication failed. User not found.' });
    } 
    else{            
      // check if password matches
      if (!bcrypt.compareSync(req.body.password, user.password)) {
        res.status(401);
        res.json({ message: 'Authentication failed. Wrong password.' });
      } 
      else{
        // if user is found and password is right
        // create a token
        var token = jwt.sign({
          data: user
        }, secret, { expiresIn: '24h' });

        // return the information including token as JSON
        res.send({
          message: 'Enjoy your token!',
          token: token
        });
      } 
    }
  })




});
router.get('/verify',jwt_mid({secret: secret}),function(req,res,next){
    res.sendStatus(200);
});

module.exports = router;