static const char mainPage[]  = "<html> \
<head> \
<meta http-equiv='refresh' content='60'> \
<title>Voltage Reference Device</title> \
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
<br> <br> \
<center> \
<div class='card'> \
<div class='header'> \
  Set Vref (0-3.3V): \
</div> \
<form> \
D1: \
<input type='text' name='D1userVRef' maxlength='4' size='4'> \
<input type='submit' value='Vref'> <br> <br> \
</form> \
<form> \
D2:\
<input type='text' name='D2userVRef' maxlength='4' size='4'> \
<input type='submit' value='Vref'> <br> <br> \
</form> \
<form> \
D3:\
<input type='text' name='D3userVRef' maxlength='4' size='4'> \
<input type='submit' value='Vref'> <br> <br> \
</form> \
<form> \
D4:\
<input type='text' name='D4userVRef' maxlength='4' size='4'> \
<input type='submit' value='Vref'> <br> <br> \
</form> \
PWM Status:<br> \
D1: <b> @@d1VRef@@ V</b> <br>\
D2: <b> @@d2VRef@@ V</b> <br>\
D3: <b> @@d3VRef@@ V</b> <br>\
D4: <b> @@d4VRef@@ V</b> <br>\
</center> \
</body> \
</html>";
