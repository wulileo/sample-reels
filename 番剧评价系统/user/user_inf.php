<html>
<head>
	<title>1652250 | 邓泓</title>
</head>
<body style="
				background-image: url(../pic/bg1.jpg);
				background-position: center center;
				background-repeat: no-repeat;
				background-size: cover;
				background-color: #000000;">
		<label style="
					position:absolute;
					top:10px;
					left:600px;
					color:rgba(255,255,255,0.7);
					font-size:60px;
					font-family:'楷书';
					font-weight:bolder;">番剧信息</label>
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
				width:220px;
				top:30px;
				background-color:rgba(255,255,255,0.7);
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
				onmouseover="this.style.backgroundColor='#000000';
							this.style.color='rgba(255,255,255,1)'"
				onmouseout="this.style.backgroundColor='rgba(255,255,255,0)';
							this.style.color='#000000'"
			type="button" onclick="window.location.href='./user_home.php?name='+GetQueryString('name')" value="返回主页">
	</div>
	<div style="
				position:absolute;
				left:10px;
				height:800px;
				width:700px;
				top:110px;
				background-color:rgba(255,255,255,0.7);
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
				echo '<a style="font-size:30px;" href="user_evaluation.php?'.$parameter.'&f_no='.$row["f_no"].'">'.$row["f_name"].'</a>'
				.'<label style="font-size:20px;">'.$row["f_time"].'|</label>'
				.'<label style="font-size:15px;">'.$row["f_sig1"].','.$row["f_sig2"].','.$row["f_sig2"].'|</label>'
				.'<label style="font-size:30px;">'.$row["f_comp"].'|</label>'.'<hr width="500px" style="border:0; background-image: linear-gradient(to right, rgba(0,0,0,0), rgba(0,0,0,1), rgba(255,255,255,0.7));" size=3 align=left color=#000000>';
				}
			$result->close();
			$mysqli->close();
		?>
	</div>
</body>
</html>