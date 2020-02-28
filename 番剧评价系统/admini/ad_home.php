<html>
<head>
	<title>1652250 | 邓泓</title>
</head>
<body style="
				background-image: url(../pic/bg2.jpg);
				background-position: center center;
				background-repeat: no-repeat;
				background-size: cover;
				background-color: #000000;">
	<label style="
				position:absolute;
				top:20px;
				left:720px;
				color:rgba(0,0,0,0.7);
				font-size:60px;
				font-family:'楷书';
				font-weight:bolder;">主页</label>
	
	<script>
			function GetQueryString(name)
			{
				 var reg = new RegExp("(^|&)"+ name +"=([^&]*)(&|$)");
				 var r = window.location.search.substr(1).match(reg);
				 if(r!=null)return  unescape(r[2]); return null;
			}
	</script>
	<div style="
				position:absolute;
				left:10px;
				height:70px;
				width:500px;
				top:30px;
				background: transparent;
				border: solid 1px #000000;
				-moz-box-shadow:2px 2px 5px #000000; 
				-webkit-box-shadow:2px 2px 5px #000000; 
				box-shadow: 3px 12px 25px #000000;
				border-radius: 10px;
				border-radius-left,top,bottom,right;">
		<input style="
						border: solid 1px #000000;
						-moz-box-shadow:2px 2px 5px #000000; 
						-webkit-box-shadow:2px 2px 5px #000000; 
						box-shadow: 1px 3px 5px #000000;
						border-radius: 5px;
						border-radius-left,top,bottom,right;
						position:absolute;
						top:10px;
						left:10px;
						width:190px;
						height:50px;
						background: transparent;
						cursor:pointer;
						font-size:25px;
						font-family: '黑体';
						font-weight:bolder;"
				onmouseover="this.style.backgroundColor='#FFFFFF';"
				onmouseout="this.style.backgroundColor='rgba(0,0,0,0)';"
			type="button" onclick="window.location.href='ad_inf.php?s_no='+GetQueryString('s_no')" value="查看番剧信息">
		<input style="
						border: solid 1px #000000;
						-moz-box-shadow:2px 2px 5px #000000; 
						-webkit-box-shadow:2px 2px 5px #000000; 
						box-shadow: 1px 3px 5px #000000;
						border-radius: 5px;
						border-radius-left,top,bottom,right;
						position:absolute;
						top:10px;
						left:210px;
						width:170px;
						height:50px;
						background: transparent;
						cursor:pointer;
						font-size:25px;
						font-family: '黑体';
						font-weight:bolder;"
				onmouseover="this.style.backgroundColor='#FFFFFF';"
				onmouseout="this.style.backgroundColor='rgba(0,0,0,0)';"
			type="button" onclick="window.location.href='ad_inf_change.html?s_no='+GetQueryString('s_no')" value="修改密码">
		<input style="
						border: solid 1px #000000;
						-moz-box-shadow:2px 2px 5px #000000; 
						-webkit-box-shadow:2px 2px 5px #000000; 
						box-shadow: 1px 3px 5px #000000;
						border-radius: 5px;
						border-radius-left,top,bottom,right;
						position:absolute;
						top:10px;
						left:390px;
						width:100px;
						height:50px;
						background: transparent;
						cursor:pointer;
						font-size:25px;
						font-family: '黑体';
						font-weight:bolder;"
				onmouseover="this.style.backgroundColor='#FFFFFF';"
				onmouseout="this.style.backgroundColor='rgba(0,0,0,0)';"
			type="button" onclick="window.location.href='ad_login.html'" value="退出">
	</div>
	<div style="overflow-y:auto;
				position:absolute;
				left:10px;
				height:500px;
				width:700px;
				top:130px;
				background: transparent;
				border: solid 1px #000000;
				-moz-box-shadow:2px 2px 5px #000000; 
				-webkit-box-shadow:2px 2px 5px #000000; 
				box-shadow: 3px 12px 25px #000000;
				border-radius: 10px;
				border-radius-left,top,bottom,right;">
		<br>
		<?php
			$parameter=$_SERVER["QUERY_STRING"];
			$mysqli=new mysqli('localhost','root','d1501450743h','evaluation');
			if (mysqli_connect_errno()){
				echo "连接失败，原因为：".mysqli_connect_error();
				exit();
			}
			$result = $mysqli->query ("set names gbk");
			$query = "select * from m_information";
			$result = $mysqli->query ($query);
			while($row=$result->fetch_assoc()){
				echo '<label style="
								font-size:20px;
								font-weight:bolder;">'.$row["m_no"].'#</label>'.$row["m_time"].'<hr align=left width="120px" color=#000000 boder:none>'
				.'<label style="
								font-size:20px;
								font-weight:bolder;">用户名#</label>'.$row["name"].'<hr align=left width="180px" color=#000000 boder:none>';
				echo '<label style="
								font-size:20px;
								font-weight:bolder;">处理情况#</label><a href="ad_msg_change.php?'.$parameter.'&m_no='.$row["m_no"].'">'.$row["m_deal"].'</a>';
				echo '<hr size=2 align=left width="300px" color=#000000>';
			}
			$result->close();
			$mysqli->close();
		?>
	</div>
</body>
</html>