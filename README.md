# SPIRV-viewer
![alt tag](https://github.com/wangmaosong/SPIRV-Viewer/blob/master/screenShot.png?raw=true)
a tool for viewing SPIRV binaries as GLSL and SPIRV assembly as well as reflection data

Static viewing tool for the SPIRV file format. loads SPIRV binary files from disk.

Quick guide:

A SPIRV Binary can be decompiled into both GLSL(OpenGL and Vulkan) and SPIRV assembly as well as contain useful reflection information. For Example, a SPIRV binary file is loaded then is decompiled into Vulkan ready GLSL and SPIRV Assembly in addition to information regarding shader outputs and uniform buffers.

SPIRV information is broken down into GLSL source, SPIRV assembly and reflection information.

Source code is displayed in a text box with GLSL being in the right box and SPIRV assembly displayed in the left text box. As for reflection information here is what you can expect to see (in order of appearance):
- GLSL version
- Whether OpenGL ES is being used
- Floating point precision
- Integer precision
- Atomics
- Push constant buffers
- Sampled images
- Stage inputs
- Stage outputs
- Storage buffers
- Storage images
- Sub pass inputs
- Uniform buffer
