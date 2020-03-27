# hdtSMP for Fallout4 (I hope)

A fork of [https://github.com/aers/hdtSMP64](https://github.com/aers/hdtSMP64) which is a fork of [original code](https://github.com/HydrogensaysHDT/hdt-skyrimse-mods) by hydrogensaysHDT

High Heels plugin has been removed, this repository only contains HDT-SMP.

## Changes 

+ build now works with unchanged bullet source
+ support both "." and "," as decimal seperators in config files
+ properly remove tracked armors from physics world if the tracked skeleton isn't part of the active scene
+ write transforms during game pauses, fixes physics appearing to reset while game is paused
+ reset system on loading screens so there's no brief physics glitches when loading between areas
+ better pause logic based on reading pause state from menumanager, fixing issues with added menus that don't pause the game (quickloot, VR)
+ add options for how to handle large rotations
+ hard-code a skeleton exception for the BenthicLurker skeleton because it is made incorrectly
+ add a debug command to print some stats to console
+ removed dependency on hdtSSEFramework and removed it from the repository
+ rename plugin hdtSMP64 to differentiate from the old version that depends on Framework, and because we can support VR, not just SSE

## Build Instructions

Requires Visual Studio 2019 (Community Edition is fine)

1) Download the clean skse64 source from [skse's website](http://skse.silverlock.org/)
2) Extract or clone this repository into the skse64 folder of the source. You can safely delete skse64_loader, skse64_steam_loader, skse64_loader_common, and the skse64 solution.
3) Download and build [bullet physics](https://github.com/bulletphysics/bullet3) using cmake and Visual Studio. Update the hdtSMP64 project to point to your bullet source and built libraries. The Release configuration is intended to be built with Bullet's "Use MSVC AVX" cmake option enabled, and Release_noavx without.
4) Download and build [Microsoft's detours library](https://github.com/microsoft/Detours) using nmake. Update the hdtSMP64 project to point to your built library.
5) Make the edits to SKSE source described below.
6) Build the solution :)

## SKSE Edits

GameMenus.h line 1090: 

add function 
```cpp
	bool                IsGamePaused() { return numPauseGame > 0; }
```
to MenuManager class

GameEvents.h line 662:

replace unk840 with
```cpp
EventDispatcher<TESMoveAttachDetachEvent>    		unk840;					//  840 - sink offset 0C8
```

NiObjects.h around line 207:

replace 

```cpp
	float		unkF8;				// 0F8
	UInt32		unkFC;				// 0FC
```

with

```cpp
	TESObjectREFR* m_owner; // 0F8
```

You will need to add a forward declaration or include for TESObjectREFR in NiObjects.h as well.

## Credits

hydrogensaysHDT - Creating this plugin

aers - that's me :)

ousnius - some fixes, and "consulting"


