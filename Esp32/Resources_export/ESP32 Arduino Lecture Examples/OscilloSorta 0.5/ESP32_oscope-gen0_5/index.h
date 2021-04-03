const char body[] PROGMEM = R"===( 
<!DOCTYPE html>
<html>
<head>
<style> 
  h3 { margin: 0px;}
  hr { border: none; height: 2px; background-color: #eee; }
  th, td { border: 1px solid gray;border-radius: 9px;  background-color: #eee; text-align: center;}
  nav { float: left; border-radius: 15px; width: 20%;  background: #ccc; padding: 10px; text-align: center;}
  article { float: left; border-radius: 15px;  padding: 10px;  width: 70%; background-color: #f1f1f1;}
  footer { float: left; border-radius: 25px; padding: 10px;  width: auto;  background: #73AD21;  text-align: center;}
  section {  width: 730px;}
  @media (max-width: 700px) {
    nav, article {   width: 100%;  height: auto;
    }
  }
</style> 
</head>

<body>
<section>
<article>
  <canvas id="myCanvas" width="500" height="400" style="border:3px solid #d3d3d3;">
  Your browser does not support the HTML5 canvas tag.
  </canvas> <br>
  <p.a style="font-size: 25px;"> OscilloSorta</p.a> v0.5 &emsp;
    Display info:   <span id='frame'> </span> <br>
&emsp;  &emsp; Resolution <input type="range" min="20" max="500" value="250" id="resslider">
  <span id='resolution'> 250 </span> data/sweep &emsp; &emsp;
    <button type="button" onclick="autoset()"> AUTOSET </button> 
</article>
<nav>
<h3 > Horizontal </h3> 
<table style="width:100%; background=red;">
  <tr>
    <th style="border-radius: 10px;   background-color: #555; color: lightgreen ; ">  <span id='speed'> 5000 </span><br> uS/div </th>
    <td>
    <button type="button" onclick="speedup()"> &nbsp; + &nbsp;  </button> <br>
    <button type="button" onclick="speeddown()"> &nbsp; -- &nbsp;  </button> <br>
    </td>
  </tr>
  <tr> <th colspan="2"> 
    <input type="range" min="0" max="500" value="125"  id="hoffslider"> </th>
  </tr>
</table>

<!--<div style="background-color: darkorange; border-radius: 15px;">-->
<hr>
<h3 id="vert"> Vertical </h3>
<table style="width:100%">
  <tr>
    <th style="border-radius: 10px;   background-color: #555; color: lightgreen ; "> <span id='volt'> 1000 </span><br>mV/div </th>
    <td><button type="button" onclick="voltup()"> &nbsp; + &nbsp; </button><br>
     <button type="button" onclick="voltdown()"> &nbsp; -- &nbsp; </button><br>
    </td>
  </tr><tr>
    <th colspan="2"> <input type="range" min="1" max="400" value="200"  id="gndslider"></th> 
  </tr><tr>
    <th colspan="2"> <label> <input id="ACCB" type="checkbox" > AC Coupling </label></th>
  </tr>
</table>

<hr>
<h3 id = "trig"> Trigger </h3> 
<table style="width:100%">
  <tr>
    <th> <input type="range" min="1" max="400" value="200"  id="triggerslider"></th>
  </tr><tr>
    <th> <label><input id="edgeCB" type="checkbox" > Edge </label>
    <span id='edgeState'> Rising </span> </th>
  </tr>
</table> 

<hr>
  <button type="button" style="background-color:pink;" id='runB' onclick="runmode()"> Off </button> 
&emsp;
  <button type="button" style="background-color:pink;" id='digiB' onclick="toggleDigiMode()"> Logic </button> <br>
  
<h3 id="chan"> Select </h3> 
  <input type="radio" id="ch1" name="channel" onclick="ch1RB()">
  <label for="ch1">Channel 1 (VP)</label><br>
  <input type="radio" id="ch2" name="channel" onclick="ch2RB()">
  <label for="ch2">Channel 2 (VN)</label><br>
  
</nav>

<footer>
<h1 style="margin:1px;"> Function Generator</h1>
output on GPIO25
<table style="width:100%; border: 1px solid gray; border-radius: 9px; background-color: #ccc;">
  <tr>
    <td style="font-size: 35px; width:150px; border-radius: 10px;   background-color: #555; color: lightgreen ; "> 
     <span id='freq' > 0 </span>Hz</td>
    <td><label> <input id="x1RB" type="radio" name="xFreq"><br>  x1 </label> </td>
    <td><label> <input id="x10RB" type="radio" name="xFreq"><br>  x10 </label></td>
    <td><label> <input id="x100RB" type="radio"  name="xFreq"><br> x100 </label></td>
    <td><label> <input id="x1kRB" type="radio"  name="xFreq"><br>  x1k </label></td>
    <td><label> <input id="x10kRB" type="radio"  name="xFreq"><br>  x10k </label></td>
    <td style="border: none; background-color: #ccc;"> &nbsp; </td>
    <td><label> <input id="squareRB" type="radio"  name="waveform"><br>  square </label><br></td>
    <td><label> <input id="sineRB" type="radio"  name="waveform"><br>  sine </label><br></td>
    <td><label> <br> saw </label><br></td>
 </tr>
 <tr><td style="border: none; background-color: #ccc;">&nbsp;<br></td> </tr>
 <tr><td > Frequency <br><input type="range" min="2" max="100" value="50"  id="FGfreqslider"></td>
   <td style="border: none;" colspan="3">  <span id="duty"> Duty</span> <br>  <input type="range" min="0" max="100" value="50" id="dutyslider"></td>  
   <td  colspan="3">  <span id="FGoffset"> Offset </span> <br>  <input type="range" min="-127" max="127" value="0"  id="FGoffsetslider"></td>
   <td  colspan="3">  <span id="FGscale"> Amplitude </span> <br>  <input type="range" min="0" max="3" value="1"  id="FGscaleslider"> </td>
 </tr>
</table> 
&emsp;  &emsp; Sine Precision <input type="range" min="1" max="8" value="8" id="FGprecisionslider">
  
</footer>
</section>

<script>
// OScope Elements
var c = document.getElementById("myCanvas");
var ctx = c.getContext("2d");
var triggerslider = document.getElementById("triggerslider");
var gndslider = document.getElementById("gndslider");
var resslider = document.getElementById("resslider");
var resolution = document.getElementById("resolution");
var hoffslider = document.getElementById("hoffslider");
var frame = document.getElementById("frame");
var ACCB = document.getElementById("ACCB");
var edgeCB = document.getElementById("edgeCB");
var digiB = document.getElementById("digiB");
var runB = document.getElementById("runB");
var trig = document.getElementById("trig");
var vert = document.getElementById("vert");
var ch1 = document.getElementById("ch1");
// Function Generator Elements
var FGfreqslider = document.getElementById("FGfreqslider");
var dutyslider = document.getElementById("dutyslider");
var duty = document.getElementById("duty");
var FGoffsetslider = document.getElementById("FGoffsetslider");
var FGoffset = document.getElementById("FGoffset");
var FGscaleslider = document.getElementById("FGscaleslider");
var FGscale = document.getElementById("FGscale");
var FGprecisionslider = document.getElementById("FGprecisionslider");
var FGprecision = document.getElementById("FGprecision");
var x1RB = document.getElementById("x1RB");
var x10RB = document.getElementById("x10RB");
var x100RB = document.getElementById("x100RB");
var x1kRB = document.getElementById("x1kRB");
var x10kRB = document.getElementById("x10kRB");
var sineRB = document.getElementById("sineRB");
var squareRB = document.getElementById("squareRB");
// Oscope interface states
let trigger = [240, 240];
var digiMode = [0, 0];
var mvPerDiv = 500;
var ground = 200,
  usPerDiv = 1000;
var risingEdge = 1;
var x10sq = 1;
var setandtriggered = 0;
var triggered;
var ch1on = 0;
var ch2on = 0;
var chSelected = 1;
var currentCh = 0;
var trigCh = 1;
const vSize = 1000
var z = new Array(3 * vSize); // input buffer
var v = new Array(vSize);
voffset = 0; // chan 1
var u = new Array(vSize);
uoffset = 0; // chan 2
var w = new Array(vSize);
woffset = 0; // time
var yoff = 125;
var tstart = 0,
  wend = 0;
var i;
var risingedges;
// Function generator state
var FGmode = "square";
var sineoffset = 0;


// INITIAL SETUP 
ch1RB();
ch1.checked = true;
speedup(); 
speeddown();
voltup();
voltdown();
getData();
runmode();
ctx.font = "14px Arial";
ctx.translate(-0.5, -0.5);
resolution.innerHTML = resslider.value;
freq.innerHTML = FGfreqslider.value;
var autosetcount = 0; // static autoset vars
var maxV = 0,
  minV = 400;

function measureMinMax() {
  var x;
  maxV = 0;
  minV = 400;
  for (i = 1; i < wend; i++) {
    x = v[i];
    if (x > maxV) maxV = x;
    if (x < minV) minV = x;
  }
}

function autoset() {
  var mytimeout;
  if (autosetcount == 0) {
    ch1RB();
    if (usPerDiv > 5000) {
      speeddown(); // hack to reduce antialiasing 
      speeddown();
      speeddown();
    }
    ground = 200;
    updateGround();
  } else if (autosetcount < 6 && (autosetcount % 2 == 1)) {
    measureMinMax();
    ground += 200 - minV;
    updateGround();
  } else if (autosetcount < 6) {
    measureMinMax();
    if (maxV > 400) { voltup(); }
    if (maxV < 250) { voltdown(); }
  } else {
    trigger[0] = 240;
    measureFreq();
    if (risingedges > 8) speeddown();
    if (risingedges < 4) speedup();
  }
  if (maxV < 400 && risingedges > 3 && risingedges < 9) setandtriggered = 1;
  else setandtriggered = 0;
  mytimeout = setTimeout(autoset, 600);
  if ((setandtriggered && autosetcount > 3) || autosetcount > 8) {
    clearTimeout(mytimeout);
    autosetcount = 0;
  }
  autosetcount++;
}

function vPush(item) {
  v[voffset++] = item;
  voffset %= vSize;
}

function uPush(item) {
  u[uoffset++] = item;
  uoffset %= vSize;
}

function wPush(item) {
  w[woffset++] = item;
  woffset %= vSize;
}

function measureFreq() {
  var oldx = 0,
    firstw = 0,
    lastw = 0;
  risingedges = 0;
  firstw = 0;
  if (chSelected == 1) {
    let trigV = Math.floor(trigger[0]);
    oldx = v[0];
    for (i = 1; i < wend; i++) {
      if (oldx > trigV && v[i] <= trigV) {
        risingedges++;
        if (firstw < 1) firstw = w[i];
        else lastw = w[i];
      }
      oldx = v[i];
    }
    return 1000000 / (usPerDiv * (lastw - firstw) / (risingedges - 1) / 50);
  }
  if (chSelected == 2) {
    let trigV = Math.floor(trigger[1]);
    oldx = u[0];
    for (i = 1; i < wend; i++) {
      if (oldx > trigV && u[i] <= trigV) {
        risingedges++;
        if (firstw == 0) firstw = w[i];
        else lastw = w[i];
      }
      oldx = u[i];
    }
    return 1000000 / (usPerDiv * (lastw - firstw) / (risingedges-1) / 50);
  }
  return 0;
}

function setsine() {
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "sineOn", true);
  xhttp.send();
  FGmode = "sine";
  duty.style.color = "lightgrey";
  FGoffset.style.color = "black";
  FGscale.style.color = "black";
}

function setsquare() {
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "squareOn", true);
  xhttp.send();
  FGmode = "square";
  FGoffset.style.color = "lightgrey";
  FGscale.style.color = "lightgrey";
  duty.style.color = "black";
}

function ch1RB() {
  chSelected = 1;
  if (trigCh == 1) {
    trig.innerHTML = "Trigger on Ch1";
    trig.style.color = "black";
  } else {
    trig.innerHTML = "Set Trigger Ch1";
    trig.style.color = "grey";
  }
  vert.innerHTML = "Vertical Ch1";
  vert.style.color = "darkorange";
  updateChannel("ch?val=0");
}

function ch2RB() {
  chSelected = 2;
  if (trigCh == 2) {
    trig.innerHTML = "Trigger on Ch2";
    trig.style.color = "black";
  } else {
    trig.innerHTML = "Set Trigger Ch2";
    trig.style.color = "grey";
  }
  vert.innerHTML = "Vertical Ch2";
  vert.style.color = "blue";
  updateChannel("ch?val=1");
}

function updateChannel(str) {
  var res = new Array(2);
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      res = this.responseText.split(",");
      ground = res[0];
      gndslider.value = ground;
      if (chSelected == 1) {
        if (res[1] == 1 || res[1] == 3) ACCB.checked = true;
        else ACCB.checked = false;
      } else { // chSelected==2
        if (res[1] == 2 || res[1] == 3) ACCB.checked = true;
        else ACCB.checked = false;
      }
      triggerslider.value = trigger[chSelected - 1];
      updateRunB();
      updateDigiB();
      updateGround();
    }
  };
  xhttp.open("GET", str, true);
  xhttp.send();
}

function updateGround() {
  var xhttp = new XMLHttpRequest();
  var str = "ground?val=";
  var res = str.concat(ground);
  xhttp.open("GET", res, true);
  xhttp.send();
  gndslider.value = ground;
}

function updateRunB() {
  if ((chSelected == 1 && ch1on) || (chSelected == 2 && ch2on)) {
    runB.innerHTML = "Run";
    runB.style = "background-color:lime";
  } else {
    runB.innerHTML = "Off";
    runB.style = "background-color:pink";
  }
}

function updateDigiB() {
  if ((chSelected == 1 && digiMode[0]) || (chSelected == 2 && digiMode[1])) {
    digiB.style = "background-color:lime";
  } else {
    digiB.style = "background-color:pink";
  }
}

function toggleDigiMode() {
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "toggleDigital", true);
  xhttp.send();
  digiMode[chSelected - 1] = !digiMode[chSelected - 1];
  updateDigiB();
}

