var bitToESP = false;
var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);
var recievedPressure;
var recievedFlow;

connection.onopen = function () {
    connection.send('Connect ' + new Date());
};
connection.onerror = function (error) {
    console.log('WebSocket Error ', error);
};
connection.onmessage = function (e) {  
    console.log('Server: ', e.data);
	var type = e.data.substring(0, 1)
	
	switch(type) {
	
	case "p":	
	recievedPressure=parseFloat(e.data.substring(1, 15)).toPrecision(5);	
	pressureBar = recievedPressure/100000;
	document.getElementById("p1").innerHTML = e.data.substring(1, 10) + " Pa (" + String(pressureBar) + " bar)";
	break;
	
	case "f":	
	recievedFlow=parseFloat(e.data.substring(1, 15)).toPrecision(5);
	flowLiterProMin = recievedFlow*1000*60;
	document.getElementById("p2").innerHTML = e.data.substring(1, 10) + " m^3/s (" + String(flowLiterProMin) + " l/min)";
	break;
	
	default:
	break;
	}
};
connection.onclose = function(){
    console.log('WebSocket connection closed');
};

function sliderDataToESP() {
    var sliderData = document.getElementById('dataToESPSlider').value**2/1023; //parabolic slider
    var sliderDataString = '#'+ sliderData.toString(2);
    console.log(sliderDataString);
    connection.send(sliderDataString);
}

function flipBitToESP(){
    bitToESP = ! bitToESP;
    if(bitToESP){
        connection.send("s");
        document.getElementById('bitToESPButton').style.backgroundColor = '#00878F';
        document.getElementById('dataToESPSlider').className = 'disabled';
        document.getElementById('dataToESPSlider').disabled = true;
    } else {
        connection.send("r");
        document.getElementById('bitToESPButton').style.backgroundColor = '#999';
        document.getElementById('dataToESPSlider').className = 'enabled';
        document.getElementById('dataToESPSlider').disabled = false;
        sliderDataToESP();
    }  
}
