var imgprocess 		= 0;
var arm 			= 2;
var trans 			= 1;
var imgrecognition 	= 0;
var tools 			= 0;

var menu_bar = 0;
var tool_bar = 0;
var saw		 = 0;
var pump	 = 0;

var rotate 	 = 0;
var vertical = 42;

var x;
var y;
var iniX;
var iniY;
var perX;
var perY;
var downstate = 0;

var forwardbtn  = 0;
var backwardbtn = 0;
var leftbtn		= 0;
var rightbtn	= 0;
var rollover	= 0;

var depthvalue = 0;
var speedvalue = 0;



function onclick_imgprocess() {
	imgprocess = imgprocess + 1;
	if(imgprocess == 3) {
		imgprocess = 0;
	}
	send('p' + imgprocess, "web/switches");
}

function onMessage_imgprocess(val) {
	imgprocess = Number(val);
	
	if(imgprocess == 1) {
		$("#imgprocess").css("opacity", "0.7");
		$("body").attr('style', 'background:url("/video_feed1") top left; background-size:100%;');
	}
	else if(imgprocess == 2) {
		$("#imgprocess").css("opacity", "1");
		$("body").attr('style', 'background:url("/video_feed2") top left; background-size:100%;');
	}
	else {
		$("#imgprocess").css("opacity", "0.4");
		$("body").attr('style', 'background:url("/video_feed0") top left; background-size:100%;');
	}
}



function showArmMenu() {
	if(menu_bar) {
		$(".Arm").css("display", "none");
		menu_bar = 0;
	}
	else {
		$(".Arm").css("display", "block");
		menu_bar = 1;
	}
}

function Arm_stretch() {
	if(arm != 1) {
		send("0", "Mega2/arm_tool");
		onMessage_Armstretch();
	}
	$(".Arm").css("display", "none");
	menu_bar = 0;
}

function onMessage_Armstretch() {
	arm = 1;
	$("#Arm_stretch").css({opacity: 0.4});
	$("#Arm_retrieve").css({opacity: 1});
	$("#Arm_grab").css({opacity: 1});
}

function Arm_retrieve() {
	if(arm != 2) {
		send("1", "Mega2/arm_tool");
		onMessage_Armretrieve();
	}
	$(".Arm").css("display", "none");
	menu_bar = 0;
}

function onMessage_Armretrieve() {
	arm = 2;
	$("#Arm_stretch").css({opacity: 1});
	$("#Arm_retrieve").css({opacity: 0.4});
	$("#Arm_grab").css({opacity: 1});
}

function Arm_grab() {
	if(arm != 3) {
		send("2", "Mega2/arm_tool");
		onMessage_Armgrab();
	}
	$(".Arm").css("display", "none");
	menu_bar = 0;
}

function onMessage_Armgrab() {
	arm = 3
	$("#Arm_stretch").css({opacity: 1});
	$("#Arm_retrieve").css({opacity: 1});
	$("#Arm_grab").css({opacity: 0.4});
}



function onclick_transform() {
	trans = ~trans + 2;
	if(trans) {
		send('Up', "Mega0/camera_trans");
	}
	else{
		send('Down', "Mega0/camera_trans");
	}
	onMessage_trans(trans);
}

function onMessage_trans(val) {
	trans = val;
	if(trans) {
		$("#transform").css("background-image", "url(../static/img/transform_expand.png)");
	} else {
		$("#transform").css("background-image", "url(../static/img/transform_shrink.png)");
	}
}

function onclick_imgrecognition() {
	imgrecognition = ~imgrecognition + 2;
	send('r' + imgrecognition, "web/switches");
}

function onMessage_imgrecognition(val) {
	imgrecognition = Number(val);
	console.log(1);
	if(imgrecognition) {
		$("#imgrecognition").css("opacity", "1");
		$("body").attr('style', 'background:url("/video_feed3") top left; background-size:100%;');
	} else {
		$("#imgrecognition").css("opacity", "0.4");
		$("body").attr('style', 'background:url("/video_feed0") top left; background-size:100%;');
	}
}

function onclick_camera() {
	send('Reset', "Mega0/camera_trans");
}




function showTools() {
	if(tool_bar) {
		$(".tool").css("display", "none");
		tool_bar = 0;
	}
	else {
		$(".tool").css("display", "block");
		tool_bar = 1;
	}
}

