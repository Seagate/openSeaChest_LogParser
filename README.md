# **openSeaChest_LogParser**
## Cross platform utility useful for reading binary data logs from SATA, SAS, NVMe, and USB storage devices.

##### Copyright (c) 2014-2023 Seagate Technology LLC and/or its Affiliates, All Rights Reserved

#### Welcome to the openSeaChest_LogParser open source project!

### Summary of supported SCSI/SAS log pages
Page Code  |  Subpage Code Log  |  Page Name Reference                  |
-----------|--------------------|:-------------------------------------:|
    0Fh          00h                  Application Client                
-----------|--------------------|---------------------------------------|
    15h    |     00h            |     Background Scan                   |
-----------|--------------------|---------------------------------------|
    15h    |     02h            |     Background Operation              |
-----------|--------------------|---------------------------------------|
    37h    |     00h            |     Cache Statistics                  |
-----------|--------------------|---------------------------------------|
    19h    |     20h            |     Cache Memory Statistics           |
-----------|--------------------|---------------------------------------|
    19h    |     21h            |     Command Duration Limits Statistics| 
-----------|--------------------|---------------------------------------|
    0Dh    |     02h            |     Environmental Limits              |
-----------|--------------------|---------------------------------------|
    0Dh    |     01h            |     Environmental Reporting           |
-----------|--------------------|---------------------------------------|
    03h    |     00h            |     Read Error Counter                |
-----------|--------------------|---------------------------------------|
    05h    |     00h            |     Verify Error Counter              |
-----------|--------------------|---------------------------------------|
    02h    |     00h            |     Write Error Counter               |
-----------|--------------------|---------------------------------------|
    3Eh    |     00h            |     Factory Log                       |
-----------|--------------------|---------------------------------------|
    08h    |     00h            |     Format Status                     |
-----------|--------------------|---------------------------------------|
    2Fh    |     00h            |     Informational Exceptions          |
-----------|--------------------|---------------------------------------|
    0Ch    |     00h            |     Logical Block Provisioning        |
-----------|--------------------|---------------------------------------|
    06h    |     00h            |     Non-Medium Error                  |
-----------|--------------------|---------------------------------------|
    15h    |     01h            |     Pending Defects                   |
-----------|--------------------|---------------------------------------|
    1Ah    |     00h            |     Power Condition Transitions       |
-----------|--------------------|---------------------------------------|
    18h    |     00h            |     Protocol Specific Port            |
-----------|--------------------|---------------------------------------|
    10h    |     00h            |     Self-Test Results                 |
-----------|--------------------|---------------------------------------|
    11h    |     00h            |     Solid State Media                 |
-----------|--------------------|---------------------------------------|
    0Eh    |     00h            |     Start-Stop Cycle Counter          |
-----------|--------------------|---------------------------------------|
    00h    |     00h            |     Supported Log Pages               |
-----------|--------------------|---------------------------------------|
    00h    |     FFh            |     Supported Log Pages and Subpages  |
-----------|--------------------|---------------------------------------|
    0Dh    |     00h            |     Temperature                       |
-----------|--------------------|---------------------------------------|
    0Eh    |     01h            |     Utilization                       |
-----------|--------------------|---------------------------------------|
    14h    |     01h            |     Zoned Block Device Statistics     |
-----------|--------------------|---------------------------------------|

