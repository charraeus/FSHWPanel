/**
 * @file error.hpp
 * @author Christian Harraeus <christian@harraeus.de>
 * @brief Fehlermeldungen
 * @version 0.1
 * @date 2020-04-27
 * 
 * Copyright © 2017 - 2020. All rights reserved.
 * 
 */


#ifndef ARDUINO
#include <string>

std::string errorMsg[ ] {
    "Dispatcher-Fehler 1: Fehler in parseSerial.", 
    "Dispatcher-Fehler 2: Zu wenig Eingaben. Abbruch.", 
    "Dispatcher-Fehler 3: Zu langer Parsereingabeparameter."  
}
#else
#endif
