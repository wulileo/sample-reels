<?php
	$parameter=$_SERVER["QUERY_STRING"];
	$num1=strpos($parameter,"=");
	$name=substr($parameter,$num1+1);
	$mysqli=new mysqli('localhost','root','d1501450743h','evaluation');
    include_once("database.php");

    $contend = $_POST['contend'];
	
    if ($contend == ''){
        echo '<script>alert("请输入内容！");history.go(-1);</script>';
        exit(0);
    }
    getConnect();
	$num = mysql_num_rows(mysql_query("select * from `m_information`")) + 1;
    $inSQL = "insert into `m_information` values('$num','$name',now(),'$contend','未读')";
	mysql_query($inSQL);
	$userSQL = "select * from `m_information` where m_no = '$num'";
    $userResult = mysql_query($userSQL);
    if (mysql_fetch_array($userResult)) {
        echo '<script>alert("添加留言成功！");window.location.href="user_home.php?'.$parameter.'";</script>';
    } else {
        echo '<script>alert("系统繁忙，请稍后再试！");history.go(-1);</script>';
        exit(0);
    }
    closeConnect();
?>