function offTool() {
	if(tools != 0) {
		send("3", "Mega2/arm_tool");
		onMessage_offTool();
	}
	$(".tool").css("display", "none");
	tool_bar = 0;
}

function onMessage_offTool() {
	tools = 0;
	$("#tools").css("background-image", "url(../static/img/offtool.png)");
	$("#tool1").css({opacity: 0.3});
	$("#tool2").css({opacity: 0.8});
	$("#tool3").css({opacity: 0.8});
}

function selectTool1() {
	if(tools != 1) {
		send("4", "Mega2/arm_tool");
		onMessage_Tool1();
	}
	else {
		if(saw) {
			send("sawOff", "Mega2/arm_tool");
			$("#tool2").css({opacity: 0.3});
			saw = 0;
		}
		else {
			send("sawOn", "Mega2/arm_tool");
			$("#tool2").css({opacity: 1});
			saw = 1;
		}
	}
	$(".tool").css("display", "none");
	tool_bar = 0;
}

function onMessage_Tool1() {
	tools = 1;
	$("#tools").css("background-image", "url(../static/img/saw.png)");
	$("#tool1").css({opacity: 0.6});
	$("#tool2").css({opacity: 0.3});
	$("#tool3").css({opacity: 0.8});
}

function selectTool2() {
	if(tools != 2) {
		send("5", "Mega2/arm_tool");
		onMessage_Tool2();
	}
	else {
		if(pump) {
			send("pumpOff", "Mega2/arm_tool");
			$("#tool3").css({opacity: 0.3});
			pump = 0;
		}
		else {
			send("pumpOn", "Mega2/arm_tool");
			$("#tool3").css({opacity: 1});
			pump = 1;
		}
	}
	$(".tool").css("display", "none");
	tool_bar = 0;
}

function onMessage_Tool2() {
	tools = 2;
	$("#tools").css("background-image", "url(../static/img/pump.png)");
	$("#tool1").css({opacity: 0.6});
	$("#tool2").css({opacity: 0.8});
	$("#tool3").css({opacity: 0.3});
}



function onMessage_anginfo(val) {
	var valsplit = val.split("#")
	var anginfofonts = document.getElementById('anginfo').getElementsByTagName("font");
	for(var i = 0; i < 4; i ++) {
		anginfofonts[i].innerHTML = valsplit[i];
	}
	
	var marker = document.getElementById("attitudemarker");
	marker.style.marginLeft = 'calc(' + String((180 - Number(valsplit[0]).toFixed(0)) * 0.27) + '%)';
	marker.style.marginTop = 'calc(' + String((180 - Number(valsplit[1]).toFixed(0)) * 0.27) + '%)';
	marker.style.transform = 'rotate(' + String(Number(valsplit[2]).toFixed(0)) +'deg)';
}



$(function () {
	document.addEventListener('mousemove', function(msmove) {
		x = msmove.pageX;
		y = msmove.pageY;
		
		if(downstate) {
			if(perX > Number(((x - iniX) / $(window).width()).toFixed(1))) {
				perX = ((x - iniX) / $(window).width()).toFixed(1);
				send("P2(-10)", "Mega0/camera_trans");
			}
			else if(perX < Number(((x - iniX) / $(window).width()).toFixed(1))) {
				perX = ((x - iniX) / $(window).width()).toFixed(1);
				send("P2(10)", "Mega0/camera_trans");
			}
			else if(perY > Number(((y - iniY) / $(window).height()).toFixed(1))) {
				perY = ((y - iniY) / $(window).height()).toFixed(1);
				send("P1(10)", "Mega0/camera_trans");
			}
			else if(perY < Number(((y - iniY) / $(window).height()).toFixed(1))) {
				perY = ((y - iniY) / $(window).height()).toFixed(1);
				send("P1(-10)", "Mega0/camera_trans");
			}
			console.log("x = " + x + ", y = " + y + ", perX = " + perX + ", perY = " + perY);
		}
	})
	
	$("#container").each(function () {
		$(this).mousedown(function () {
			iniX = x;
			iniY = y;
			perX = 0;
			perY = 0;
			if(imgrecognition) {
				send("#" + (iniX / $(window).width()).toFixed(2) + "#" + (iniY / $(window).height()).toFixed(2), "Mega0/camera_trans");
			}
			else {
				downstate = 1;
			}
		});
	});
	
	$("#container").each(function () {
		$(this).mouseup(function () {
			downstate = 0;
		});
	});
});



