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
        echo '<script>alert("���������ݣ�");history.go(-1);</script>';
        exit(0);
    }
    getConnect();
    $inSQL = "update `m_information` set m_deal = '����Ա".$s_no."����:$contend' where m_no = '$m_no'";
	mysql_query($inSQL);
	$userSQL = "select * from `m_information` where m_deal = '����Ա".$s_no."����:$contend' and m_no = '$m_no'";
    $userResult = mysql_query($userSQL);
    if (mysql_fetch_array($userResult)) {
        echo '<script>alert("�޸����Լ�������Ϣ�ɹ���");window.location.href="ad_home.php?s_no='.$s_no.'";</script>';
    } else {
        echo '<script>alert("ϵͳ��æ�����Ժ����ԣ�");history.go(-1);</script>';
        exit(0);
    }
    closeConnect();
?>