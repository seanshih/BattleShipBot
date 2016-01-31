 /* Start Header -------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.                    
Reproduction positionof this file or its contents without the    
prior written consent of DigiPen Institute of Technology is prohibited.
                                                                             
File Name:     ConsoleManager_Impl.h 
Purpose:       interface definition for ConsoleManager_Impl
Language:      C++ 
Platform:      VS2013, x64 Win8         
Project:       hsihung.shih_CS260_2
Author:        Hsi-Hung Shih, hsihung.shih, 180006613 
Creation date: Sun, Mar  8, 2015  3:20:46 AM
- End Header --------------------------------------------------------*/


#include <utility>

template<typename Handler_t, typename ...Args>
void ConsoleManager::AddHandler(Args&&... args)
{
    AddHandler(std::make_unique<Handler_t>(std::forward<Args>(args)...));
}