function change_depslider() {
	depthvalue = $('#depthslider').val();
	if(depthvalue < 0) {
		depthvalue = parseInt(depthvalue) + 9;
	}
	send("d"+ depthvalue, 'Mega1/propeller');
}

function onMessage_depslider(val) {
	if(val > 4) {
		val = val - 9;
	}
	depthvalue = val;
	$('#depvalue').text(depthvalue);
	$('#depthslider').val(depthvalue);
}

function change_speslider() {
	speedvalue = $('#speedslider').val() / 100;
	
	if(rollover) {
		if(forwardbtn) {
			send("p" + forwardbtn + speedvalue, 'Mega1/propeller');
		}
		else if(backwardbtn) {
			send("P" + backwardbtn + speedvalue, 'Mega1/propeller');
		}
		else if(leftbtn) {
			send("o" + leftbtn + speedvalue, 'Mega1/propeller');
		}
		else if(rightbtn) {
			send("O" + rightbtn + speedvalue, 'Mega1/propeller');
		}
	}
	else {
		if(forwardbtn) {
			send("A" + forwardbtn + speedvalue, 'Mega1/propeller');
		}
		else if(backwardbtn) {
			send("B" + backwardbtn + speedvalue, 'Mega1/propeller');
		}
		else if(leftbtn) {
			send("L" + leftbtn + speedvalue, 'Mega1/propeller');
		}
		else if(rightbtn) {
			send("R" + rightbtn + speedvalue, 'Mega1/propeller');
		}
	}
	
	onMessage_speslider(speedvalue);
}

function onMessage_speslider(val) {
	speedvalue = val;
	$('#spevalue').text(speedvalue * 100);
	$('#speedslider').val(speedvalue * 100);
}



function onclick_forwardbtn() {
	forwardbtn = ~forwardbtn + 2;
	if(rollover == 1) {
		send("p" + forwardbtn + speedvalue, 'Mega1/propeller');
	}
	else {
		send("A" + forwardbtn + speedvalue, 'Mega1/propeller');
	}
}

function onMessage_forwardbtn(val) {
	forwardbtn = val;
	backwardbtn = 0;
	$("#backwardbtn").css("background-image", "url(../static/img/backwardbtn_unclick.png)");
	if(forwardbtn == 1) {
		$("#forwardbtn").css("background-image", "url(../static/img/forwardbtn_click.png)");
	} else {
		$("#forwardbtn").css("background-image", "url(../static/img/forwardbtn_unclick.png)");
	}
}

function onclick_backwardbtn() {
	backwardbtn = ~backwardbtn + 2;
	if(rollover == 1) {
		send("P" + backwardbtn + speedvalue, 'Mega1/propeller');
	}
	else {
		send("B" + backwardbtn + speedvalue, 'Mega1/propeller');
	}
}

function onMessage_backwardbtn(val) {
	backwardbtn = val;
	forwardbtn = 0;
	$("#forwardbtn").css("background-image", "url(../static/img/forwardbtn_unclick.png)");
	if(backwardbtn == 1) {
		$("#backwardbtn").css("background-image", "url(../static/img/backwardbtn_click.png)");
	} else {
		$("#backwardbtn").css("background-image", "url(../static/img/backwardbtn_unclick.png)");
	}
}

function onclick_leftbtn() {
	leftbtn = ~leftbtn + 2;
	if(rollover == 1) {
		send("o" + leftbtn + speedvalue, 'Mega1/propeller');
	}
	else {
		send("L" + leftbtn + speedvalue, 'Mega1/propeller');
	}
}

function onMessage_leftbtn(val) {
	leftbtn = val;
	right = 0;
	$("#rightbtn").css("background-image", "url(../static/img/rightbtn_unclick.png)");
	if(leftbtn == 1) {
		$("#leftbtn").css("background-image", "url(../static/img/leftbtn_click.png)");
	} else {
		$("#leftbtn").css("background-image", "url(../static/img/leftbtn_unclick.png)");
	}
}

