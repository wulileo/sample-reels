<?php
	$parameter=$_SERVER["QUERY_STRING"];
	$num1=strpos($parameter,"=");
	$num2=strpos($parameter,"&");
	$name=substr($parameter,$num1+1,$num2-$num1-1);
	$f_no=substr($parameter,($num1+1+strlen($name.'&f_no=')));
	$mysqli=new mysqli('localhost','root','d1501450743h','evaluation');
    include_once("database.php");

    $grade = $_POST['grade'];
    $eva = $_POST['eva'];
	
    if (($grade !='0' && 
		$grade !='1' && 
		$grade !='2' && 
		$grade !='3' && 
		$grade !='4' && 
		$grade !='5' && 
		$grade !='6' && 
		$grade !='7' && 
		$grade !='8' && 
		$grade !='9' && 
		$grade !='10')||
		$grade == ''){
        echo '<script>alert("请输入0-10的评分！");history.go(-1);</script>';
        exit(0);
    }
	if ($eva == ''){
        echo '<script>alert("请输入评价！");history.go(-1);</script>';
        exit(0);
    }
    getConnect();
    $inSQL = "insert into `u_f_evaluation` values('$name', '$f_no','$eva','$grade')";
	mysql_query($inSQL);
	$userSQL = "select * from `u_f_evaluation` where name = '$name' and f_no = '$f_no' and eva = '$eva' and grade = '$grade'";
    $userResult = mysql_query($userSQL);
    if (mysql_fetch_array($userResult)) {
        echo '<script>alert("添加评价成功！");window.location.href="user_evaluation.php?'.$parameter.'";</script>';
    } else {
        echo '<script>alert("系统繁忙，请稍后再试！");history.go(-1);</script>';
        exit(0);
    }
    closeConnect();
?>