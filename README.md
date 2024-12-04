## [ManagedDonkey](https://github.com/twist84/ManagedDonkey)

[![wakatime](https://wakatime.com/badge/github/twist84/ManagedDonkey.svg)](https://wakatime.com/badge/github/twist84/ManagedDonkey)
[![Build status](https://ci.appveyor.com/api/projects/status/ha46jv9ytnxwlfr4/branch/main?svg=true)](https://ci.appveyor.com/project/theTwist84/ManagedDonkey/branch/main)
[![Build status](https://github.com/twist84/ManagedDonkey/actions/workflows/build.yml/badge.svg?branch=main)](https://github.com/twist84/ManagedDonkey/actions/workflows/build.yml)

## What is this?
 - This is Donkey, a project I started back in August 2022 after years of reverse engineering the Halo engine and learning C++ while I went.
 - I wanted to write my own take on what a Halo mod should be and started going to town, putting all the information I had in my head into one place that others could use it (**Except for Faber**).

## Getting started
 - Source a build of `Halo Online ms23 1.106708`
   - Extract this to somewhere like `G:\Games\ms23`; we will refer to this as `GameDir`.
 - Grab the files from [DonkeyFileShare](https://github.com/twist84/DonkeyFileShare).

### Required Visual Studio 2022 Extensions
 - [Smart Command Line Arguments VS2022](https://marketplace.visualstudio.com/items?itemName=MBulli.SmartCommandlineArguments2022)
 - [Microsoft Child Process Debugging Power Tool 2022](https://marketplace.visualstudio.com/items?itemName=vsdbgplat.MicrosoftChildProcessDebuggingPowerTool2022)

### Project Configuration
 - Right-click the `game` project
   - Properties -> Debugging -> Set Working Directory to the `GameDir`.

 - Right-click the `launcher` project
   - **Set as StartUp Project**
   - Properties -> Debugging -> Set Working Directory to the `GameDir`.

### Building:
 - `Git` and `Powershell`, only for the last commit revision

### Debugging:
 - Visual Studio 2022 Extensions
   - [Smart Command Line Arguments VS2022](https://marketplace.visualstudio.com/items?itemName=MBulli.SmartCommandlineArguments2022)
   - [Microsoft Child Process Debugging Power Tool 2022](https://marketplace.visualstudio.com/items?itemName=vsdbgplat.MicrosoftChildProcessDebuggingPowerTool2022)

## How can I help?
 - Mostly just by testing Donkey out.
 - Ideas are welcome.
 - No outside contributions, please.

## Other
 - For anything else, either join the [Anvil Station](https://discord.gg/Bp6knsfG2z) discord and join the [ᴅᴏɴᴋᴇʏ ᴅᴇᴠᴇʟᴏᴘᴍᴇɴᴛ](https://discord.com/channels/933259958701654016/1082719367512465430) thread or use the [discussions](https://github.com/twist84/ManagedDonkey/discussions).
