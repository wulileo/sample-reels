<?php
    include_once("database.php");

    $password = $_POST['password1'];
    $confirmPassword = $_POST['password2'];
	
	if ($password == ''){
        echo '<script>alert("请输入密码！");history.go(-1);</script>';
        exit(0);
    }
    // 判断输入密码与确认密码是否相同
    if ($password != $confirmPassword) {
        echo '<script>alert("两次密码不相等！");history.go(-1);</script>';
        exit(0);
    }
    getConnect();
	$num = mysql_num_rows(mysql_query("select * from `super_information`")) + 1;
    $inSQL = "insert into `super_information` values('$num', md5('$password'))";
	mysql_query($inSQL);
	$userSQL = "select * from `super_information` where s_no = '$num'";
    $userResult = mysql_query($userSQL);
    if (mysql_fetch_array($userResult)) {
        echo '<script>alert("注册成功，请记住您的管理员编号：'.$num.'");history.go(-2);</script>';
    } else {
        echo '<script>alert("系统繁忙，请稍后再试！");history.go(-1);</script>';
        exit(0);
    }
    closeConnect();
?>