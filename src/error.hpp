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

/**
 * @brief Fehlerkonstantenmakros für dispatcher
 * 
 */
#define _ERR_DISPATCHER_001 1   ///< "Fehler in parseSerial."
#define _ERR_DISPATCHER_002 2   ///< "Fehler - zuwenig Eingaben. Abbruch."
#define _ERR_DISPATCHER_003 3   ///< "Fehler: zu langer Parsereingabeparameter."

#ifndef ARDUINO
#include <string>

std::string errorMsg[ ] {
    "Dispatcher-Fehler 1: Fehler in parseSerial.", 
    "Dispatcher-Fehler 2: Zu wenig Eingaben. Abbruch.", 
    "Dispatcher-Fehler 3: Zu langer Parsereingabeparameter."  
}
#else
#endif
