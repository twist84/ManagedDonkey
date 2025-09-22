# [ManagedDonkey](https://github.com/twist84/ManagedDonkey)

[![wakatime](https://wakatime.com/badge/github/twist84/ManagedDonkey.svg)](https://wakatime.com/badge/github/twist84/ManagedDonkey)
[![Build status](https://ci.appveyor.com/api/projects/status/ha46jv9ytnxwlfr4/branch/main?svg=true)](https://ci.appveyor.com/project/theTwist84/ManagedDonkey/branch/main)
[![Build status](https://github.com/twist84/ManagedDonkey/actions/workflows/build.yml/badge.svg?branch=main)](https://github.com/twist84/ManagedDonkey/actions/workflows/build.yml)
[![Ask DeepWiki](https://deepwiki.com/badge.svg)](https://deepwiki.com/twist84/ManagedDonkey)

## What is this?

- Donkey is a project I started in August 2022, inspired by years of reverse engineering the Halo engine and learning C++.
- The goal is to create my own version of a Halo mod, documenting all the information for others to use.
- This project is intended for technical folk such as developers, not for users who want a finished product.

### Why Halo Online and why `ms23 1.106708` specifically?

- I started looking at this build back in mid 2015 and have grown quite a fondness for it.
- It was built with less compiler optimizations than newer builds.
- Working client simulation logic, broadcast logic (systemlink) and much more.

### Will other Halo builds be supported in the future?

- Maybe? While there's no plan to move to newer builds there's also no reason to stay on `ms23 1.106708` once everything that's missing from new builds is implemented.


## Getting started

- Source a build of `Halo Online ms23 1.106708`
  - Extract to `G:\Games\ms23`; refer to this as `GameDir`.
- Download files from [DonkeyFileShare](https://github.com/twist84/DonkeyFileShare).


## Project Configuration

- Right-click the `game` project
  - Properties -> Debugging -> Set Working Directory to `GameDir`.

- Right-click the `launcher` project
  - **Set as StartUp Project**
  - Properties -> Debugging -> Set Working Directory to `GameDir`.


## Building

- `Git` and `Powershell` are only used in a prebuild step to get the last commit revision.


## Debugging

- Required Extensions (Visual Studio 2022)
  - [Smart Command Line Arguments VS2022](https://marketplace.visualstudio.com/items?itemName=MBulli.SmartCommandlineArguments2022)
  - [Microsoft Child Process Debugging Power Tool 2022](https://marketplace.visualstudio.com/items?itemName=vsdbgplat.MicrosoftChildProcessDebuggingPowerTool2022)


## How can I help?

- Test Donkey.
- Share ideas.
- No outside contributions, please.


## Other

- For more information, join the [Halo Mods](https://discord.gg/halo-mods-615301822474878977) discord and the [ᴅᴏɴᴋᴇʏ ᴅᴇᴠᴇʟᴏᴘᴍᴇɴᴛ](https://discord.com/channels/615301822474878977/1380333857508950159) post.
