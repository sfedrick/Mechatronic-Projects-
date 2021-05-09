//html
const char body[] PROGMEM = R"===( 


<!DOCTYPE html>
<html>

<style>
.button {
  border: none;
  color: white;
  background-color: #008CBA;
  padding: 15px 32px;
  text-decoration: none;
  font-size: 16px;
  margin: 4px 2px;
  cursor: pointer;
}
.buttonBot{
    position: relative;
 right: 130px;
  border: none;
  color: white;
  background-color: #008CBA;
  padding: 15px 32px;
  text-decoration: none;
  font-size: 16px;
  margin: 4px 2px;
  cursor: pointer;
}

.buttonAuton {
  border: none;
  color: white;
  background-color: #008CBA;
  padding: 15px 32px;
  text-decoration: none;
  font-size: 16px;
  margin: 4px 2px;
  cursor: pointer;
}

#leftrotatebutton{
  margin: 5px 40px;
 margin-bottom:15px;
}

#rightrotatebutton{
  margin: 5px 40px;
 margin-bottom:15px;
}



p{
text-align: center;
   font-size: 20px;
   padding=0px 0px;
}
.space{

  position: relative;
   left: 300px;
     text-align: center;
   margin:auto;
   margin-top:15px;
   width: 20%;
   font-size: 20px;
   background-color: red;
   padding: 0.05px 20px;
   color: white;

}

.space2{

   margin:auto;
   margin-top:15px;
   width: 15%;
   font-size: 20px;
   background-color: red;
   padding: 15px 5px;
   color: white;

}
.info1{
  text-align: left;
     font-size: 12px;
color: white;
background-color: #008CBA;
width:10%;
position: absolute;
top:2%;
right:2.5%;

}

.info2{
    text-align: left;
   font-size: 12px;
  color: white;
  background-color: #008CBA;
  width:10%;
  position: absolute;
  top:2.5%;
  right:40%;
}