function runmode() {
  var xhttp = new XMLHttpRequest();
  if (chSelected == 1) {
    ch1on = !ch1on;
  } else ch2on = !ch2on;
  updateRunB();
  var str = "chOn?val=";
  var res = str.concat(ch1on + 2 * ch2on); // 0 of off, 1 on off, 2, off on, 3 on on 
  xhttp.open("GET", res, true);
  xhttp.send();
}
FGfreqslider.oninput = function() {
  var nval = this.value*x10sq;
  if (FGmode=="sine") 
    nval = findSineFreq(nval);
  if (nval>1000) {
    var val = nval/1000;
    freq.innerHTML = String(val.toPrecision(3))+"k";
  }  else
    freq.innerHTML = nval.toPrecision(3);
}
FGfreqslider.onchange = function() {
  updateFGfreq(this.value);
}
FGoffsetslider.oninput = function() {
  FGoffset.innerHTML = this.value;
  sineoffset = this.value;
}
FGoffsetslider.onchange = function() {
  updateFGoffset(sineoffset);
}
FGscaleslider.oninput = function() {
  FGscale.innerHTML = "Ampl. x" + String(1 / (1 << this.value));
}
FGscaleslider.onchange = function() {
  updateFGscale(this.value);
}

FGprecisionslider.onchange = function() {
  updateFGprecision(this.value);
}

