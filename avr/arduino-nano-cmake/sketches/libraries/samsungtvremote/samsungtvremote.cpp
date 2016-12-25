/*
 *  samsungtvremote - a helper library to send IR commands to Smart Tv.
 *
 *  (C) Amitesh Singh <singh.amitesh@gmail.com>, 2016
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "samsungtvremote.h"

#define SMART_HUB 	0xE0E09E61
#define ENTER     	0xE0E016E9
#define UP        	0xE0E006F9
#define DOWN      	0xE0E08679
#define LEFT      	0xE0E0A659
#define RIGHT 		0xE0E046B9
#define POWER  		0xE0E040BF
#define SOURCE 		0xE0E0807F
#define HDMI   		0xE0E0D12E
#define BUTTON1  	0xE0E020DF
#define BUTTON2  	0xE0E0A05F
#define BUTTON3  	0xE0E0609F
#define BUTTON4 	0xE0E010EF
#define BUTTON5 	0xE0E0906F
#define BUTTON6 	0xE0E050AF
#define BUTTON7 	0xE0E030CF
#define BUTTON8 	0xE0E0B04F
#define BUTTON9 	0xE0E0708F
#define BUTTON0 	0xE0E08877
#define PRECHANNEL 	0xE0E0C837
#define VOLUP     	0xE0E0E01F
#define VOLDOWN  	0xE0E0D02F
#define MUTE    	0xE0E0F00F
#define CHLIST  	0xE0E0D629
#define CHUP  		0xE0E048B7
#define CHDOWN  	0xE0E008F7
#define MENU 		0xE0E058A7
#define TOOLS 		0xE0E0D22D
#define EXIT 		0xE0E0B44B
#define BACKWARD 	0xE0E0A25D
#define PAUSE   	0xE0E052AD
#define FORWARD 	0xE0E012ED
#define RECORD 		0xE0E0926D
#define PLAY 		0xE0E0E21D
#define STOP 		0xE0E0629D

void samsungtvremote::powerButton()
{
	_irsend.sendSAMSUNG(POWER, _protocolBits);
}

void samsungtvremote::muteButton()
{
	_irsend.sendSAMSUNG(MUTE, _protocolBits);
}

void samsungtvremote::leftButton()
{
	_irsend.sendSAMSUNG(LEFT, _protocolBits);
}

void samsungtvremote::rightButton()
{
	_irsend.sendSAMSUNG(RIGHT, _protocolBits);
}

void samsungtvremote::upButton()
{
	_irsend.sendSAMSUNG(UP, _protocolBits);
}

void samsungtvremote::downButton()
{
	_irsend.sendSAMSUNG(DOWN, _protocolBits);
}

void samsungtvremote::enterButton()
{
	_irsend.sendSAMSUNG(ENTER, _protocolBits);
}

void samsungtvremote::volUpButton()
{
	_irsend.sendSAMSUNG(VOLUP, _protocolBits);
}

void samsungtvremote::volDownButton()
{
	_irsend.sendSAMSUNG(VOLDOWN, _protocolBits);
}

void samsungtvremote::playButton()
{
	_irsend.sendSAMSUNG(PLAY, _protocolBits);
}

void samsungtvremote::pauseButton()
{
	_irsend.sendSAMSUNG(PAUSE, _protocolBits);
}

void samsungtvremote::stopButton()
{
	_irsend.sendSAMSUNG(STOP, _protocolBits);
}

void samsungtvremote::sourceButton()
{
	_irsend.sendSAMSUNG(SOURCE, _protocolBits);
}

void samsungtvremote::hdmiButton()
{
	_irsend.sendSAMSUNG(HDMI, _protocolBits);
}

void samsungtvremote::exitButton()
{
	_irsend.sendSAMSUNG(EXIT, _protocolBits);
}

void samsungtvremote::smartHubButton()
{
	_irsend.sendSAMSUNG(SMART_HUB, _protocolBits);
}
