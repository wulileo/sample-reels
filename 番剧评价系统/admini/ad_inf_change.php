<?php
	$parameter=$_SERVER["QUERY_STRING"];
	$num1=strpos($parameter,"=");
	$s_no=substr($parameter,$num1+1);
	$mysqli=new mysqli('localhost','root','d1501450743h','evaluation');
    include_once("database.php");

    $psd1 = $_POST['password1'];
    $psd2 = $_POST['password2'];
	
    if ($psd1 == ''){
        echo '<script>alert("请输入密码！");history.go(-1);</script>';
        exit(0);
    }
	if ($psd1 != $psd2){
        echo '<script>alert("两次密码不一致！");history.go(-1);</script>';
        exit(0);
    }
    getConnect();
	$selectSQL="select * from `super_information` where s_no = '$s_no' and s_psd = MD5('$psd1')";
    $resultSet = mysql_query($selectSQL);
	if (mysql_num_rows($resultSet) > 0) {
        echo '<script>alert("请勿和之前密码一样！");history.go(-1);</script>';
        exit(0);
    }
    $inSQL = "update `super_information` set s_psd = MD5('$psd1') where s_no = '$s_no'";
	mysql_query($inSQL);
	$userSQL = "select * from `super_information` where s_no = '$s_no' and s_psd = MD5('$psd1')";
    $userResult = mysql_query($userSQL);
    if (mysql_fetch_array($userResult)) {
        echo '<script>alert("修改密码成功！");window.location.href="ad_home.php?'.$parameter.'";</script>';
    } else {
        echo '<script>alert("系统繁忙，请稍后再试！");history.go(-1);</script>';
        exit(0);
    }
    closeConnect();
?>