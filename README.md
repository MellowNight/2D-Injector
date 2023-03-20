# 2D-injector

2D Injector: An AetherVisor-based DLL injector. 

In the proof of concept code, I use NPT to stealthily map an unsigned payload DLL in the same address space as OWClient.dll (Overwolf's overlay DLL). Not only does the 2D injector bypass memory permission scans and DLL certificate checks, but it also renders the payload DLL invisible and unreadable.

This was tested and is undetected on Fortnite and EFT, but the major caveat was injector caused FPS drops with cheats.

I can see this being used for things other than cheating, such as debugging heavily protected apps or hiding from EDR. 

<br>

![image](https://user-images.githubusercontent.com/66788741/226236958-1166af80-bb8b-4c60-a148-7227ec157775.png)

<br>

FULL INFORMATION HERE:
https://mellownight.github.io/2023/01/10/BEInjector.html

# Acknowledgements:

Thank you to @red0x0002 for helping me test the injector and giving me tips. 
