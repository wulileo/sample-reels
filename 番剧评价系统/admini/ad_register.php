<?php
    include_once("database.php");

    $password = $_POST['password1'];
    $confirmPassword = $_POST['password2'];
	
	if ($password == ''){
        echo '<script>alert("���������룡");history.go(-1);</script>';
        exit(0);
    }
    // �ж�����������ȷ�������Ƿ���ͬ
    if ($password != $confirmPassword) {
        echo '<script>alert("�������벻��ȣ�");history.go(-1);</script>';
        exit(0);
    }
    getConnect();
	$num = mysql_num_rows(mysql_query("select * from `super_information`")) + 1;
    $inSQL = "insert into `super_information` values('$num', md5('$password'))";
	mysql_query($inSQL);
	$userSQL = "select * from `super_information` where s_no = '$num'";
    $userResult = mysql_query($userSQL);
    if (mysql_fetch_array($userResult)) {
        echo '<script>alert("ע��ɹ������ס���Ĺ���Ա��ţ�'.$num.'");history.go(-2);</script>';
    } else {
        echo '<script>alert("ϵͳ��æ�����Ժ����ԣ�");history.go(-1);</script>';
        exit(0);
    }
    closeConnect();
?>