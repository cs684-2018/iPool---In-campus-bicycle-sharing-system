module.exports = {
    'secret': 'secret',
    'salt':10,
    'mqtt':{
        'url':"mqtt://test.mosquitto.org",
        'port':1833,
        'username':'username',
        'password':'password',
        'clientId': 'material-mean-boilerplate'
    },
    'database':{
        "mongo":{
            'URI': "mongodb://[username:password@]host:[port][/database]"
        },
        "mysql":{
            'host':'localhost',
            'name':'database',
            'username':'username',
            'password':'password'
        }  
    }
};