function onclick_rightbtn() {
	rightbtn = ~rightbtn + 2;
	if(rollover == 1) {
		send("O" + rightbtn + speedvalue, 'Mega1/propeller');
	}
	else {
		send("R" + rightbtn + speedvalue, 'Mega1/propeller');
	}
}

function onMessage_rightbtn(val) {
	rightbtn = val;
	leftbtn = 0;
	$("#leftbtn").css("background-image", "url(../static/img/leftbtn_unclick.png)");
	if(rightbtn == 1) {
		$("#rightbtn").css("background-image", "url(../static/img/rightbtn_click.png)");
	} else {
		$("#rightbtn").css("background-image", "url(../static/img/rightbtn_unclick.png)");
	}
}



function onclick_Rollover() {
	rollover = ~rollover + 2;
	onMessage_Rollover(rollover);
}

function onMessage_Rollover(val) {
	rollover = val;
	
	if(rollover == 1) {
		$("#rollover").css("background-image","url(../static/img/rollover_click.png)");
	} else {
		$("#rollover").css("background-image","url(../static/img/rollover_unclick.png)");
	}
	
	onMessage_forwardbtn(0);
	onMessage_backwardbtn(0);
	onMessage_leftbtn(0);
	onMessage_rightbtn(0);	
}

function onclick_Reset() {
	send("s;", "Mega1/propeller");
	onMessage_Rollover(0);
}



/*********
---mqtt---
*********/
function getOS() {
	var u = navigator.userAgent;

	if (!!u.match(/compatible/i) || u.match(/Windows/i)) {
		return 'Windows_' + Math.ceil(Math.random()*10000);
	} else if (!!u.match(/Raspbian Chromium/i)) {
		return 'RaspberryPi_' + Math.ceil(Math.random()*10000);
	} else if (!!u.match(/Linux/i)) {
		return 'Linux_' + Math.ceil(Math.random()*10000);
	} else if (!!u.match(/Macintosh/i) || u.match(/MacIntel/i)) {
		return 'MacOS_' + Math.ceil(Math.random()*10000);
	} else if (!!u.match(/iphone/i) || u.match(/Ipad/i)) {
		return 'iOS_' + Math.ceil(Math.random()*10000);
	} else if (!!u.match(/android/i)) {
		return 'Android_' + Math.ceil(Math.random()*10000);
	} else {
		return 'Other_' + Math.ceil(Math.random()*10000);
	}
}


// 自动获取并修改IP
var hostname = '192.168.0.1',
	port = 8083,
	clientId = getOS(),
	timeout = 5,
	keepAlive = 60,
	cleanSession = true,
	ssl = false,
	topic = ["web/switches", "Mega0/camera_trans", "Mega1/propeller", "Mega1/gyroscope", "Mega2/arm_tool"];
	/*************
	---通信协议---
	web/switches
		p0:图像处理关
		p1:dehaze
		p2:defog

		r0:图像识别关
		r1:图像识别开
		
		#: 图像识别位置

	Mega1/propeller:
		深度：min="0" max="4" min="5" max="8" step="1" value="0"
		'd' + val
		速度：min="0" max="4" step="1" value="1"	//min="0" max="400" step="100" value="100"
		'v' + val
		
		方向：min="0" max="4" step="1" value="1"	//min="0" max="400" step="100" value="100"
		'A' + forwardbtn + val
		'B' + backwardbtn + val
		'L' + leftbtn + val
		'R' + rightbtn + val

		仰俯、翻滚：min="0" max="4" step="1" value="1"
		'p' + forwardbtn + val
		'P' + backwardbtn + val
		'o' + leftbtn + val
		'O' + rightbtn + val
		
		复位：
		's'
        
    Mega2/arm_tool:
        0:Arm伸出
        1:Arm收回
        2:Arm抓取

        3:卸工具
        4:装Tool1
        5:装Tool2
		
	Mega0/camera_trans:
		0:trans关，收缩
		1:trans开，展开
		
		2:camera复位
		
		P:云台控制信号
	**************/

var options = {
	timeout: timeout,
	keepAliveInterval: keepAlive,
	cleanSession: cleanSession,
	useSSL: ssl, 
	onSuccess: onConnect,
	onFailure: function (errorMsg) {
		console.log("CONNECTION FAILURE - " + errorMsg.errorMessage);
	}
};

