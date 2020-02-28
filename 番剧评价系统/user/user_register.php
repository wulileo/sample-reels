<?php
    include_once("database.php");

    $username = $_POST['username'];
    $password = $_POST['password1'];
    $confirmPassword = $_POST['password2'];
	$email = $_POST['email'];
	
	//没有输入用户名
    if ($username == ''){
        echo '<script>alert("请输入用户名！");history.go(-1);</script>';
        exit(0);
    }
	if ($password == ''){
        echo '<script>alert("请输入密码！");history.go(-1);</script>';
        exit(0);
    }
	if ($email == ''){
        echo '<script>alert("请输入邮箱！");history.go(-1);</script>';
        exit(0);
    }
    // 判断输入密码与确认密码是否相同
    if ($password != $confirmPassword) {
        echo '<script>alert("两次密码不相等！");history.go(-1);</script>';
        exit(0);
    }
    // 判断用户名是否重复
    getConnect();
    $usernameSQL = "select * from `user_information` where name = '$username'";
    $resultSet = mysql_query($usernameSQL);
    if (mysql_num_rows($resultSet) > 0) {
        echo '<script>alert("用户已存在！");history.go(-1);</script>';
        exit(0);
    }
    $registerSQL = "insert into `user_information` values('$username', md5('$password'),'$email')";
	mysql_query($registerSQL);
	$userSQL = "select * from `user_information` where name = '$username'";
    $userResult = mysql_query($userSQL);
    if (mysql_fetch_array($userResult)) {
        echo '<script>alert("注册成功，请登录！");history.go(-2);</script>';
    } else {
        echo '<script>alert("系统繁忙，请稍后再试！");history.go(-1);</script>';
        exit(0);
    }
    closeConnect();
?>