</style>
<head>  <title>Ranging Display</title>
<style>
  article, footer { float: left; border-radius: 15px;  padding: 10px;  width:100%; background-color: #f1f1f1;}
  footer { background-color: #ddd; }
  section { width: 510px;}
</style>
</head>

<body>
<section>
<article>
  <canvas id="myCanvas" width="500" height="400" style="border:3px solid #d3d3d3;">
  Your browser does not support the HTML5 canvas tag.
  </canvas> <br>
  <span style="font-size: 25px;"> Ranging </span>  &nbsp;

  <button type="button" onclick="zoomf()"> &nbsp; toggle zoom &nbsp; </button> <br>
</article>
</section>

<body style="text-align: right;" >


  <div class="info1" >

    Position:<span id="PositionX"> X </span>, <span id="PositionY"> Y</span><br>
    Orientation:<span id="Orientation"> Theta </span><br>
    Direction:<span id="MotorDirection"> [x,y,rotate] </span><br>
    Motor Speed:<span id="MotorSpeed"> Motor Speed </span><br>
  </div>

<div class="info2">
  ToF reading:<span id="TOFreading"> TOF</span><br>
  Sensor Direction:<span id="ServoSensor"> Theta </span><br>
  Servo Gripper:<span id="Servogripper"> Theta </span><br>
  Autonomous:<span id="Autonomy"> True/False</span><br>
  AI Message:<span id="AutonomyMessage"> True/False</span><br>
  </div>

    <div class="space"><p>Movement</p></div><br>
  <button class="button" type="button" onclick="leftrotate()" id="leftrotatebutton"> [Q] Rotate Left </button>


  <button class="button" type="button" onclick="forward()" id="forwardbutton"> [W] Forward </button>
    <button class="button" type="button" onclick="rightrotate()" id="rightrotatebutton">[E] Rotate RIght  </button><br>
  <button class="buttonBot" type="button" onclick="left()" id="leftbutton"> [A] Left </button>
  <button class="buttonBot" type="button" onclick="back()" id="backbutton">[S] Back  </button>
  <button class="buttonBot" type="button" onclick="right()" id="rightbutton">[D] Right  </button>
  <br>
  <div class="space"><p>Servo and Speed Control</p> </div><br>
  <button class="button" type="button" onclick="motorspeedupbutton" id="motorspeedupbutton">[up] Speed up</button>
  <button class="button" type="button" onclick="turnsensorleft()" id="sensorleft">[<-] sensor left</button>
  <button class="button" type="button" onclick="opengripper()" id="openbutton">[F] open  gripper</button>
  <button class="button" type="button" onclick="closegripper()" id="closebutton">[K] close gripper</button>
  <button class="buttonBot" type="button" onclick="turnsensorright()" id="sensorright">[->] sensor right</button>
  <button class="buttonBot" type="button" onclick="motorslowdownbutton" id="motorslowdownbutton">[down] Slow down</button>

  <div class="space2">Autonomous functions </div> <br>
  <div class="Auton Buttons" style="text-align: center;" >
  <button class="button" type="button" onclick="scancan()" id="scanbutton">[O]Scan</button>
   <button class="button" type="button" onclick="ccwturn()" id="ccwturnbutton">[I]-90 degree turn</button>
    <button class="button" type="button" onclick="caref" id="carefulfoward">[9]Foward careful</button>
  <button class="button" type="button" onclick="cwturn()" id="cwturnbutton">[P]+90 degree turn</button><br>
  <button class="button" type="button" onclick="onWall()" id="onWallbutton">[Y]Get on wall</button> 
  <button class="button" type="button" onclick="initwall()" id="initWallbutton">[H] Init wall</button>  
  <button class="button" type="button" onclick="resethome()" id="resethomebutton">[-]reset Home</button>
   <button class="button" type="button" onclick="reset()" id="resetbutton"> [R] RESET</button><br>
   </div>

<script>

 document.addEventListener('keydown', keyDownHandler, true);
 document.addEventListener('keyup', keyUpHandler, true);
 var reset=0;
 var resetHome=0;
 var forwardstate=0;
 var rightstate=0;
 var rotatestate=0;

var motorspeedstate=0;
var Sensorservostate=0;
var Gripperstate=0;


//autonomous states
var auton=0;
var Onwallstate=0;
var Scanstate=0;
var ScanInterval;
var CWturnState=0;
var carefulForwardState=0;
var esp32message=[];
var esp32Status=[];
var initWallState=0;

sendState();



var keyboardCode = function(event) {
  var code;
  if (event.repeat) return 0; // ignore repeating if held down
  if (event.key !== undefined) { code = event.key; }
  else if (event.keyCode !== undefined) {code = event.keyCode;}
  return code;
};


setInterval(updateState, 500);
function updateState(){
  sendState();
  checkState();
}






function keyDownHandler(event) {
 var code = keyboardCode(event);

 if(auton===0){
    if(code == 87 || code == 'w') { // w key
      forwardstate = 1;
      document.getElementById("forwardbutton").style = "background-color:lime";
    }
    if(code == 83 || code == 's') { // s key
      forwardstate = -1;
      document.getElementById("backbutton").style = "background-color:lime";
    }
    if(code == 87 || code == 'd') { // d key
      rightstate = 1;
      document.getElementById("rightbutton").style = "background-color:lime";
    }
    if(code == 65 || code == 'a') { // a key
      rightstate = -1;
      document.getElementById("leftbutton").style = "background-color:lime";
    }

    if(code == 69 || code == 'e') { // e key
      rotatestate = 1;
      document.getElementById("rightrotatebutton").style = "background-color:lime";
    }
    if(code == 81 || code == 'q') { // q  key
      rotatestate = -1;
      document.getElementById("leftrotatebutton").style = "background-color:lime";
    }
//servor and speed controls

     if(code == 38 || code == "ArrowUp") { // arrow up key
      motorspeedstate = 1;
      document.getElementById("motorspeedupbutton").style = "background-color:lime";
    }

     if(code == 40 || code == "ArrowDown") { // arrow down key
      motorspeedstate = -1;
      document.getElementById("motorslowdownbutton").style = "background-color:lime";
    }

     if(code == 37 || code == "ArrowLeft") { // arrow left key
      Sensorservostate= 1;
      document.getElementById("sensorleft").style = "background-color:lime";
    }

     if(code == 39 || code == "ArrowRight") { // arrow right key
      Sensorservostate= -1;
      document.getElementById("sensorright").style = "background-color:lime";
    }

   if(code == 70 || code == "f") { // f key
      Gripperstate= 1;
      document.getElementById("openbutton").style = "background-color:lime";
    }

     if(code == 75 || code == "k") { // k key
      Gripperstate= -1;
      document.getElementById("closebutton").style = "background-color:lime";
    }

     if(code == 72 || code == "h") { // k key
      if(initWallState==0){
      initWallState= 1;
      document.getElementById("initWallbutton").style = "background-color:lime";
      }
      else{
        initWallState=0;
        document.getElementById("initWallbutton").style = "background-color:#008CBA";
      }

      
    }

    //autonomous functions

     if(code == 89 || code == "y") { // y key
      Onwallstate= 1;
      auton=1;
      document.getElementById("onWallbutton").style = "background-color:lime";
    }


     if(code == 79 || code == "o") { // o key
      if(Scanstate==0){
         Scanstate= 1;
         ScanInterval=setInterval(getData, 1000);
         document.getElementById("scanbutton").style = "background-color:lime";
      }
     else if(Scanstate==1){
       clearInterval(ScanInterval);
      Scanstate= 0;
       document.getElementById("scanbutton").style = "background-color:#008CBA";
      
     }
     }
     
      if(code == 80 || code == "p") { // p key
      CWturnState=1;
      auton=1;
      document.getElementById("cwturnbutton").style = "background-color:lime";
    }
     if(code == 73 || code == "i") { // i key
      CWturnState=-1;
      auton=1;
      document.getElementById("ccwturnbutton").style = "background-color:lime";
    }

/*

  <button class="button" type="button" onclick="GrabCan" id="GrabCanButton">[H]GrabCan</button>  

*/
if(code == 57 || code == "9") { // p key
      carefulForwardState=1;
      auton=1;
      document.getElementById("carefulfoward").style = "background-color:lime";
    }
        
    

    
 }
//reset keys hard reset power off esp32
    if(code == 189 || code == "-") { // k key
      resetHome= 1;
      document.getElementById("resethomebutton").style = "background-color:red";

    }
//reset keys soft reset reload website
  if(code == 82 || code == "r") { // r key
      reset= 1;
      document.getElementById("resetbutton").style = "background-color:red";
    }
    updateState();
}

function keyUpHandler(event) {
  var code =keyboardCode(event);

  if(auton===0){

  if(code == 87 || code == 'w') { // w key
      forwardstate = 0;
      document.getElementById("forwardbutton").style = "background-color:#008CBA";
    }
   if(code == 83 || code == 's') { // s key
      forwardstate = 0;
      document.getElementById("backbutton").style = "background-color:#008CBA";
    }
    if(code == 87 || code == 'd') { // d key
      rightstate = 0;
      document.getElementById("rightbutton").style = "background-color:#008CBA";
    }
    if(code == 65 || code == 'a') { // a key
      rightstate = 0;
      document.getElementById("leftbutton").style = "background-color:#008CBA";
    }

    if(code == 69 || code == 'e') { // e key
      rotatestate = 0;
      document.getElementById("rightrotatebutton").style = "background-color:#008CBA";
    }
    if(code == 81 || code == 'q') { // q  key
      rotatestate = 0;
      document.getElementById("leftrotatebutton").style = "background-color:#008CBA";
    }


    //servor and speed controls

     if(code == 38 || code == "ArrowUp") { // arrow up key
      motorspeedstate = 0;
      document.getElementById("motorspeedupbutton").style = "background-color:#008CBA";
    }

     if(code == 40 || code == "ArrowDown") { // arrow down key
      motorspeedstate = 0;
      document.getElementById("motorslowdownbutton").style = "background-color:#008CBA";
    }
     if(code == 37 || code == "ArrowLeft") { // arrow left key
      Sensorservostate= 0;
      document.getElementById("sensorleft").style = "background-color:#008CBA";
    }

     if(code == 39 || code == "ArrowRight") { // arrow right key
      Sensorservostate=0;
      document.getElementById("sensorright").style = "background-color:#008CBA";
    }

   if(code == 70 || code == "f") { // f key
      Gripperstate= 0;
      document.getElementById("openbutton").style = "background-color:#008CBA";
    }

     if(code == 75 || code == "k") { // k key
      Gripperstate= 0;
      document.getElementById("closebutton").style = "background-color:#008CBA";
    }


  }

    if(code == 189 || code == "-") { // k key
      resetHome= 1;
      document.getElementById("resethomebutton").style = "background-color:orange";

    }
    if(code == 82 || code == "r") { // k key
      reset= 1;
      document.getElementById("resetbutton").style = "background-color:orange";

    }
    
    updateState();

  }
//sends commands to esp32
function sendState() {
    var xhttp = new XMLHttpRequest();
    //sets the url that we use to get the attach handler
    var str="Orders?val=";
    var res=str.concat(resetHome,",",reset,",",auton,",",forwardstate,",",rightstate,",",rotatestate,",",motorspeedstate,",",Sensorservostate,",",Gripperstate,",",Onwallstate,",",Scanstate,",",CWturnState,",",carefulForwardState,",",initWallState);
    xhttp.onreadystatechange = function() {
        if (this.status == 200 && this.readyState == 4) {
  //whatever you send as plain text or html in the function attached to the Orders?val=  attach handler gets displayed here
  //we split that plain text using commas
        esp32message=this.responseText.split(",");
        document.getElementById("MotorDirection").innerHTML = esp32message[0];
        document.getElementById("MotorSpeed").innerHTML = esp32message[1];
        document.getElementById("ServoSensor").innerHTML = esp32message[2];
        document.getElementById("Servogripper").innerHTML = esp32message[3];

        }
    };
    xhttp.open("GET", res, true);
    xhttp.send();
  }

function checkState() {
    var xhttp = new XMLHttpRequest();
    //sets the url that we use to get the attach handler
    var str="Check?val=";
    var res=str.concat(reset);
    xhttp.onreadystatechange = function() {
        if (this.status == 200 && this.readyState == 4) {
  //whatever you send as plain text or html in the function attached to the Check?val=  attach handler gets displayed here
  //we split that plain text using commas
        esp32Status=this.responseText.split(",");
        reset = parseInt(esp32Status[0]);
        if(reset===1){
          location.reload();
        }
        auton= parseInt(esp32Status[1]);

        document.getElementById("Autonomy").innerHTML = esp32Status[2];
        document.getElementById("PositionX").innerHTML = esp32Status[3];
        document.getElementById("PositionY").innerHTML = esp32Status[4];
        document.getElementById("Orientation").innerHTML = esp32Status[5];
        document.getElementById("TOFreading").innerHTML = esp32Status[6];

        }
    };
    xhttp.open("GET", res, true);
    xhttp.send();
  }


//Scan map javascript from Mark Yim
// Display Elements
var c = document.getElementById("myCanvas");
var ctx = c.getContext("2d");
ctx.font="14px Arial";

var ch = [];
var scansize=45;
var zoom = 8;



function zoomf() {
  if (zoom == 8) zoom = 2;
  else zoom = 8;
  getData();
}

function drawScreen() {
  ctx.clearRect(0, 0, c.width, c.height);
  ctx.strokeStyle = "#008800";
  ctx.beginPath();
  ctx.setLineDash([1, 5]);

  // draw distance lines
  for (let i=1; i < 8; i++) {
    ctx.moveTo(0, i * 50);
    ctx.lineTo(c.width, i * 50);  
    ctx.fillText(String((8-i)*50*zoom)+" mm",20,i*50);
  }
  ctx.stroke();
  
  // draw origin (robot as circle)
  ctx.beginPath();
  ctx.arc(c.width/2, c.height-2, 30/zoom, 0, 2 * Math.PI, false);
  ctx.fill();
  ctx.stroke();
}

function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200 ) { 
      updateGraph(this);
    }
  };
  xhttp.open("GET", "up", true);
  xhttp.send();
}

function radius(i){return ch[i+1]; }

function theta(i){ return ch[i+1+scansize]*Math.PI/180; }

function x(i){     return radius(i)*Math.cos(theta(i))/zoom; }

function y(i){     return radius(i)*Math.sin(theta(i))/zoom; }

function drawDataCircles() {
  ctx.setLineDash([]);
  for (let i=0; i < scansize; i++) {
    ctx.beginPath();
    ctx.arc(c.width/2 + y(i), c.height - x(i),
            radius(i)/80, 0, 2*Math.PI);
    ctx.stroke();
  }
}

function updateGraph(xhttp) {
      scansize = parseInt(xhttp.responseText); // get 1st value
      drawScreen(); 
      
      // draw old data in light green
      ctx.strokeStyle = "#88FF88"; 
      drawDataCircles();
      
      // draw new data in dark green
      ch = xhttp.responseText.split(","); // get ranging data
      ctx.strokeStyle = "#008800";
      drawDataCircles();
//  debug.innerHTML = ch; // debugging print
}










</script>

</body>
</html>
)===";