[![Build Status](https://travis-ci.org/Seagate/openSeaChest_LogParser.svg?branch=develop)](https://travis-ci.org/Seagate/openSeaChest_LogParser)

BINARIES and SOURCE CODE files of the openSeaChest_LogParser open source project have
been made available to you under the Mozilla Public License 2.0 (MPL).  The
openSeaChest project repository is maintained at
https://github.com/Seagate/openSeaChest_LogParser.

Compiled binary versions of the openSeaChest utilities for various operating
systems may be found at
https://github.com/Seagate/ToolBin/tree/master/openSeaChest_LogParser

***openSeaChest_LogParser is a tool which reads in various types of binary data
logs taken from storage devices and then outputs the data in human
readable formats.***

### The libraries:

**opensea-common**      - Operating System common operations, not specific to
                      storage standards. Contains functions and defines that
                      are useful to all other libraries.

**opensea-parser**      - Contains standard library calls for logs of Various
                      types.

### Source
Depend on your git version & client you can use either of the following two commands to clone the repository.

git clone --recurse-submodules -j8 https://github.com/Seagate/openSeaChest_LogParser.git

or

git clone --recursive https://github.com/Seagate/openSeaChest_LogParser.git


Note that cloning **_recursively_** is **_important_** as it clones all the necessary submodules.

### Building

#### Linux
Required Tools:
* gcc
* make

From a terminal, change directory to "openSeaChest_LogParser/Make/gcc".

Run the command "make release" to build the release version of the tool.
After compilation is complete, the tool binary will be located in the same folder as Make/gcc

#### Windows
Required Tools:
* Visual Studio 2017 (can also use msbuild)
  * Windows 10 SDK version 10.0.15063.0 for Visual Studio 2017 (x86 and x86_84)
* Visual Studio 2019 (can also use msbuild) 
 * Windows 10 SDK (latest installed version) for Visual Studio 2019 (x86 and x86_84)
 * Platform Toolset version 142 c++ 14 standard
* Visual Studio 2022 (can also use msbuild)
 * Windows 10 SDK (latest installed version) for Visual Studio 2022 (x86 and x86_84)
 * Platform Toolset version 143 c++ 14 standard

  ##### Visual Studio
  Open the solution file in "openSeaChest_LogParser/Make/VS.(version)/openSeaChest_LogParser/openSeaChest_LogParser/openseachest.sln".
  Set the desired build configuration.
  Press "F7" to build all of the tools and libraries, or select "Build->Build All" from the menu.
  The tools will be output into "openSeaChest_LogParser/Make/VS.(version)/(platform)/(build type)".

  Example: "openSeaChest_LogParser/Make/VS.2017/x64/Release"
  ##### msbuild
  From the developer command prompt for the version of visual studio that installed msbuild, change directory to "openSeaChest_LogParser/Make/VS.(version)".

  Build with the command "msbuild /p:Configuration=(build type) /p:Platform=(platform)".

  Example: "msbuild "msbuild /p:Configuration=Release /p:Platform=x64"

  Available Platforms:
  * Win32
  * x64

  Available Build Types:
  * Release
  * Debug
  * Static-Release
  * Static-Debug

#### Platforms

Under Linux this library can be built on the following platforms using
a cross platform compiler:

        x86
        x86_64

### Important Notes:

**Note:**  One gigabyte, or GB, equals one billion bytes when referring to hard
drive capacity. This software may use information provided by the operating
system to display capacity and volume size. The Windows file system uses a
binary calculation for gibibyte or GiB (2^30) which causes the abbreviated size
to appear smaller. The total number of bytes on a disk drive divided by the
decimal calculation for gigabyte or GB (10^9) shows the expected abbreviated
size. See this FAQ for more information
<http://knowledge.seagate.com/articles/en_US/FAQ/172191en?language=en_US>.

### About openSeaChest Command Line Diagnostics

Seagate offers both graphical user interface (GUI) and command line interface
(CLI) diagnostic tools for our storage devices.  SeaTools for Windows and
SeaTools Bootable for end users are the two most popular GUI tools.  These
tools support 15 languages.

openSeaChest diagnostics are command line utilities which are available for
expert users.  These command line tools assume the user is knowledgeable about
running software from the operating system command prompt.  CLI tools are in
the English language only and use "command line arguments" to define the
various tasks and specific devices.  openSeaChest diagnostics are available for
both Linux and Windows environments.

Linux versions of openSeaChest tools are available as stand alone 32 or 64-bit
executables you can copy to your own system.  Windows OS versions of
openSeaChest diagnostics are also available.

Technical Support for openSeaChest drive utilities is not available.  If you
have the time to send us some feedback about this software, especially if you
notice something we should fix or improve, we would greatly appreciate hearing
from you.  To report your comments and suggestions, please use this email
seaboard@seagate.com.  Please let us know the name and version of the tool you
are using.

openSeaChest drive utilities support SATA, SAS, NVMe and USB interface devices.

**openSeaChest_Basics** - Contains the most important tests and tools.

Other openSeaChest "break out" utilities are available and listed below which
offer more in-depth functionality in specific areas. These are:

**openSeaChest_Configure** - Tools to control various settings on the drives are
brought together into this one tool.  Typical commands, for example, include
Write Cache and Read Lookahead Cache enable or disable.

**openSeaChest_Erase** - The focus is on data erasure.  There are many different
choices for erasing data from simple boot track erase to full cryptographic
erasure (when supported).  Be sure to back up important data before using this
tool.  Seagate is not responsible for lost user data.  This tool only works on
Seagate drives.

**openSeaChest_Firmware** - Seagate products are run by firmware.  Having the
latest firmware can improve performance and or reliability of your product.
Seagate recommends applying new firmware to enhance the performance and or
reliability of your drive.  Most products may see one or two firmware updates
within the early life of the product.

**openSeaChest_Format** - Storage products which utilize the SAS, SCSI or Fibre
Channel interfaces are able to perform a full low-level media format in the
field.  The SCSI Format Unit command offers many specialty options to
accommodate a variety of conditions and to fine tune the procedure.  This
complex command deserves its own "break out" utility.

**openSeaChest_GenericTests** - Read Tests are the original disk drive diagnostic
which is to just read every sector on the drive, sequentially.  This tool
offers several common read tests which can be defined by either time or range.
In addition, the Long Generic test has the ability to repair problem sectors,
possibly eliminating an unnecessary drive return.

**openSeaChest_Info** - Historical generic activity logs (like total bytes written
and power on hours) and performance logs (like temperature) are available for
analytical review.  Identification and inquiry data stored on the drive is also
provided.  A view of SMART and device statistics is available when supported by
the drive.

**openSeaChest_LogParser** - Tool for parsing the Scsi/SAS generic logs. Also support
for ATA logs (identify, device statistics log, ext comp error log, sct temp log, 
NCQ error log, power condition log ext Self Test log), With support for Seagates
FARM Log and other Seagate Vendor Specific Logs.

**openSeaChest_Logs** - Several binary logs are maintained on disk drives. These logs 
are not stored in the user data area. Sometimes these logs are reviewed by support 
engineers to help analyze event history on the device. These binary data logs are 
saved to filenames using the drive's serial number and date-time. Some logs are 
Seagate-specific while many others are common to the interface specifications. 
Several of these binary logs may be parsed into human-readable form by using the 
openSeaChest_LogParser utility.

**openSeaChest_NVMe** - NVMe interface devices tend to have unique commands and 
challenges. Many of these commands are also unique to NVMe SSD products. This 
tool gathers the most important commands under one title. This tool is similar 
to openSeaChest_Basics but for NVMe.

**openSeaChest_PowerControl** - Seagate disk drives offer a multitude of options to
manage power.  This tool manipulates the various power modes.

**openSeaChest_Security** - Various settings are available on modern Seagate disk
drives which may be locked and unlocked.  These settings may interact with the
operating systems and systems BIOS.  Options also include cryptographic erase
for Self-Encrypting Drives (SED).

**openSeaChest_SMART** - This tool provides a closer look at the collected SMART
attributes data.  SMART stands for Self-Monitoring, Analysis and Reporting
Technology.  Short Drive Self Test is included as one of the standard SMART
commands. In addition, the DST & Clean test has the ability to repair problem
sectors, possibly eliminating an unnecessary drive return.

### Names, Logos, and Brands

All product names, logos, and brands are property of their respective owners.
All company, product and service names mentioned in the source code are for
clarification purposes only. Use of these names, logos, and brands does not
imply endorsement.

### Support and Open Source Statement

Seagate offers technical support for disk drive installation.  If you have any
questions related to Seagate products and technologies, feel free to submit
your request on our web site. See the web site for a list of world-wide
telephone numbers.

Seagate Support:
http://www.seagate.com/support-home/
Contact Us:
http://www.seagate.com/contacts/

Please report bugs/suggestions to https://github.com/Seagate/openSeaChest.
Include the output of --version information in the email. See the user guide
section 'General Usage Hints' for information about saving output to a log file.

This software uses open source packages obtained with permission from the
relevant parties. For a complete list of open source components, sources and
licenses, please see our Linux USB Boot Maker Utility FAQ for additional
information.

The newest online version of the openSeaChest Utilities documentation, open
source usage and acknowledgement licenses, and our Linux USB Boot Maker FAQ can
be found at: https://github.com/Seagate/openSeaChest.

Copyright (c) 2014-2023 Seagate Technology LLC and/or its Affiliates, All Rights Reserved

-----------------------------------------

BINARIES and SOURCE CODE files of the openSeaChest open source project have
been made available to you under the Mozilla Public License 2.0 (MPL).  Mozilla
is the custodian of the Mozilla Public License ("MPL"), an open source/free
software license.

https://www.mozilla.org/en-US/MPL/

Mozilla Public License Version 2.0
==================================

### 1. Definitions

**1.1. “Contributor”**  
    means each individual or legal entity that creates, contributes to
    the creation of, or owns Covered Software.

**1.2. “Contributor Version”**  
    means the combination of the Contributions of others (if any) used
    by a Contributor and that particular Contributor's Contribution.

**1.3. “Contribution”**  
    means Covered Software of a particular Contributor.

**1.4. “Covered Software”**  
    means Source Code Form to which the initial Contributor has attached
    the notice in Exhibit A, the Executable Form of such Source Code
    Form, and Modifications of such Source Code Form, in each case
    including portions thereof.

**1.5. “Incompatible With Secondary Licenses”**  
    means

* **(a)** that the initial Contributor has attached the notice described
    in Exhibit B to the Covered Software; or
* **(b)** that the Covered Software was made available under the terms of
    version 1.1 or earlier of the License, but not also under the
    terms of a Secondary License.

**1.6. “Executable Form”**  
    means any form of the work other than Source Code Form.

**1.7. “Larger Work”**  
    means a work that combines Covered Software with other material, in
    a separate file or files, that is not Covered Software.

**1.8. “License”**  
    means this document.

**1.9. “Licensable”**  
    means having the right to grant, to the maximum extent possible,
    whether at the time of the initial grant or subsequently, any and
    all of the rights conveyed by this License.

**1.10. “Modifications”**  
    means any of the following:

* **(a)** any file in Source Code Form that results from an addition to,
    deletion from, or modification of the contents of Covered
    Software; or
* **(b)** any new file in Source Code Form that contains any Covered
    Software.

**1.11. “Patent Claims” of a Contributor**  
    means any patent claim(s), including without limitation, method,
    process, and apparatus claims, in any patent Licensable by such
    Contributor that would be infringed, but for the grant of the
    License, by the making, using, selling, offering for sale, having
    made, import, or transfer of either its Contributions or its
    Contributor Version.

**1.12. “Secondary License”**  
    means either the GNU General Public License, Version 2.0, the GNU
    Lesser General Public License, Version 2.1, the GNU Affero General
    Public License, Version 3.0, or any later versions of those
    licenses.

**1.13. “Source Code Form”**  
    means the form of the work preferred for making modifications.

**1.14. “You” (or “Your”)**  
    means an individual or a legal entity exercising rights under this
    License. For legal entities, “You” includes any entity that
    controls, is controlled by, or is under common control with You. For
    purposes of this definition, “control” means **(a)** the power, direct
    or indirect, to cause the direction or management of such entity,
    whether by contract or otherwise, or **(b)** ownership of more than
    fifty percent (50%) of the outstanding shares or beneficial
    ownership of such entity.


### 2. License Grants and Conditions

#### 2.1. Grants

Each Contributor hereby grants You a world-wide, royalty-free,
non-exclusive license:

* **(a)** under intellectual property rights (other than patent or trademark)
    Licensable by such Contributor to use, reproduce, make available,
    modify, display, perform, distribute, and otherwise exploit its
    Contributions, either on an unmodified basis, with Modifications, or
    as part of a Larger Work; and
* **(b)** under Patent Claims of such Contributor to make, use, sell, offer
    for sale, have made, import, and otherwise transfer either its
    Contributions or its Contributor Version.

#### 2.2. Effective Date

The licenses granted in Section 2.1 with respect to any Contribution
become effective for each Contribution on the date the Contributor first
distributes such Contribution.

#### 2.3. Limitations on Grant Scope

The licenses granted in this Section 2 are the only rights granted under
this License. No additional rights or licenses will be implied from the
distribution or licensing of Covered Software under this License.
Notwithstanding Section 2.1(b) above, no patent license is granted by a
Contributor:

* **(a)** for any code that a Contributor has removed from Covered Software;
    or
* **(b)** for infringements caused by: **(i)** Your and any other third party's
    modifications of Covered Software, or **(ii)** the combination of its
    Contributions with other software (except as part of its Contributor
    Version); or
* **(c)** under Patent Claims infringed by Covered Software in the absence of
    its Contributions.

This License does not grant any rights in the trademarks, service marks,
or logos of any Contributor (except as may be necessary to comply with
the notice requirements in Section 3.4).

#### 2.4. Subsequent Licenses

No Contributor makes additional grants as a result of Your choice to
distribute the Covered Software under a subsequent version of this
License (see Section 10.2) or under the terms of a Secondary License (if
permitted under the terms of Section 3.3).

#### 2.5. Representation

Each Contributor represents that the Contributor believes its
Contributions are its original creation(s) or it has sufficient rights
to grant the rights to its Contributions conveyed by this License.

#### 2.6. Fair Use

This License is not intended to limit any rights You have under
applicable copyright doctrines of fair use, fair dealing, or other
equivalents.

#### 2.7. Conditions

Sections 3.1, 3.2, 3.3, and 3.4 are conditions of the licenses granted
in Section 2.1.


### 3. Responsibilities

#### 3.1. Distribution of Source Form

All distribution of Covered Software in Source Code Form, including any
Modifications that You create or to which You contribute, must be under
the terms of this License. You must inform recipients that the Source
Code Form of the Covered Software is governed by the terms of this
License, and how they can obtain a copy of this License. You may not
attempt to alter or restrict the recipients' rights in the Source Code
Form.

#### 3.2. Distribution of Executable Form

If You distribute Covered Software in Executable Form then:

* **(a)** such Covered Software must also be made available in Source Code
    Form, as described in Section 3.1, and You must inform recipients of
    the Executable Form how they can obtain a copy of such Source Code
    Form by reasonable means in a timely manner, at a charge no more
    than the cost of distribution to the recipient; and

* **(b)** You may distribute such Executable Form under the terms of this
    License, or sublicense it under different terms, provided that the
    license for the Executable Form does not attempt to limit or alter
    the recipients' rights in the Source Code Form under this License.

#### 3.3. Distribution of a Larger Work

You may create and distribute a Larger Work under terms of Your choice,
provided that You also comply with the requirements of this License for
the Covered Software. If the Larger Work is a combination of Covered
Software with a work governed by one or more Secondary Licenses, and the
Covered Software is not Incompatible With Secondary Licenses, this
License permits You to additionally distribute such Covered Software
under the terms of such Secondary License(s), so that the recipient of
the Larger Work may, at their option, further distribute the Covered
Software under the terms of either this License or such Secondary
License(s).

#### 3.4. Notices

You may not remove or alter the substance of any license notices
(including copyright notices, patent notices, disclaimers of warranty,
or limitations of liability) contained within the Source Code Form of
the Covered Software, except that You may alter any license notices to
the extent required to remedy known factual inaccuracies.

#### 3.5. Application of Additional Terms

You may choose to offer, and to charge a fee for, warranty, support,
indemnity or liability obligations to one or more recipients of Covered
Software. However, You may do so only on Your own behalf, and not on
behalf of any Contributor. You must make it absolutely clear that any
such warranty, support, indemnity, or liability obligation is offered by
You alone, and You hereby agree to indemnify every Contributor for any
liability incurred by such Contributor as a result of warranty, support,
indemnity or liability terms You offer. You may include additional
disclaimers of warranty and limitations of liability specific to any
jurisdiction.


### 4. Inability to Comply Due to Statute or Regulation

If it is impossible for You to comply with any of the terms of this
License with respect to some or all of the Covered Software due to
statute, judicial order, or regulation then You must: **(a)** comply with
the terms of this License to the maximum extent possible; and **(b)**
describe the limitations and the code they affect. Such description must
be placed in a text file included with all distributions of the Covered
Software under this License. Except to the extent prohibited by statute
or regulation, such description must be sufficiently detailed for a
recipient of ordinary skill to be able to understand it.


### 5. Termination

**5.1.** The rights granted under this License will terminate automatically
if You fail to comply with any of its terms. However, if You become
compliant, then the rights granted under this License from a particular
Contributor are reinstated **(a)** provisionally, unless and until such
Contributor explicitly and finally terminates Your grants, and **(b)** on an
ongoing basis, if such Contributor fails to notify You of the
non-compliance by some reasonable means prior to 60 days after You have
come back into compliance. Moreover, Your grants from a particular
Contributor are reinstated on an ongoing basis if such Contributor
notifies You of the non-compliance by some reasonable means, this is the
first time You have received notice of non-compliance with this License
from such Contributor, and You become compliant prior to 30 days after
Your receipt of the notice.

**5.2.** If You initiate litigation against any entity by asserting a patent
infringement claim (excluding declaratory judgment actions,
counter-claims, and cross-claims) alleging that a Contributor Version
directly or indirectly infringes any patent, then the rights granted to
You by any and all Contributors for the Covered Software under Section
2.1 of this License shall terminate.

**5.3.** In the event of termination under Sections 5.1 or 5.2 above, all
end user license agreements (excluding distributors and resellers) which
have been validly granted by You or Your distributors under this License
prior to termination shall survive termination.


### 6. Disclaimer of Warranty

> Covered Software is provided under this License on an “as is”
> basis, without warranty of any kind, either expressed, implied, or
> statutory, including, without limitation, warranties that the
> Covered Software is free of defects, merchantable, fit for a
> particular purpose or non-infringing. The entire risk as to the
> quality and performance of the Covered Software is with You.
> Should any Covered Software prove defective in any respect, You
> (not any Contributor) assume the cost of any necessary servicing,
> repair, or correction. This disclaimer of warranty constitutes an
> essential part of this License. No use of any Covered Software is
> authorized under this License except under this disclaimer.

### 7. Limitation of Liability

> Under no circumstances and under no legal theory, whether tort
> (including negligence), contract, or otherwise, shall any
> Contributor, or anyone who distributes Covered Software as
> permitted above, be liable to You for any direct, indirect,
> special, incidental, or consequential damages of any character
> including, without limitation, damages for lost profits, loss of
> goodwill, work stoppage, computer failure or malfunction, or any
> and all other commercial damages or losses, even if such party
> shall have been informed of the possibility of such damages. This
> limitation of liability shall not apply to liability for death or
> personal injury resulting from such party's negligence to the
> extent applicable law prohibits such limitation. Some
> jurisdictions do not allow the exclusion or limitation of
> incidental or consequential damages, so this exclusion and
> limitation may not apply to You.


### 8. Litigation

Any litigation relating to this License may be brought only in the
courts of a jurisdiction where the defendant maintains its principal
place of business and such litigation shall be governed by laws of that
jurisdiction, without reference to its conflict-of-law provisions.
Nothing in this Section shall prevent a party's ability to bring
cross-claims or counter-claims.


### 9. Miscellaneous

This License represents the complete agreement concerning the subject
matter hereof. If any provision of this License is held to be
unenforceable, such provision shall be reformed only to the extent
necessary to make it enforceable. Any law or regulation which provides
that the language of a contract shall be construed against the drafter
shall not be used to construe this License against a Contributor.


### 10. Versions of the License

#### 10.1. New Versions

Mozilla Foundation is the license steward. Except as provided in Section
10.3, no one other than the license steward has the right to modify or
publish new versions of this License. Each version will be given a
distinguishing version number.

#### 10.2. Effect of New Versions

You may distribute the Covered Software under the terms of the version
of the License under which You originally received the Covered Software,
or under the terms of any subsequent version published by the license
steward.

#### 10.3. Modified Versions

If you create software not governed by this License, and you want to
create a new license for such software, you may create and use a
modified version of this License if you rename the license and remove
any references to the name of the license steward (except to note that
such modified license differs from this License).

#### 10.4. Distributing Source Code Form that is Incompatible With Secondary Licenses

If You choose to distribute Source Code Form that is Incompatible With
Secondary Licenses under the terms of this version of the License, the
notice described in Exhibit B of this License must be attached.

## Exhibit A - Source Code Form License Notice

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

If it is not possible or desirable to put the notice in a particular
file, then You may include the notice in a location (such as a LICENSE
file in a relevant directory) where a recipient would be likely to look
for such a notice.

You may add additional accurate notices of copyright ownership.

## Exhibit B - “Incompatible With Secondary Licenses” Notice

    This Source Code Form is "Incompatible With Secondary Licenses", as
    defined by the Mozilla Public License, v. 2.0.
