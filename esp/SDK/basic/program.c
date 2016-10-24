/*
   Note, that all we do in user_init is calling the system_init_done_cb API function. This function accepts one parameter, which is a pointer to function which will be called once all system modules will be properly initialised. You can put your initialisation code in user_init too, but you can face problems with some system function (like WiFi), just because appropriate modules aren’t initialised yet. Thus, it is better to use system_init_done_cb and perform initialisation in the callback function.

   Beware of the dog

   ESP8266 has a watchdog functionality. And there is NO documented API to rule it (there is some undocumented stuff, but out of scope for this tutorial). Its timeout is 1 second.

   What does that mean? It means, that you have to return the control flow to system every second, otherwise the device will be rebooted. This code leads to the device reboot:

   void sdk_init_done_cb(void)
   { while(1);  or any long operation
   }

   In general, watchdog is not evil, it helps if the program hangs. And, 1 second is not so small as it sounds. Just keep this fact in mind.

 */

void sdk_init_done_cb(void)
{
   /* You app Initialization here */
}

void user_init()
{
   system_init_done_cb(sdk_init_done_cb);
}
