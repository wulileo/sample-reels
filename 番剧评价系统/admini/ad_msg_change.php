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
				left:600px;
				color:rgba(0,0,0,0.7);
				font-size:60px;
				font-family:'楷书';
				font-weight:bolder;">留言及反馈查看</label>
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
				top:10px;
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
			type="button" onclick="window.location.href='ad_home.php?s_no='+GetQueryString('s_no')" value="返回主页">
		
	</div>
	<div style="overflow-y:auto;
				position:absolute;
				left:10px;
				height:700px;
				width:560px;
				top:110px;
				background: transparent;
				border: solid 1px #000000;
				-moz-box-shadow:2px 2px 5px #000000; 
				-webkit-box-shadow:2px 2px 5px #000000; 
				box-shadow: 3px 12px 25px #000000;
				border-radius: 10px;
				border-radius-left,top,bottom,right;">
		<?php
			$parameter=$_SERVER["QUERY_STRING"];
			$num1=strpos($parameter,"=");
			$num2=strpos($parameter,"&");
			$name=substr($parameter,$num1+1,$num2-$num1-1);
			$m_no=substr($parameter,($num1+1+strlen($name.'&f_no=')));
			$mysqli=new mysqli('localhost','root','d1501450743h','evaluation');
			if (mysqli_connect_errno()){
				echo "连接失败，原因为：".mysqli_connect_error();
				exit();
				}
			$result = $mysqli->query ("set names gbk");
			$query = 'select * from m_information where m_no = "'.$m_no.'"';
			$result = $mysqli->query ($query);
			while($row=$result->fetch_assoc()){
				echo '<h1>'.$row["m_no"].'</h1><hr align=left width="80px" color=#000000>'
				.'<label style="
								font-size:20px;
								font-weight:bolder;">用户#</label>'.$row["name"].'<hr align=left width="150px" color=#000000>'
				.'<label style="
								font-size:20px;
								font-weight:bolder;">时间#</label>'.$row["m_time"].'<hr align=left width="150px" color=#000000>'
				.'<label style="
								font-size:20px;
								font-weight:bolder;">内容#</label>'.$row["m_com"].'<hr align=left width="150px" color=#000000>'
				.'<label style="
								font-size:20px;
								font-weight:bolder;">处理情况#</label>'.$row["m_deal"];
			}
			echo '</div>';
			echo '<div style="
						position:absolute;
						left:10px;
						height:70px;
						width:220px;
						top:830px;
						background: transparent;
						border: solid 1px #000000;
						-moz-box-shadow:2px 2px 5px #000000; 
						-webkit-box-shadow:2px 2px 5px #000000; 
						box-shadow: 3px 12px 25px #000000;
						border-radius: 10px;
						border-radius-left,top,bottom,right;">';
			echo '<input type="button" 
						style="
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
						font-family: \'黑体\';
						font-weight:bolder;"
						onmouseover="this.style.backgroundColor=\'#FFFFFF\';"
						onmouseout="this.style.backgroundColor=\'rgba(0,0,0,0)\';"
					type="button"
					onclick="window.location.href=\'./ad_ch_msg_deal.html?'.$parameter.'\'"
					value="修改">';
			$result->close();
			$mysqli->close();
		?>
	</div>
</body>
</html>