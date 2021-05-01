/*
 * 
 */
const char body[] PROGMEM = R"===(

<!DOCTYPE html>
<!DOCTYPE html>
<html><body>
<H1>
<a href="/H">Turn ON</a> LED.<br>
<a href="/L">Turn OFF</a> LED.<br>

</H1>

<span id="sliderlabel">  </span> <br>
<input type="range" min="1" max="100" value="50"  id="slider">
<span id="outputlabel">  </span> <br>

</body>
<script>

slider.oninput = function() {
  document.getElementById("sliderlabel").innerHTML = "slider val " + this.value;
}

slider.onchange = function() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("outputlabel").innerHTML = this.responseText;
    }
  };
  var str = "slider?val=";
  var res = str.concat(this.value);
  xhttp.open("GET", res, true);
  xhttp.send();
}


</script>

</html>
)===";
