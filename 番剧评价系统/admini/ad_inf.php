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
				top:10px;
				left:650px;
				color:rgba(0,0,0,0.7);
				font-size:60px;
				font-family:'楷书';
				font-weight:bolder;">番剧信息</label>
	<div style="
				position:absolute;
				left:10px;
				height:70px;
				width:220px;
				top:10px;
				background: transparent;
				border: solid 1px #000000;
				-moz-box-shadow:2px 2px 5px #000000; 
				-webkit-box-shadow:2px 2px 5px #000000; 
				box-shadow: 3px 12px 25px #000000;
				border-radius: 10px;
				border-radius-left,top,bottom,right;">
		<script>
			function GetQueryString(name)
			{
				 var reg = new RegExp("(^|&)"+ name +"=([^&]*)(&|$)");
				 var r = window.location.search.substr(1).match(reg);
				 if(r!=null)return  unescape(r[2]); return null;
			}
		</script>
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
						width:200px;
						height:50px;
						background: transparent;
						cursor:pointer;
						font-size:25px;
						font-family: '黑体';
						font-weight:bolder;"
				onmouseover="this.style.backgroundColor='#FFFFFF';"
				onmouseout="this.style.backgroundColor='rgba(0,0,0,0)';"
			type="button" onclick="window.location.href='ad_home.php?s_no='+GetQueryString('s_no')" value="返回主页">
	</div>
	<div style="
				position:absolute;
				left:10px;
				height:650px;
				width:700px;
				top:110px;
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
			$query = "select * from f_information";
			$result = $mysqli->query ($query);
			while($row=$result->fetch_assoc()){
				echo '<label style="font-size:30px;">'.$row["f_name"].'</label>'
				.'<label style="font-size:20px;">'.$row["f_time"].'</label>'
				.'<label style="font-size:30px;">'.$row["f_comp"].'</label>'
				.'<label style="font-size:15px;">'.$row["f_sig1"].','.$row["f_sig2"].','.$row["f_sig3"].'</label>';
				echo '<hr style="border:0; background-image: linear-gradient(to right, rgba(0,0,0,0), rgba(0,0,0,1), rgba(255,255,255,0.7));" size=2 align=left color=#000000>';
			}
			$result->close();
			$mysqli->close();
		?>
	</div>
	<div style="
				position:absolute;
				left:10px;
				height:70px;
				width:220px;
				top:780px;
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
						width:200px;
						height:50px;
						background: transparent;
						cursor:pointer;
						font-size:25px;
						font-family: '黑体';
						font-weight:bolder;"
				onmouseover="this.style.backgroundColor='#FFFFFF';"
				onmouseout="this.style.backgroundColor='rgba(0,0,0,0)';"
			type="button" onclick="window.location.href='ad_add_f_inf.html?s_no='+GetQueryString('s_no')" value="添加番剧信息">
	</div>
</body>
</html>