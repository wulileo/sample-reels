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
					top:20px;
					left:720px;
					color:rgba(255,255,255,0.7);
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
	<label style="
			position:absolute;
			top:300px;
			left:720px;
			color:rgba(255,255,255,0.7);
			font-size:40px;
			font-family:'楷书';
			font-weight:bolder;">番剧推荐</label>
	<label style="
			position:absolute;
			top:730px;
			left:720px;
			color:rgba(255,255,255,0.7);
			font-size:40px;
			font-family:'楷书';
			font-weight:bolder;">留言反馈</label>
	<div style="
				position:absolute;
				left:10px;
				height:70px;
				width:700px;
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
						width:190px;
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
			type="button" onclick="window.location.href='./user_inf.php?name='+GetQueryString('name')" value="查看番剧信息">
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
				onmouseover="this.style.backgroundColor='#000000';
							this.style.color='rgba(255,255,255,1)'"
				onmouseout="this.style.backgroundColor='rgba(255,255,255,0)';
							this.style.color='#000000'"
			type="button" onclick="window.location.href='./user_msg.html?name='+GetQueryString('name')" value="留言及反馈">
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
						width:190px;
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
			type="button" onclick="window.location.href='./user_inf_change.html?name='+GetQueryString('name')" value="修改个人信息">
		<input style="
						border: solid 1px #000000;
						-moz-box-shadow:2px 2px 5px #000000; 
						-webkit-box-shadow:2px 2px 5px #000000; 
						box-shadow: 1px 3px 5px #000000;
						border-radius: 5px;
						border-radius-left,top,bottom,right;
						position:absolute;
						top:10px;
						left:590px;
						width:100px;
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
			type="button" onclick="window.location.href='../user_login.html'" value="退出">
	</div>
	<div style="overflow-y:auto;
				position:absolute;
				left:10px;
				height:500px;
				width:700px;
				top:110px;
				background-color:rgba(255,255,255,0.7);
				border: solid 1px #000000;
				-moz-box-shadow:2px 2px 5px #000000; 
				-webkit-box-shadow:2px 2px 5px #000000; 
				box-shadow: 3px 12px 25px #000000;
				border-radius: 10px;
				border-radius-left,top,bottom,right;">
		<?php
			$parameter=$_SERVER["QUERY_STRING"];
			$num1=strpos($parameter,"=");
			$name=substr($parameter,$num1+1);
			$mysqli=new mysqli('localhost','root','d1501450743h','evaluation');
			if (mysqli_connect_errno()){
				echo "连接失败，原因为：".mysqli_connect_error();
				exit();
			}
			$result = $mysqli->query("select * from `f_information`");
			$max = $result->num_rows;
			$num = rand(0,$max - 1);
			$count = 0;
			$result = $mysqli->query ("set names gbk");
			$query = "select * from f_information";
			$result = $mysqli->query ($query);
			while($row=$result->fetch_assoc()){
				if($count == $num){
					echo '<h1>'.$row["f_name"].'</h1><hr size=3 align=left width="250px" color=#000000><br>'
					.$row["f_time"].'<hr size=1 align=left width="80px" color=#000000><br>'
					.$row["f_comp"].'<hr size=1 align=left width="100px" color=#000000><br>'
					.$row["f_sig1"].','.$row["f_sig2"].','.$row["f_sig3"].'<hr size=1 align=left width="100px" color=#000000><br>'
					.'&#160'.'&#160'.'&#160'.'&#160'.'&#160'.'&#160'.'&#160'.'&#160'.$row["f_view"];
				}
				$count++;
			}
			$num++;
			$query = "select avg(grade) from u_f_evaluation where f_no = '$num'";
			$result = $mysqli->query ($query);
			while($row=$result->fetch_assoc()){
				echo '<label style="
							position:absolute;
							top:10px;
							left:460px;
							font-size:150px;
							font-weight:bolder;">'.round($row["avg(grade)"],1).'</label>';
			}
			echo '</div>';
			echo '<div style="
						overflow-y:auto;
						position:absolute;
						left:10px;
						height:300px;
						width:700px;
						top:620px;
						background-color:rgba(255,255,255,0.7);
						border: solid 1px #000000;
						-moz-box-shadow:2px 2px 5px #000000; 
						-webkit-box-shadow:2px 2px 5px #000000; 
						box-shadow: 3px 12px 25px #000000;
						border-radius: 10px;
						border-radius-left,top,bottom,right;">';
			$query = "select * from m_information where name = '$name'";
			$result = $mysqli->query ($query);
			while($row=$result->fetch_assoc()){
				echo '<label style="
								font-size:20px;
								font-weight:bolder;">'.$row["m_no"].'#</label>'.$row["m_time"].'<hr align=left width="120px" color=#000000 boder:none>'
				.'<label style="
								font-size:20px;
								font-weight:bolder;">内容#</label>'.$row["m_com"].'<hr color=#000000 boder:none>'
				.'<label style="
								font-size:20px;
								font-weight:bolder;">处理情况#</label>'.$row["m_deal"];
				echo '<hr size=2 color=#000000>';
			}
			$result->close();
			$mysqli->close();
		?>
	</div>
</body>
</html>