resslider.oninput = function() {
  resolution.innerHTML = this.value;
}
resslider.onchange = function() {
  var xhttp = new XMLHttpRequest();
  var str = "resolution?val=";
  var res = str.concat(this.value);
  xhttp.open("GET", res, true);
  xhttp.send();
}

dutyslider.oninput = function() {
  if (FGmode == "square") {
    duty.innerHTML = this.value;
  }
}
dutyslider.onchange = function() {
  if (FGmode == "square") {
    var xhttp = new XMLHttpRequest();
    var str = "duty?val=";
    var res = str.concat(this.value);
    xhttp.open("GET", res, true);
    xhttp.send();
  }
}

function findSineFreq( f_target) {
  var step, divi,f, delta, delta_min=10000, step_target, divi_target=0;
    for ( step=1;step<2000; step++) {
      for ( divi=0; divi<FGprecisionslider.value; divi++) {
        f=125.6*step/(divi+1);
        delta= Math.abs((f_target-f)); 
        if (delta  < delta_min) {delta_min=delta; step_target=step; divi_target=divi;}
      }
    }
    return 8500000 * step_target  / (1 + divi_target) / 65536;
}

function updateFGfreq(val) {
  var xhttp = new XMLHttpRequest();
  var str = "testfq?val=";
  var nval = val*x10sq;
  let res = str.concat(nval);
  xhttp.open("GET", res, true);
  xhttp.send();

  if (FGmode=="sine") 
    nval = findSineFreq(nval);
  if (nval>=1000) {
    var sval = nval/1000;
    freq.innerHTML = String(sval.toPrecision(3))+"k";
  }  else
    freq.innerHTML = nval.toPrecision(3);
}

