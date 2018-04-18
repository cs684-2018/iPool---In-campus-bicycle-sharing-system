angular.module('myapp', ['ngMaterial','ngRoute','ngResource','angular-jwt'])

.config(function($mdThemingProvider) {
  $mdThemingProvider.theme('default')
    .primaryPalette('teal')
    .accentPalette('blue')
    .dark();
})
.config(function Config($httpProvider, jwtOptionsProvider) {
	jwtOptionsProvider.config({
	  tokenGetter: ['options', function(options) {
	    // Skip authentication for any requests ending in .html
	    if (options.url.substr(options.url.length - 5) == '.html') {
	      return null;
	    }

	    return localStorage.getItem('id_token');
	  }]
	});

	$httpProvider.interceptors.push('jwtInterceptor');
});