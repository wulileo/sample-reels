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
				font-weight:bolder;">番剧评价</label>
	<div style="
				position:absolute;
				left:10px;
				height:70px;
				width:220px;
				top:10px;
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
	<label style="
			position:absolute;
			top:230px;
			left:720px;
			color:rgba(255,255,255,0.7);
			font-size:40px;
			font-family:'楷书';
			font-weight:bolder;">番剧信息</label>
	<label style="
			position:absolute;
			top:630px;
			left:720px;
			color:rgba(255,255,255,0.7);
			font-size:40px;
			font-family:'楷书';
			font-weight:bolder;">用户评价</label>
	<div style="overflow-y:auto;
				position:absolute;
				left:10px;
				height:350px;
				width:700px;
				top:90px;
				background-color:rgba(255,255,255,0.7);
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
		<?php
			$parameter=$_SERVER["QUERY_STRING"];
			$num1=strpos($parameter,"=");
			$num2=strpos($parameter,"&");
			$name=substr($parameter,$num1+1,$num2-$num1-1);
			$f_no=substr($parameter,($num1+1+strlen($name.'&f_no=')));
			$mysqli=new mysqli('localhost','root','d1501450743h','evaluation');
			if (mysqli_connect_errno()){
				echo "连接失败，原因为：".mysqli_connect_error();
				exit();
				}
			$result = $mysqli->query ("set names gbk");
			$query = 'select * from f_information where f_no = "'.$f_no.'"';
			$result = $mysqli->query ($query);
			while($row=$result->fetch_assoc()){
				echo '<h1>'.$row["f_name"].'</h1><hr size=3 align=left width="250px" color=#000000>'
					.$row["f_time"].'<hr size=1 align=left width="80px" color=#000000>'
					.$row["f_comp"].'<hr size=1 align=left width="100px" color=#000000>'
					.$row["f_sig1"].','.$row["f_sig2"].','.$row["f_sig3"].'<hr size=1 align=left width="100px" color=#000000>'
					.'&#160'.'&#160'.'&#160'.'&#160'.'&#160'.'&#160'.'&#160'.'&#160'.$row["f_view"];
			}
			$query = "select avg(grade) from u_f_evaluation where f_no = '$f_no'";
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
					height:370px;
					width:700px;
					top:450px;
					background-color:rgba(255,255,255,0.7);
					border: solid 1px #000000;
					-moz-box-shadow:2px 2px 5px #000000; 
					-webkit-box-shadow:2px 2px 5px #000000; 
					box-shadow: 3px 12px 25px #000000;
					border-radius: 10px;
					border-radius-left,top,bottom,right;">';
			$query = 'select * from u_f_evaluation where f_no = "'.$f_no.'"';
			$result = $mysqli->query ($query);
			while($row=$result->fetch_assoc()){
				echo '&#160'.'&#160'.'&#160'.'&#160'.'&#160'.'&#160'.'&#160'.'&#160'.$row["eva"].'<br>'
				.'<label>评分：'.$row["grade"].'</label><br>'
				.'<label>作者：'.$row["name"].'</label><hr size=1 color=#000000>';
			}
			echo '</div>';
			echo '<div style="
						position:absolute;
						left:10px;
						height:70px;
						width:220px;
						top:830px;
						background-color:rgba(255,255,255,0.7);
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
						onmouseover="this.style.backgroundColor=\'#000000\';
							this.style.color=\'rgba(255,255,255,1)\'"
						onmouseout="this.style.backgroundColor=\'rgba(255,255,255,0)\';
							this.style.color=\'#000000\'"
					onclick="window.location.href=\'./user_add_evaluation.html?'.$parameter.'\'"
					value="添加评价">';
			$result->close();
			$mysqli->close();
		?>
	</div>
</body>
</html>