function updateFGoffset(val) {
  var xhttp = new XMLHttpRequest();
  var str = "sineoffset?val=";
  var res = str.concat(val);
  xhttp.open("GET", res, true);
  xhttp.send();
  FGoffset.innerHTML = "Offset " + String(val);
}

function updateFGprecision(val) {
  var xhttp = new XMLHttpRequest();
  var str = "sineprecision?val=";
  var res = str.concat(val);
  xhttp.open("GET", res, true);
  xhttp.send();
  updateFGfreq(FGfreqslider.value);
}

function updateFGscale(val) {
  var xhttp = new XMLHttpRequest();
  var str = "sinescale?val=";
  var res = str.concat(val);
  xhttp.open("GET", res, true);
  xhttp.send();
  FGscale.innerHTML = "Ampl. x" + String(1 / (1 << val));
}

triggerslider.oninput = function() {
  trigCh = chSelected;
  trigger[chSelected - 1] = this.value;
  trig.innerHTML = "Trigger on Ch" + String(trigCh);
  trig.style.color = "black";
}
hoffslider.oninput = function() {
  yoff = Math.floor(this.value);
}
gndslider.oninput = function() {
  ground = this.value;
}
gndslider.onchange = function() {
  updateGround();
}

function speedup() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      usPerDiv = parseInt(this.responseText);
      document.getElementById("speed").innerHTML = usPerDiv;
      tstart = 0; // force refresh    
    }
  };
  xhttp.open("GET", "speedup", true);
  xhttp.send();
}

