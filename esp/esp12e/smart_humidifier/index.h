const char mainPage[] PROGMEM = "<html> \
<head> \
<meta http-equiv='refresh' content='60'> \
<title>Smart Humidifier by Ami</title> \
<style> \
div.card { \
  width: 250px; \
  box-shadow: 0 4px 8px 0 rgba(0, 0, 0, 0.2), 0 6px 20px 0 rgba(0, 0, 0, 0.19); \
  text-align: center; \
  border-radius: 5px; \
  background-color: #F5F7A0 \
} \
div.header { \
    background-color: #E03C3C; \
    color: white; \
    padding: 10px; \
    font-size: 40px; \
    border-radius: 5px; \
} \
div.container { \
  padding: 4px; \ 
} \
</style> \
</head> \
<body> \
<center><h1> Smart Humidifier </h1> \
<div class='card'> \
<div class='header'> \
  <h2>@@humidity@@</h2> \
</div> \
<div class='container'> \
  <h3>Humidity</h3> \
</div> \
</div> \
<div class='card'> \
  <div class='header'> \
    <h2>@@roomTemp@@ C</h2> \
  </div> \
  <div class='container'> \
    <h3>Temperature</h3> \
  </div> \
</div> \
<br> <br> \
<form> \
Set Humid Value (10-60): \
<input type='text' name='userHumidValue' maxlength='2' size='2'> \
<input type='submit' value='Set Humid'> <br> <br> \
</form> \
Current Threshold humidity: <b>@@userhval@@</b> <br> \
Fan: <font color='@@color@@'> <b> @@fan@@ </b> </font> <br> \
Heater: <font color='@@color@@'> <b> @@heater@@ </b> </font> <br> \
</center> \
</body> \
</html>";
