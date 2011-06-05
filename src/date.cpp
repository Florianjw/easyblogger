/**
 * get_localdate.cpp
 * 
 * Copyright 2010 Florian Weber <florian.weber@sfz-bw.de>
 * 
 * This program is free software; you can redistribute it and/or modify
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
**/

#include "date.hpp"

#include "lines.hpp"

#include <cstdio>
#include <ctime>
#include <clocale>
#include <cstring>

using namespace std;




std::string get_localdate(std::string format,std::string locale){
	time_t TIME;
	char timestring[51];
	struct tm *timeinfo;
	setlocale(LC_ALL, locale.c_str());
	time(&TIME);
	timeinfo=localtime(&TIME);
	strftime(timestring,50,format.c_str(),timeinfo);
	timestring[50]=0;
	
	return replace(string(timestring),"  "," ");
}

std::string get_localdate(settings S){
	string format=DEFAULT_TIME_FORMAT,locale=DEFAULT_LOCALE;
	if(!S.time_format.empty()){
		format=S.time_format;
	}
	if(!S.locale.empty()){
		locale=S.locale;
	}
	return get_localdate(format, locale);
}
