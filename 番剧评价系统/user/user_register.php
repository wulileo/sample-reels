<?php
    include_once("database.php");

    $username = $_POST['username'];
    $password = $_POST['password1'];
    $confirmPassword = $_POST['password2'];
	$email = $_POST['email'];
	
	//û�������û���
    if ($username == ''){
        echo '<script>alert("�������û�����");history.go(-1);</script>';
        exit(0);
    }
	if ($password == ''){
        echo '<script>alert("���������룡");history.go(-1);</script>';
        exit(0);
    }
	if ($email == ''){
        echo '<script>alert("���������䣡");history.go(-1);</script>';
        exit(0);
    }
    // �ж�����������ȷ�������Ƿ���ͬ
    if ($password != $confirmPassword) {
        echo '<script>alert("�������벻��ȣ�");history.go(-1);</script>';
        exit(0);
    }
    // �ж��û����Ƿ��ظ�
    getConnect();
    $usernameSQL = "select * from `user_information` where name = '$username'";
    $resultSet = mysql_query($usernameSQL);
    if (mysql_num_rows($resultSet) > 0) {
        echo '<script>alert("�û��Ѵ��ڣ�");history.go(-1);</script>';
        exit(0);
    }
    $registerSQL = "insert into `user_information` values('$username', md5('$password'),'$email')";
	mysql_query($registerSQL);
	$userSQL = "select * from `user_information` where name = '$username'";
    $userResult = mysql_query($userSQL);
    if (mysql_fetch_array($userResult)) {
        echo '<script>alert("ע��ɹ������¼��");history.go(-2);</script>';
    } else {
        echo '<script>alert("ϵͳ��æ�����Ժ����ԣ�");history.go(-1);</script>';
        exit(0);
    }
    closeConnect();
?>