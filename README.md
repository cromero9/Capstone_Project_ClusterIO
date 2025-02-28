# Capstone_Project_ClusterIO
Capstone Project
<html>
<H1>Setting up the ClusterIO Demo</H1>

<H2>Compiling the Source Code</H2>
The code to drive the demo can be found in the <i>source_code</i>
directory of the project. There are two iterations of this project, C and C++. The C project is the main and more simplistic iteration using just cluster_update.c and ClusterIO_events.h. The C++ version demonstrates object-oriented princples and implementaition and is not a complete development. The C++ files have been isolated under /source_code/CPP_Project<p>

The source is straightforward and uses standard ANSI C89 types,
but requires that the include path and library path include
the path to the Crank Storyboard IO headers and libraries.
See the current Makefile in source_code for the recommended libraries
for the STM32MP157 board<p>

Assuming a base path to a specific Storyboard Engine distribution is
defined as <b>SB_ROOT</b>:
<ul>
<dl>
<dt>Headers</dt>
<dd>The Storyboard IO header file <i>gre/greio.h</i> is located in
the <i>include</i> sub-directory of the runtime engine.</dd>
<dt>Libraries</dt>
<dd>The Storyboard IO library <i>libgreio.[a|so|lib|dll]</i> is located in
the <i>lib</i> sub-directory of the runtime engine.</dd>
</dl><p>
</ul>

A sample Makefile for a build might look something like this:
<pre>
SB_ROOT=/Applications/Crank_Software/Storyboard_Engine/3.1.0.201304081236/macos-x86-opengles_2.0-obj
SB_INC=-I$(SB_ROOT)/include
SB_LIB=-L$(SB_ROOT)/lib -lgreio

cluster_update: cluster_update.c
	$(CC) $(SB_INC) -o cluster_update cluster_update.c $(SB_LIB)
</pre>

Note: Storyboard for Linux is built as a 32 bit system, if running on a 64 bit Linux system then
you will need to do the following:
<ul>
	<li>Install the libc6-dev-i386 package to build 32-bit</li>
	<li>Specify '-m32' in the build command right before the '-o'</li>
</ul>

<H4>Building with Visual Studio</H4>
<ol>
	<li>Open ClusterIO.sln</li>
	<li><b>[Debug->ClusterIO Properties->Configuration Properties->C/C++->General->Additional Include Directories]</b> 
	Point to your installation of Storyboard Engine's include directory.
	(e.g. C:\Program Files (x86)\Crank_Software\Storyboard_Engine\5.0.201702270437\win32-x86-opengles_2.0-obj\include)</li>
	<li><b>[Debug->ClusterIO Properties->Configuration Properties->Linker->General->Additional Library Directories]</b> 
	Point to your installation of Storyboard Engine's lib directory.
	(e.g. C:\Program Files (x86)\Crank_Software\Storyboard_Engine\5.0.201702270437\win32-x86-opengles_2.0-obj\lib)</li>
	<li><b>[Debug->ClusterIO Properties->Configuration Properties->Linker->Input->Additional Dependencies]</b> 
	Append libgreio.lib if it is not in the list.<br>
	If using VS2015 or newer: Also append legacy_stdio_definitions.lib</li>
	<li><b>Build->Build Solution</b></li>
	<li><b>Debug->Start Debugging</b></li>
</ol>

Clients building this sample for Visual Studio may require additional standard C
library headers (inttypes.h, stdint.h) depending on their version of Visual Studio.<br>

Storyboard Suite provides the missing headers for these types in the
<i>include/[win32|wince|wincompact7]</i> sub-directories of the Windows
flavoured Storyboard Engine distributions.<p>

<H2>Using the Simulator as the Target</H2>

To use the simulator in Storyboard Designer you need to make sure the
Storyboard IO (a.k.a. GREIO) channel name is aligned with the channel
name in the source code ("cluster").<p>

To do this you need to open up the launch configuration and set the channel name as shown.<p>
<center><img height="480" width="640" src="./simulator.png"></img></center>

<H2>Running an Embedded <b>sbengine</b> Target</H2>

On a target system you would simply pass the Storyboard IO channel name ("cluster") on the
command line when starting sbengine.
<pre>
 #./sbengine -ogreio,channel=cluster .....
</pre>

<H2>Running the Demo</H2>
In order to run the Storyboard IO demonstration project, all that is required is
to first launch the Storyboard Application, either as a simulator or by using the
sbengine target command line, and then run the <b>cluster_update</b> application
from the command line.<p>

The start-up order is important since the Storyboard application will create the
initial channel for the cluster_update application to open.<p>

On QNX embedded systems, it is also required that the <b>mqueue</b> system service
be running before either the Storyboad Engine or the <b>cluster_update</b> application
are run as it provides the communication channel infrastructure for Storyboard IO.

</html>
