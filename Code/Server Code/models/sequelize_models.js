var config = require('../config')
var models={};
// Database setup
var Sequelize = require('sequelize');
var sequelize = new Sequelize(config.database.mysql.name,config.database.mysql.username,config.database.mysql.password,{host:config.database.mysql.host,dialect:'mysql',timezone : "+5:30"});
models.sequelize = sequelize;
models.User = sequelize.define('user', {
  id: {
    type: Sequelize.INTEGER,
    primaryKey: true,
    autoIncrement: true
  },
  username: {
    type: Sequelize.STRING
  },
  password:{
    type:Sequelize.STRING
  },
  type:{
    type:Sequelize.STRING
  }
});
models.Device = sequelize.define('device', {
  id: {
    type: Sequelize.INTEGER,
    primaryKey: true,
    autoIncrement: true
  },
});
models.Ride = sequelize.define('ride', {
  id: {
    type: Sequelize.INTEGER,
    primaryKey: true,
    autoIncrement: true
  },
  user_id:{
    type: Sequelize.INTEGER,
    references: {
       model: models.User,
       key: 'id',
     }
  },
  device_id:{
    type: Sequelize.INTEGER,
    references: {
       model: models.Device,
       key: 'id',
     }
  }
});

models.User.hasMany(models.Ride,{foreignKey: 'user_id', targetKey: 'id'});
models.Ride.belongsTo(models.User,{foreignKey: 'user_id', targetKey: 'id'})
models.Device.hasMany(models.Ride,{foreignKey: 'device_id', targetKey: 'id'});
models.Ride.belongsTo(models.Device,{foreignKey: 'device_id', targetKey: 'id'})

module.exports=models
