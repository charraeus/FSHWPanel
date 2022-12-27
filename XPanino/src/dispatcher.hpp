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

#include <event.hpp>
#include <m803.hpp>
#include <xpdr.hpp>

extern ClockDavtronM803 m803;
extern TransponderKT76C xpdr;
extern EventQueueClass eventQueue;


/***************************************************************************************************
 * @brief dispatchen
 *
 */
class DispatcherClass {
public:
    /**
     * @brief dispatch a specific event.
     *
     * @param event Event to dispatch.
     */
    void dispatch(EventClass *event) const;


    /**
     * @brief dispatch all events in the event queue.
     *
     */
    void dispatchAll();
};