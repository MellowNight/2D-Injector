# BE-injector
A DLL manual mapper built on Fortevisor, intended for usage on Battleye games. 

The code on this branch queues an APC to load a signed DLL, then it maps a cheat over the signed dll. This is necessary for bypassing PUBG's Xenuine protection because Xenuine frees PAGE_EXECUTE_READWRITE memory allocations that aren't part of an internal whitelist. 
