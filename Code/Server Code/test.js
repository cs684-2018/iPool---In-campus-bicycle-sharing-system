// var mongoose = require('mongoose')
// var mqtt = require('mqtt')
var bcrypt = require('bcrypt');
var models = require('./models/sequelize_models');
var config = require('./config');
// models.User.create({
//   username: 'zgod',
//   password: bcrypt.hashSync('walkalone',config.salt),
//   type:'admin'
// }).then(user => {
//   // let's assume the default of isAdmin is false:
//   console.log(user.get({
//     plain: true
//   })) // => { username: 'barfooz', isAdmin: false }
// })

// models.User.findOne({ where: {username: "zgod"} }).then(user => {
//     if( !user)
//     	console.log("not found")
// 	else
// 		console.log(user.password)
// })
models.Device.create({}).then(device => {
  // let's assume the default of isAdmin is false:
  console.log(device.get({
    plain: true
  })) // => { username: 'barfooz', isAdmin: false }
})