function speeddown() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      usPerDiv = parseInt(this.responseText);
      document.getElementById("speed").innerHTML = usPerDiv;
      tstart = 0; // force refresh    
    }
  };
  xhttp.open("GET", "speeddown", true);
  xhttp.send();
}

function voltup() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("volt").innerHTML = this.responseText;
      mvPerDiv = parseInt(this.responseText);
    }
  };
  xhttp.open("GET", "voltup", true);
  xhttp.send();
}

function voltdown() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("volt").innerHTML = this.responseText;
      mvPerDiv = parseInt(this.responseText);
    }
  };
  xhttp.open("GET", "voltdown", true);
  xhttp.send();
}
ACCB.addEventListener('change', () => {
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "toggleAC", true);
  xhttp.send();
});
edgeCB.addEventListener('change', () => {
  risingEdge = !risingEdge;
});
x1RB.addEventListener('change', () => {
  x10sq = 1;
  updateFGfreq(FGfreqslider.value);
});
x10RB.addEventListener('change', () => {
  x10sq = 10;
  updateFGfreq(FGfreqslider.value);
});
x100RB.addEventListener('change', () => {
  x10sq = 100;
  updateFGfreq(FGfreqslider.value);
});
x1kRB.addEventListener('change', () => {
  x10sq = 1000;
  updateFGfreq(FGfreqslider.value);
});
x10kRB.addEventListener('change', () => {
  x10sq = 10000;
  updateFGfreq(FGfreqslider.value);
});
sineRB.addEventListener('change', () => {
  setsine();
});
squareRB.addEventListener('change', () => {
  setsquare();
});

function LEDon() {
  ctx.fillStyle = '#00ff00';
  ctx.strokeStyle = '#00cc00';
}

function LEDoff() {
  ctx.fillStyle = '#008800';
  ctx.strokeStyle = '#007700';
}

var togglevar; 
function toggleLED (){
  togglevar = !togglevar;
  ctx.setLineDash([]);
  ctx.lineWidth = 9;
  ctx.beginPath();
  ctx.arc(492, 9, 4, 0, 2 * Math.PI);
  if (togglevar) LEDon();
  else LEDoff();
  ctx.stroke();
  ctx.fill();
  ctx.lineWidth = 1; //reset line and text color
  ctx.fillStyle = '#000000';
}

