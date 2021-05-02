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

.info2{

}
</style>


<body style="text-align: center;" >


  <div class="info1" >

    Position:<span id="Position x"> X </span>, <span id="Position y"> Y</span><br>
    Orientation:<span id="Orientation"> Theta </span><br>
    Direction:<span id="MotorDirection">  Vector </span><br>
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
  <div class="space">Servo and Speed control Control </div><br>
  <button class="button" type="button" onclick="motorspeedupbutton" id="motorspeedupbutton">[up] Speed up</button>
  <button class="button" type="button" onclick="turnsensorleft()" id="sensorleft">[<-] sensor left</button>
  <button class="button" type="button" onclick="opengripper()" id="openbutton">[F] open  gripper</button>
  <button class="button" type="button" onclick="closegripper()" id="closebutton">[K] close gripper</button>
  <button class="button" type="button" onclick="turnsensorright()" id="openbutton">[->] sensor right</button>
  <button class="button" type="button" onclick="motorslowdownbutton" id="motorslowdownbutton">[down] Slow down</button>

  <div class="space">Autonomous functions </div> <br>
  <button class="button" type="button" onclick="onWall()" id="onWallbutton">[Y]Get on wall</button>
  <button class="button" type="button" onclick="wallfollow()" id="wallfollowbutton">[U]Wall follow</button>
  <button class="button" type="button" onclick="offwall()" id="offwallbutton">[I]Get off Wall</button>
  <button class="button" type="button" onclick="circletrace()" id="circletracebutton">[P]Circle Trace</button>
  <button class="button" type="button" onclick="circledown()" id="radiusup"> [N] Reduce circle</button>
  <button class="button" type="button" onclick="circleup()" id="radiusdown"> [M] Increase circle</button>
    <button class="button" type="button" onclick="scancan()" id="scanbutton">[O]Scan</button><br>

  <button class="button" type="button" onclick="setloc1()" id="setlocation1"> [Z] Set Location 1 </button>
  <button class="button" type="button" onclick="setloc2()" id="setlocation2"> [X] Set Location 2  </button>
  <button class="button" type="button" onclick="goloc1()" id="golocation1">[1] go to location 1 </button>
  <button class="button" type="button" onclick="goloc2()" id="golocation2">[2] go to location 2  </button>
  <button class="button" type="button" onclick="scancan()" id="resethome">[-]reset Home</button>
  <button class="button" type="button" onclick="gohome()" id="homebutton"> [0] Go Home</button><br>

<script>

</script>


</html>





)===";
