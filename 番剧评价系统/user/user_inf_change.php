<?php
	$parameter=$_SERVER["QUERY_STRING"];
	$num1=strpos($parameter,"=");
	$name=substr($parameter,$num1+1);
	$mysqli=new mysqli('localhost','root','d1501450743h','evaluation');
    include_once("database.php");

    $psd1 = $_POST['password1'];
    $psd2 = $_POST['password2'];
    $email = $_POST['email'];
	
    if ($psd1 == ''){
        echo '<script>alert("���������룡");history.go(-1);</script>';
        exit(0);
    }
	if ($psd1 != $psd2){
        echo '<script>alert("�������벻һ�£�");history.go(-1);</script>';
        exit(0);
    }
	if ($email == ''){
        echo '<script>alert("���������䣡");history.go(-1);</script>';
        exit(0);
    }
    getConnect();
	$selectSQL="select * from `user_information` where name = '$name' and psd = MD5('$psd1')";
    $resultSet = mysql_query($selectSQL);
	if (mysql_num_rows($resultSet) > 0) {
        echo '<script>alert("�����֮ǰ����һ����");history.go(-1);</script>';
        exit(0);
    }
    $inSQL = "update `user_information` set psd = MD5('$psd1'),email = '$email' where name = '$name'";
	mysql_query($inSQL);
	$userSQL = "select * from `user_information` where name = '$name' and psd = MD5('$psd1')";
    $userResult = mysql_query($userSQL);
    if (mysql_fetch_array($userResult)) {
        echo '<script>alert("�޸���Ϣ�ɹ���");window.location.href="user_home.php?'.$parameter.'";</script>';
    } else {
        echo '<script>alert("ϵͳ��æ�����Ժ����ԣ�");history.go(-1);</script>';
        exit(0);
    }
    closeConnect();
?>