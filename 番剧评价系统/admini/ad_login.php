<?php
    include_once("database.php");
    $username = addslashes($_POST['username']);
    $password = addslashes($_POST['password']);
    getConnect();
    $loginSQL = "select * from super_information where s_no='$username' and s_psd=MD5('$password')";
    $resultLogin = mysql_query($loginSQL);
    if (mysql_num_rows($resultLogin) > 0) {
        echo '<script>window.location.href="ad_home.php?s_no='.$username.'";</script>';
    } else {
        echo '<script>alert("管理员编号或密码错误！");history.go(-1);</script>';
    }
    closeConnect();
?>