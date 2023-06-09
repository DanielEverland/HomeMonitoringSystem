const char WEBSITE[] = R"rawliteral(
<DOCTYPE HTML>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css"
integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
<style>
html {
font-family: Arial;
display: inline-block;
margin: 0px auto;
text-align: center;
}
h2 {
font-size: 3.0rem;
}
p {
font-size: 3.0rem;
}
.units {
font-size: 1.2rem;
}
.dht-labels {
font-size: 1.5rem;
vertical-align: middle;
padding-bottom: 15px;
}
</style>
</head>
<body>
<h2>ESP8266 Home monitoring system</h2>
<p>
<i class="fas fa-thermometer-half" style="color:#059e8a;"></i>
<span class="dht-labels">Temperature</span>
<span id="temperature">%TEMPERATURE%</span>
<sup class="units">&deg;C</sup>
</p>
<p>
<i class="fas fa-tint" style="color:#00add6;"></i>
<span class="dht-labels">Humidity</span>
<span id="humidity">%HUMIDITY%</span>
<sup class="units">%</sup>
</p>
<p>
<i class="fas fa-exclamation"></i>
<span class="dht-labels">Motion</span>
<span id="motionDetection">%MOTION%</span>
</p>
<p>
<i class="fas fa-sun"></i>
<span class="dht-labels">Light</span>
<span id="lightDetection">%LIGHT%</span>
</p>
<p>
<i class="fas fa-key"></i>
<span class="dht-labels">Status RFID</span>
<span id="rfid">%RFID Status%</span>
</p>
<p>
<i class="fas fa-exclamation"></i>
<span class="dht-labels">Keypad</span>
<span id="keypad">%Keypad Status%</span>
</p>
<p>
<button class="button"
onmousedown="toggleCheckbox('on');"
ontouchstart="toggleCheckbox('on');">
<span>OPEN DOOR</span>
</button>
</p>
</body>
<script>
setInterval(function () {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function () {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("temperature").innerHTML = this.responseText;
}
};
xhttp.open("GET", "/temperature", true);
xhttp.send();
}, 1000);
setInterval(function () {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function () {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("humidity").innerHTML = this.responseText;
}
};
xhttp.open("GET", "/humidity", true);
xhttp.send();
}, 1000);
setInterval(function () {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function () {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("motionDetection").innerHTML = this.responseText;
}
};
xhttp.open("GET", "/motionDetection", true);
xhttp.send();
}, 1000);
setInterval(function () {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function () {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("lightDetection").innerHTML = this.responseText;
}
};
xhttp.open("GET", "/lightDetection", true);
xhttp.send();
}, 1000);
setInterval(function () {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function () {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("keypad").innerHTML = this.responseText;
}
};
xhttp.open("GET", "/keypad", true);
xhttp.send();
}, 1000);
setInterval(function () {
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function () {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("rfid").innerHTML = this.responseText;
}
};
xhttp.open("GET", "/rfid", true);
xhttp.send();
}, 1000);
function toggleCheckbox(x) {
var xhr = new XMLHttpRequest();
xhr.open("GET", "/" + x, true);
xhr.send();
}
</script>
</html>
)rawliteral";
