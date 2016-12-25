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

#ifndef SKETCHES_LIBRARIES_SAMSUNGTVREMOTE_H_
#define SKETCHES_LIBRARIES_SAMSUNGTVREMOTE_H_

#include <IRremote.h>

class samsungtvremote
{
	IRsend _irsend;

	static const uint8_t _protocolBits = 32;
public:
	void 				powerButton();
	void 				muteButton();
	void 				upButton();
	void				downButton();
	void 				leftButton();
	void				rightButton();
	void				enterButton();
	void				volUpButton();
	void				volDownButton();
	void				playButton();
	void				pauseButton();
	void				stopButton();
	void				sourceButton();
	void				hdmiButton();
	void				exitButton(); //or back
	void				smartHubButton();
};


#endif /* SKETCHES_LIBRARIES_SAMSUNGTVREMOTE_H_ */