function drawScope() {
  var tmp = Math.floor(yoff);
  ctx.clearRect(0, 0, 501, 401);
  toggleLED();
  ctx.setLineDash([]);
  ctx.strokeStyle = "#008800";
  ctx.beginPath();
  ctx.moveTo(250, 0);
  ctx.lineTo(250, 400);
  ctx.moveTo(0, 200);
  ctx.lineTo(500, 200);
  ctx.stroke();
  ctx.beginPath();
  ctx.setLineDash([1, 5]);
  for (i = 1; i < 8; i++) {
    ctx.moveTo(0, i * 50);
    ctx.lineTo(500, i * 50);
  }
  for (i = 1; i < 10; i++) {
    ctx.moveTo(i * 50, 0);
    ctx.lineTo(i * 50, 400);
  }
  ctx.stroke();
  //trigger pointer
  ctx.setLineDash([]);
  if (triggered) ctx.strokeStyle = "#00FF00"; // green pointer
  else ctx.strokeStyle = "#FF0000"; // red pointer
  ctx.beginPath();
  ctx.moveTo(yoff, 10);
  ctx.lineTo(yoff + 10, 1);
  ctx.lineTo(yoff - 10, 1);
  ctx.lineTo(yoff, 10);
  ctx.stroke();
  ctx.fillText(String(trigCh), yoff - 3, 20);
  //trigger voltage
  let xt = 400 - trigger[chSelected - 1];
  ctx.strokeStyle = "#880000";
  ctx.setLineDash([5, 5]);
  ctx.fillText(String((trigger[chSelected - 1] - ground) * Math.floor(mvPerDiv / 50)) + " mV", 440, xt);
  ctx.fillText(String(Math.round(measureFreq())) + " Hz", 440, xt + 20);
  ctx.beginPath();
  ctx.moveTo(0, xt);
  ctx.lineTo(500, xt);
  ctx.stroke();
  // ground line
  ctx.strokeStyle = "#000088";
  ctx.fillText("0 mV", 450, 400 - ground);
  ctx.beginPath();
  ctx.moveTo(0, 400 - ground);
  ctx.lineTo(500, 400 - ground);
  ctx.stroke();
}

function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4) { // && this.status == 200 ) { //ignore errors
      z = this.responseText.split(",");
      setTimeout(updateloop, 10); // repeat call up to 100Hz
    }
  };
  xhttp.open("GET", "up", true);
  xhttp.send();
}
var t = 0;

function processData() {
  i = 0;
  while (i < z.length) {
    t = z[i++];
    if (tstart == 0) { //t > (tstart + 10*usPerDiv)) { //wrap
      wend = woffset;
      woffset = 0;
      voffset = 0;
      uoffset = 0
      tstart = t;
    }
    wPush(Math.floor((500 * (t - tstart)) / (10 * usPerDiv)));
    if (ch1on)   vPush(z[i++]);
    if (ch2on)   uPush(z[i++]);
  }
  wend = woffset - 1;
}

function findTrigger() {
  var i, oldx = 0,
    x = 0;
  var istart = Math.floor(wend / 3); // choose left of middle
  var iend = Math.floor(wend);
  var trigV = Math.floor(trigger[trigCh - 1]);
  triggered = 1;
  if (trigCh == 1) oldx = v[istart - 1];
  if (trigCh == 2) oldx = u[istart - 1];
  for (i = istart; i < iend; i++) {
    if (trigCh == 1) x = v[i];
    if (trigCh == 2) x = u[i];
    if (risingEdge && oldx > trigV && x <= trigV) { return i; } else if (!risingEdge && oldx < trigV && x >= trigV) { return i; }
    oldx = x;
  }
  if (trigCh == 1) oldx = v[istart];
  if (trigCh == 2) oldx = u[istart];
  for (i = istart-1; i > 0; i--) {
    if (trigCh == 1) x = v[i];
    if (trigCh == 2) x = u[i];
    if (!risingEdge && oldx > trigV && x <= trigV) { return i; } else if (risingEdge && oldx < trigV && x >= trigV) { return i; }
    oldx = x;
  }
  triggered = 0;
  return istart;
}

function drawLine(whichchannel) {
  var iend = Math.floor(wend),
    yshift = 0,
    oldx, edge;
  ctx.setLineDash([]);
  ctx.beginPath();
  edge = findTrigger();
  yshift = w[edge] - 250 - yoff + 250;
  if (whichchannel == 1) x = v[0];
  if (whichchannel == 2) x = u[0];
  ctx.moveTo(w[0] - yshift, 400 - x);
  oldx = x;
  for (i = 1; i < iend; i++) {
    if (whichchannel == 1) x = v[i];
    if (whichchannel == 2) x = u[i];
    if (w[i] - w[i - 1] > 50) // || x < oldx ) // if large jump or return stroke ignore
      ctx.moveTo(w[i] - yshift, 400 - x);
    else
      ctx.lineTo(w[i] - yshift, 400 - x);
    oldx = x;
  }
  ctx.stroke();
}

function updateloop() {
  tstart = 0; // refresh line
  getData();
  if (z.length > 6) { // skip short packet, likely bad data
    processData();
    drawScope();
    if (ch1on) {
      ctx.strokeStyle = "darkorange";
      drawLine(1);
    }
    if (ch2on) {
      ctx.strokeStyle = "blue";
      drawLine(2);
    }
  }
  frame.innerHTML = " " //" z="+String(z.length)
    +  " data/frame=" + String(wend);
}
</script>
</body>
</html>
)===";
