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


#leftrotatebutton{
  margin: 5px 40px;
 margin-bottom:15px;
}

#rightrotatebutton{
  margin: 5px 40px;
 margin-bottom:15px;
}




.space{
   margin:auto;
   margin-top:15px;
   width: 50%;
   font-size: 20px;
   background-color: red;
   padding: 10px 20px;
   color: white;
}
.info1{
color: white;
background-color: #008CBA;
width:20%;
position: absolute;
top:15%;
left:5%;
}

.info2{
  color: white;
  background-color: #008CBA;
  width:20%;
  position: absolute;
  top:15%;
  right:5%;
}


</style>


<body style="text-align: center;" >


  <div class="info1" >

    Position:<span id="PositionX"> X </span>, <span id="PositionY"> Y</span><br>
    Orientation:<span id="Orientation"> Theta </span><br>
    Direction:<span id="MotorDirection"> [x,y,rotate] </span><br>
    Motor Speed:<span id="MotorSpeed"> Motor Speed </span><br>

  </div>

<div class="info2">
  ToF reading:<span id="TOFreading"> TOF goes here </span><br>
  Sensor Direction:<span id="ServoSensor"> Theta </span><br>
  Servo Gripper:<span id="Servogripper"> Theta </span><br>
  Autnomous:<span id="Autonomy"> True/False</span><br>
  </div>

    <div class="space">Movement</div><br>
  <button class="button" type="button" onclick="leftrotate()" id="leftrotatebutton"> [Q] Rotate Left </button>
  <button class="button" type="button" onclick="rightrotate()" id="rightrotatebutton">[E] Rotate RIght  </button> <br>

  <button class="button" type="button" onclick="forward()" id="forwardbutton"> [W] Forward </button> <br>
  <button class="button" type="button" onclick="left()" id="leftbutton"> [A] Left </button>
  <button class="button" type="button" onclick="back()" id="backbutton">[S] Back  </button>
  <button class="button" type="button" onclick="right()" id="rightbutton">[D] Right  </button>
  <br>
  <div class="space">Servo and Speed Control </div><br>
  <button class="button" type="button" onclick="motorspeedupbutton" id="motorspeedupbutton">[up] Speed up</button>
  <button class="button" type="button" onclick="turnsensorleft()" id="sensorleft">[<-] sensor left</button>
  <button class="button" type="button" onclick="opengripper()" id="openbutton">[F] open  gripper</button>
  <button class="button" type="button" onclick="closegripper()" id="closebutton">[K] close gripper</button>
  <button class="button" type="button" onclick="turnsensorright()" id="sensorright">[->] sensor right</button>
  <button class="button" type="button" onclick="motorslowdownbutton" id="motorslowdownbutton">[down] Slow down</button>

  <div class="space">Autonomous functions </div> <br>
  <button class="button" type="button" onclick="onWall()" id="onWallbutton">[Y]Get on wall</button>
  <button class="button" type="button" onclick="wallfollow()" id="wallfollowbutton">[U]Wall follow</button>
  <button class="button" type="button" onclick="offwall()" id="offwallbutton">[I]Get off Wall</button>
  <button class="button" type="button" onclick="circletrace()" id="circletracebutton">[P]Circle Trace</button>
    <button class="button" type="button" onclick="scancan()" id="scanbutton">[O]Scan</button><br>

  <button class="button" type="button" onclick="setloc1()" id="setlocation1"> [Z] Set Location 1 </button>
  <button class="button" type="button" onclick="setloc2()" id="setlocation2"> [X] Set Location 2  </button>
  <button class="button" type="button" onclick="goloc1()" id="golocation1">[1] go to location 1 </button>
  <button class="button" type="button" onclick="goloc2()" id="golocation2">[2] go to location 2  </button>
  <button class="button" type="button" onclick="scancan()" id="resethome">[-]reset Home</button>
  <button class="button" type="button" onclick="gohome()" id="homebutton"> [0] Go Home</button>
   <button class="button" type="button" onclick="reset()" id="resetbutton"> [R] RESET</button><br>

<script>

 document.addEventListener('keydown', keyDownHandler, true);
 document.addEventListener('keyup', keyUpHandler, true);
 var reset=0;
 var forwardstate=0;
 var rightstate=0;
 var rotatestate=0;

var motorspeedstate=0;
var Sensorservostate=0;
var Gripperstate=0;



var esp32message=[];
var esp32Status=[];


//autonomous states
var auton=0;
var Onwallstate=0;
 
var keyboardCode = function(event) {
  var code;
  if (event.repeat) return 0; // ignore repeating if held down
  if (event.key !== undefined) { code = event.key; } 
  else if (event.keyCode !== undefined) {code = event.keyCode;}
  return code;
};


setInterval(updateState, 200); 
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
      Sensorservostate= -1;
      document.getElementById("sensorleft").style = "background-color:lime";
    }

     if(code == 39 || code == "ArrowRight") { // arrow right key 
      Sensorservostate= 1;
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

     if(code == 89 || code == "y") { // y key 
      Onwallstate= 1;
      auton=1;
      document.getElementById("onWallbutton").style = "background-color:lime";
    }
 }

  if(code == 82 || code == "r") { // k key 
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

    Onwallstate=0;
    document.getElementById("onWallbutton").style = "background-color:#008CBA";
  }
    updateState();
  
  }
//sends commands to esp32
function sendState() {
    var xhttp = new XMLHttpRequest(); 
    //sets the url that we use to get the attach handler
    var str="Orders?val=";
    var res=str.concat(reset,",",auton,",",forwardstate,",",rightstate,",",rotatestate,",",motorspeedstate,",",Sensorservostate,",",Gripperstate);
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
        esp32message=this.responseText.split(",");
        //document.getElementById("MotorDirection").innerHTML = esp32satus[0];
        
        }    
    };
    xhttp.open("GET", res, true);
    xhttp.send();
  }





</script>


</html>





)===";
