<?php
    include_once("database.php");
    $username = addslashes($_POST['username']);
    $password = addslashes($_POST['password']);
    getConnect();
    $loginSQL = "select * from user_information where name='$username' and psd=MD5('$password')";
    $resultLogin = mysql_query($loginSQL);
    if (mysql_num_rows($resultLogin) > 0) {
        echo '<script>window.location.href="user_home.php?name='.$username.'";</script>';
    } else {
        echo '<script>alert("用户名或密码错误！");history.go(-1);</script>';
    }
    closeConnect();
?>
