/**************************************************************
   WebPage html file
   2020.05.06 modified by @tomorrow56
 **************************************************************/

#ifndef WebPage_h
#define WebPage_h

const String HTTP_Page[] = {
// Display the HTML web page
"<!DOCTYPE html><html><head>",
"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">",
"<title>ESP32 Web controller</title>",
//"<link rel=\"icon\" href=\"data:,\">",
// CSS to style the on/off buttons
// Feel free to change the background-color and font-size attributes to fit your preferences
"<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}",
"  #ledPosR{color: red;}",
"  #ledPosG{color: green;}",
"  #ledPosB{color: blue;}",
"  input.button2{margin:8px;width:80px;height:60px;color:#ffffff;background-color:#2f4f4f;border-color:#fdf5e6 #fdf5e6 #ffdab9 #ffdab9;}"
"  input.button3{margin:8px;width:80px;height:60px;color:#ffffff;background-color:#4f2f2f;border-color:#fdf5e6 #fdf5e6 #ffdab9 #ffdab9;}"
"  input.slid{width: 300px;}",
"</style>",
// Web Page
"</head>",
"<body>",
"<h1>ESP32 Web controller</h1>",
"<h2>Motor Control</h2>",
//Motor Control button
"<form method='get'>",
"  <input class='button3' type='button' name='MOTOR_L' value='LEFT' onclick='motorL()'>",
"  &nbsp;",
"  <input class='button2' type='button' name='MOTOR_F' value='FWD' onclick='motorF()'>",
"  &nbsp;",
"  <input class='button3' type='button' name='MOTOR_R' value='RIGHT' onclick='motorR()'><br>",
"</form>",
"<form method='get'>",
"  <input class='button2' type='button' name='MOTOR_TL' value='TURN_L' onclick='motorTL()'>",
"  &nbsp;",
"  <input class='button2' type='button' name='STOP' value='STOP' onclick='motorS()'>",
"  &nbsp;",
"  <input class='button2' type='button' name='MOTOR_TR' value='TURN_R' onclick='motorTR()'><br>",
"</form>",
"<form method='get'>",
"  <input class='button3' type='button' name='MOTOR_SL' value='SPIN_L' onclick='motorSL()'>",
"  &nbsp;",
"  <input class='button2' type='button' name='MOTOR_B' value='BACK' onclick='motorB()'>",
"  &nbsp;",
"  <input class='button3' type='button' name='MOTOR_SR' value='SPIN_R' onclick='motorSR()'><br>",
"</form>",
"<br>",

"<h2>Servo Control</h2>",
//ServoL slide bar
"<p>Servo L(0-180deg)</p>",
"<form name='slidform' method='get'>",
"  <input class='slid' type='range' name='servoSlidL' value='servoValueL' min='0' max='180' step='1' onchange='setvalSL(this.value)' oninput='setvalSL(this.value)'>",  // Servo angle is valiable
"</form>",
//ServoR slide bar
"<p>Servo R(0-180deg)</p>",
"<form name='slidform' method='get'>",
"  <input class='slid' type='range' name='servoSlidR' value='servoValueR' min='0' max='180' step='1' onchange='setvalSR(this.value)' oninput='setvalSR(this.value)'>",  // Servo angle is valiable
"</form>",
"<br>",
"<form method='get'>",
//"  <input class='button' type='submit' name='servoRESET' value='RESET'><br>",
"  <input type='button' name='servoRESET' value='RESET' onclick='srvRST()'><br>",
"</form>",
"<br>",

"<h2>LED Control</h2>",
//LED_R slide bar
"<p>LED R(0-255)</p>",
"<form name='slidform' method='get'>",
"  <input class='slid' type='range' name='ledSlidR' value='ledValueR' min='0' max='255' step='1' onchange='setvalLR(this.value)' oninput='setvalLR(this.value)'>",  // LED brightness is valiable
"</form>",
//LED_G slide bar
"<p>LED G(0-255)</p>",
"<form name='slidform' method='get'>",
"  <input class='slid' type='range' name='ledSlidG' value='ledValueG' min='0' max='255' step='1' onchange='setvalLG(this.value)' oninput='setvalLG(this.value)'>",  // LED brightness is valiable
"</form>",
//LED_B slide bar
"<p>LED B(0-255)</p>",
"<form name='slidform' method='get'>",
"  <input class='slid' type='range' name='ledSlidB' value='ledValueB' min='0' max='255' step='1' onchange='setvalLB(this.value)' oninput='setvalLB(this.value)'>",  // LED brightness is valiable
"</form>",
"<br>",
"<form method='get'>",
//"  <input class='button' type='submit' name='ledRESET' value='OFF'><br>",
"  <input type='button' name='ledRESET' value='OFF' onclick='ledRST()'><br>",
"</form>",


"<script>",
//reload
"function doReloadNoCache(){",
"    window.location.reload(true);",
"}",

//send Motor Foward
"function motorF(){",
"    var xhr = new XMLHttpRequest();",
"    xhr.open('get', '?MOTOR=FWD&');",
"    xhr.timeout = 1000 ;",
"    xhr.send();",
"}",
//send Motor STOP
"function motorS(){",
"    var xhr = new XMLHttpRequest();",
"    xhr.open('get', '?MOTOR=STOP&');",
"    xhr.timeout = 1000 ;",
"    xhr.send();",
"}",
//send Motor BACK
"function motorB(){",
"    var xhr = new XMLHttpRequest();",
"    xhr.open('get', '?MOTOR=BACK&');",
"    xhr.timeout = 1000 ;",
"    xhr.send();",
"}",
//send Motor LEFT
"function motorL(){",
"    var xhr = new XMLHttpRequest();",
"    xhr.open('get', '?MOTOR=LEFT&');",
"    xhr.timeout = 1000 ;",
"    xhr.send();",
"}",
//send Motor TURN LEFT
"function motorTL(){",
"    var xhr = new XMLHttpRequest();",
"    xhr.open('get', '?MOTOR=TURN_L&');",
"    xhr.timeout = 1000 ;",
"    xhr.send();",
"}",
//send Motor SPINLEFT
"function motorSL(){",
"    var xhr = new XMLHttpRequest();",
"    xhr.open('get', '?MOTOR=SPIN_L&');",
"    xhr.timeout = 1000 ;",
"    xhr.send();",
"}",
//send Motor RIGHT
"function motorR(){",
"    var xhr = new XMLHttpRequest();",
"    xhr.open('get', '?MOTOR=RIGHT&');",
"    xhr.timeout = 1000 ;",
"    xhr.send();",
"}",
//send Motor TURN RIGHT
"function motorTR(){",
"    var xhr = new XMLHttpRequest();",
"    xhr.open('get', '?MOTOR=TURN_R&');",
"    xhr.timeout = 1000 ;",
"    xhr.send();",
"}",
//send Motor SPINRIGHT
"function motorSR(){",
"    var xhr = new XMLHttpRequest();",
"    xhr.open('get', '?MOTOR=SPIN_R&');",
"    xhr.timeout = 1000 ;",
"    xhr.send();",
"}",

//send servoL value
"function setvalSL(thisval){",
"    var xhr = new XMLHttpRequest();",
"    xhr.open('get', '?servoSlidL=' +thisval+ '&');",
"    xhr.timeout = 1000 ;",
"    xhr.send();",
"}",
//send servoR value
"function setvalSR(thisval){",
"    var xhr = new XMLHttpRequest();",
"    xhr.open('get', '?servoSlidR=' +thisval+ '&');",
"    xhr.timeout = 1000 ;",
"    xhr.send();",
"}",
//send SERVO_RESET
"function srvRST(){",
"    var xhr = new XMLHttpRequest();",
"    xhr.open('get', '?servoRESET=RESET&');",
"    xhr.timeout = 1000 ;",
"    xhr.send();",
"    setTimeout(doReloadNoCache, 200);",
"}",

//send LEDR value
"function setvalLR(thisval){",
"    var xhr = new XMLHttpRequest();",
"    xhr.open('get', '?valueR=' +thisval+ '&');",
"    xhr.timeout = 1000 ;",
"    xhr.send();",
"}",
//send LEDG value
"function setvalLG(thisval){",
"    var xhr = new XMLHttpRequest();",
"    xhr.open('get', '?valueG=' +thisval+ '&');",
"    xhr.timeout = 1000 ;",
"    xhr.send();",
"}",
//send LEDB value
"function setvalLB(thisval){",
"    var xhr = new XMLHttpRequest();",
"    xhr.open('get', '?valueB=' +thisval+ '&');",
"    xhr.timeout = 1000 ;",
"    xhr.send();",
"}",
//send LED_RESET
"function ledRST(){",
"    var xhr = new XMLHttpRequest();",
"    xhr.open('get', '?ledRESET=OFF&');",
"    xhr.timeout = 1000 ;",
"    xhr.send();",
"    setTimeout(doReloadNoCache, 200);",
"}",
"</script>",
"</body>",
"</html>"
};
#endif