function onConnect() {
	console.log(clientId + " onConnected");
	for(var th = 0; th < topic.length; th ++){
		client.subscribe(topic[th]);
	}
}

function onConnectionLost(responseObject) {
	console.log(responseObject);
	if (responseObject.errorCode !== 0) {
		console.log("onConnectionLost:" + responseObject.errorMessage);
	}
}



function textScroll(val) {
	var fonts = document.getElementById('textarea').getElementsByTagName("font");
	for(var i = 5; i >= 0; i --) {
		fonts[i + 1].innerHTML = fonts[i].innerHTML;
	}
	fonts[0].innerHTML = val;
}

function onMessageArrived(message) {
	msgPayload = message.payloadString + "";
	msgTopic = message.destinationName;
	textScroll(msgTopic + ": " + msgPayload);
	
	console.log(msgPayload);
	
	switch(msgTopic) {
		case "web/switches":
			switch(msgPayload[0]) {
				case 'p':
					onMessage_imgprocess(msgPayload[1]);
					break;
				case 'r':
					onMessage_imgrecognition(msgPayload[1]);
					break;
				default:
					console.log("Message's payload is invalid")
			}
			break;
			
		case 'Mega1/propeller':
			switch(msgPayload[0]) {
				case 'd':
					onMessage_depslider(msgPayload[1]);
					break;
				case 'v':
					onMessage_speslider(msgPayload[1]);
					break;
					
				case 'A':
					onMessage_forwardbtn(msgPayload[1]);
					onMessage_speslider(msgPayload[2]);
					break;
				case 'B':
					onMessage_backwardbtn(msgPayload[1]);
					onMessage_speslider(msgPayload[2]);
					break;
				case 'L':
					onMessage_leftbtn(msgPayload[1]);
					onMessage_speslider(msgPayload[2]);
					break;
				case 'R':
					onMessage_rightbtn(msgPayload[1]);
					onMessage_speslider(msgPayload[2]);
					break;
					
				case 'p':
					onMessage_forwardbtn(msgPayload[1]);
					onMessage_speslider(msgPayload[2]);
					break;
				case 'P':
					onMessage_backwardbtn(msgPayload[1]);
					onMessage_speslider(msgPayload[2]);
					break;
				case 'o':
					onMessage_leftbtn(msgPayload[1]);
					onMessage_speslider(msgPayload[2]);
					break;
				case 'O':
					onMessage_rightbtn(msgPayload[1]);
					onMessage_speslider(msgPayload[2]);
					break;
					
				case 's':
					onMessage_depslider(0);
					onMessage_speslider(0);
					
					onMessage_forwardbtn(0);
					onMessage_backwardbtn(0);
					onMessage_leftbtn(0);
					onMessage_rightbtn(0);
					
					onMessage_Rollover(0);
					break;
				default:
					console.log("Message's payload is invalid")
			}
			break;
			
		case "Mega2/arm_tool":
			switch(msgPayload[0]) {
				case '0':
					onMessage_Armstretch();
					break;
				case '1':
					onMessage_Armretrieve();
					break;
				case '2':
					onMessage_Armgrab();
					break;
				case '3':
					onMessage_offTool();
					break;
				case '4':
					onMessage_Tool1();
					break;
				case '5':
					onMessage_Tool2();
					break;
				default:
					console.log("Message's payload is invalid")
			}
			break;
		
		case "Mega0/camera_trans":
			switch(msgPayload[0]) {
				case '0':
					onMessage_trans(0);
					break;
				case '1':
					onMessage_trans(1);
					break;
				case '2':
					break;
				case 'P':
					break;
				case '#':
					break;
				default:
					console.log("Message's payload is invalid")
			}
			break;
			
		case 'Mega1/gyroscope':
			onMessage_anginfo(msgPayload);
			break;
		default:
			console.log("Message's topic is invalid")
	}
}

function send(msg, Topic) {
	message = new Paho.MQTT.Message(msg);
	message.destinationName = Topic;
	client.send(message);
}


// 让其他设备刚连接时同步状态
client = new Paho.MQTT.Client(hostname, port, clientId);	// 建立客户端实例
client.connect(options);	// 连接服务器并注册连接成功处理事件
client.onConnectionLost = onConnectionLost;	// 注册连接断开处理事件  
client.onMessageArrived = onMessageArrived;	// 注册消息接收处理事件
