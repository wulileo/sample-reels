<?php
	$parameter=$_SERVER["QUERY_STRING"];
	$num1=strpos($parameter,"=");
	$s_no=substr($parameter,$num1+1);
	$mysqli=new mysqli('localhost','root','d1501450743h','evaluation');
    include_once("database.php");

    $f_name = $_POST['f_name'];
    $f_comp = $_POST['f_comp'];
    $f_time = $_POST['f_time'];
    $f_sig1 = $_POST['f_sig1'];
    $f_sig2 = $_POST['f_sig2'];
    $f_sig3 = $_POST['f_sig3'];
    $f_view = $_POST['f_view'];
	
    if ($f_name == ''){
        echo '<script>alert("番剧名不能空！");history.go(-1);</script>';
        exit(0);
    }
    if ($f_comp == ''){
        echo '<script>alert("制作公司不能空！");history.go(-1);</script>';
        exit(0);
    }
    if ($f_time == ''){
        echo '<script>alert("时间不能空！");history.go(-1);</script>';
        exit(0);
    }
    if ($f_view == ''){
        echo '<script>alert("简介不能空！");history.go(-1);</script>';
        exit(0);
    }
    getConnect();
	if(mysql_num_rows(mysql_query("select * from `f_information` where f_name = '$f_name'")) > 0){
		echo '<script>alert("请勿重复录入番剧！");history.go(-1);</script>';
        exit(0);
	}
	$num = mysql_num_rows(mysql_query("select * from `f_information`")) + 1;
    $inSQL = "insert into `f_information` values('$num', '$f_name','$f_time','$f_sig1','$f_sig2','$f_sig2','$f_comp','$f_view')";
	mysql_query($inSQL);
	$userSQL = "select * from `f_information` where f_name = '$f_name'";
    $userResult = mysql_query($userSQL);
    if (mysql_fetch_array($userResult)) {
        echo '<script>alert("添加番剧成功！");window.location.href="ad_inf.php?'.$parameter.'";</script>';
    } else {
        echo '<script>alert("系统繁忙，请稍后再试！");history.go(-1);</script>';
        exit(0);
    }
    closeConnect();
?>