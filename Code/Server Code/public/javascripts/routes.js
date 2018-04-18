angular.module('myapp')
.config(function($routeProvider) {
    $routeProvider
    .when("/", {
        templateUrl : "templates/home.html",
        controller : "HomeCtrl"
    })
    .when("/login", {
        templateUrl : "templates/login.html",
        controller: "LoginCtrl"
    })
    .when("/blue", {
        templateUrl : "templates/blue.html"
    })
    .otherwise({
        redirectTo: '/'
    });
});