<?php
	$parameter=$_SERVER["QUERY_STRING"];
	$num1=strpos($parameter,"=");
	$num2=strpos($parameter,"&");
	$s_no=substr($parameter,$num1+1,$num2-$num1-1);
	$m_no=substr($parameter,($num1+1+strlen($s_no.'&m_no=')));
	$mysqli=new mysqli('localhost','root','d1501450743h','evaluation');
    include_once("database.php");

    $contend = $_POST['contend'];
	
    if ($contend == ''){
        echo '<script>alert("请输入内容！");history.go(-1);</script>';
        exit(0);
    }
    getConnect();
    $inSQL = "update `m_information` set m_deal = '管理员".$s_no."处理:$contend' where m_no = '$m_no'";
	mysql_query($inSQL);
	$userSQL = "select * from `m_information` where m_deal = '管理员".$s_no."处理:$contend' and m_no = '$m_no'";
    $userResult = mysql_query($userSQL);
    if (mysql_fetch_array($userResult)) {
        echo '<script>alert("修改留言及反馈信息成功！");window.location.href="ad_home.php?s_no='.$s_no.'";</script>';
    } else {
        echo '<script>alert("系统繁忙，请稍后再试！");history.go(-1);</script>';
        exit(0);
    }
    closeConnect();
?>