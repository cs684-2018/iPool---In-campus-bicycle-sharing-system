var API_ROOT = "/api/"
angular.module('myapp')

.controller('HomeCtrl', function($scope,$http, Auth) {
	Auth.loginRequired();
  // $http.get(API_ROOT+'channels').then(function(res){
  // 	$scope.channels = res.data;
  // })
})
.controller('LoginCtrl',['$scope','$window','$http','$location',function($scope,$window,$http,$location){
    $scope.login=function(){
        $http.post(API_ROOT+'auth/authenticate', $scope.user).then(function(response){
            $window.localStorage.setItem('id_token', response.data.token);
            $location.path('/');
        }, function(response){
            alert(response.data.message);
        });
    }
}]);
