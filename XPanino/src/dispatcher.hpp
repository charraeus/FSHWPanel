/*********************************************************************************************************//**
 * @file dispatcher.hpp
 * @brief Dispatcher-Klasse
 *
 * @author Christian Harraeus (christian@harraeus.de)
 * @date 2022-12-13
 * @version 0.2
 *
 ***********************************************************************************************************/

#pragma once

#include <buffer.hpp>
#include <m803.hpp>
#include <xpdr.hpp>

extern ClockDavtronM803 m803;
extern TransponderKT76C xpdr;
extern EventQueueClass eventQueue;

/// @brief dispatchen
class DispatcherClass {
public:
    void dispatch(EventClass event);
};